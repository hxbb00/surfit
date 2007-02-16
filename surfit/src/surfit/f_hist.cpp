
/*------------------------------------------------------------------------------
 *	$Id$
 *
 *	Copyright (c) 2002-2006 by M. V. Dmitrievsky and V. N. Kutrunov
 *	See COPYING file for copying and redistribution conditions.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	Contact info: surfit.sourceforge.net
 *----------------------------------------------------------------------------*/

#include "surfit_ie.h"

#include "fileio.h"

#include "f_hist.h"
#include "hist.h"
#include "hist_internal.h"
#include "vec.h"
#include "bitvec.h"
#include "intvec.h"
#include "grid_user.h"
#include "sort_alg.h"
#include "matr_diag.h"

#include <algorithm>
#include <float.h>

namespace surfit {

f_hist::f_hist(const d_hist * ihst, REAL imult) :
functional("f_hist", F_CONDI) 
{
	hist = ihst;
	mult = imult;
	mask = NULL;
};

f_hist::~f_hist() {
	cleanup();
};

void f_hist::cleanup() {
	if (mask)
		mask->release();
	mask = NULL;
};

int f_hist::this_get_data_count() const {
	return 1;
};

const data * f_hist::this_get_data(int pos) const {
	if (pos == 0)
		return hist;
	return NULL;
};

bool f_hist::make_matrix_and_vector(matr *& matrix, extvec *& v) {

	writelog(LOG_MESSAGE,"histogram \"%s\"", 
		hist->getName()?hist->getName():"noname");
	
	size_t matrix_size = method_basis_cntX*method_basis_cntY;

	REAL minz = FLT_MAX, maxz = -FLT_MAX;
	size_t i;
	for (i = 0; i < matrix_size; i++) {
		
		if (method_mask_solved->get(i))
			continue;
		if (method_mask_undefined->get(i))
			continue;

		REAL val = (*method_X)(i);
		if (val == FLT_MAX)
			continue;
		minz = MIN(minz, val);
		maxz = MAX(maxz, val);
	}

	if ((minz == FLT_MAX) || (maxz == -FLT_MAX))
		return false;

	if (mask)
		mask->release();
	mask = create_bitvec(matrix_size);
	mask->init_true();

	bitvec * mask_solved_undefined = create_bitvec(method_mask_solved);
	mask_solved_undefined->OR(method_mask_undefined);
	size_t solved_or_undefined = mask_solved_undefined->true_size();

	size_t intervs = hist->size();

	d_hist * surf_hist = _hist_from_extvec(method_X, minz, maxz, intervs, FLT_MAX, mask_solved_undefined);

	if (surf_hist->get_step() == 0) {
		mask_solved_undefined->release();
		surf_hist->release();
		return false;
	}
	
	surf_hist->normalize();
	vec * T = surf_hist->get_cumulative_hist();

	d_hist * dest_hist = create_hist(hist);
	dest_hist->normalize();
	REAL elem = REAL(1)/matrix_size;
	REAL dest_minz = dest_hist->from();
	REAL dest_maxz = dest_hist->to();
	
	for (i = 0; i < matrix_size; i++) {
		if (method_mask_solved->get(i) == false)
			continue;
		
		REAL val = (*method_X)(i);

		if (val > dest_maxz)
			continue;
		if (val < dest_minz)
			continue;

		size_t pos = (*dest_hist)(val);
		
		(*dest_hist)(pos) = MAX(0, (*dest_hist)(pos)-elem);

	}

	dest_hist->normalize();

	vec * Z = dest_hist->get_cumulative_hist();

	size_t points = 0;

	v = create_extvec(matrix_size);
	extvec * diag = create_extvec(matrix_size);

	for (i = 0; i < matrix_size; i++) {
		if (mask_solved_undefined->get(i) == true) {
			mask->set_false(i);
			continue;
		}
		REAL val = (*method_X)(i);
		REAL eqval = get_eq_value(T, Z, val,
					  minz, maxz,
					  dest_minz, dest_maxz);

		if (eqval > dest_maxz) {
			bool stop = true;
			eqval = get_eq_value(T, Z, val,
					  minz, maxz,
					  dest_minz, dest_maxz);
		}

		if (val != eqval) {
			REAL dist = fabs(eqval - val);
			(*v)(i) = eqval*mult;
			(*diag)(i) = mult;
			points++;
		}
	}

	mask_solved_undefined->release();

	matr_diag * M = new matr_diag(diag, matrix_size, mask);
	matrix = M;

	
	if (points == 0) {
		delete M;
		M = NULL;
		matrix = NULL;
		if (v)
			v->release();
		v = NULL;
	}
	
	if (T)
		T->release();
	if (Z)
		Z->release();
	if (surf_hist)
		surf_hist->release();
	if (dest_hist)
		dest_hist->release();

	

	/*
	// it is necessary to split for areas first!

	delete mask;
	mask = new bitvec(matrix_size);
	mask->init_true();

	vec * fit_hst = new vec(hst->size());

	// two stages algorithm

	// first stage is to find unchangable cells (first step is to fix solved cells)
	int i;

	int undef_size = 0;
	for (i = 0; i < matrix_size; i++) {

		if ( method_mask_undefined->get(i) ) {
			mask->set_false(i);
			undef_size++;
			continue;
		}

		if ( method_mask_solved->get(i) )  {
			REAL z = (*method_X)(i);
			mask->set_false(i);
			int pos;
			if ( hst->get_interv(z, pos) ) {
				(*fit_hst)(pos) += 1;
			} else {
				undef_size++;
			}

			continue;
		}

	};

	for (i = 0; i < matrix_size; i++) {

		if ( method_mask_undefined->get(i) ) 
			continue;
	
		if ( method_mask_solved->get(i) ) 
			continue;
		
		REAL z = (*method_X)(i);

		int pos;
		if ( hst->get_interv(z, pos) ) {

			REAL hst_val = *(hst->Z_begin + pos);
			REAL fit_val = (*fit_hst)(pos)*100/REAL(matrix_size - undef_size);
			REAL fit_val2 = ((*fit_hst)(pos)+1)*100/REAL(matrix_size - undef_size);

			bool fitted = (
			( fit_val > hst_val ) ||
			(( fabs(fit_val-hst_val) > fabs(fit_val2-hst_val) ) && (fit_val2 > hst_val))
			      );

			if (!fitted) {
				(*fit_hst)(pos) += 1;
				mask->set_false(i);
			} else {
			//	if (solved)
			//		undef_size++;
			}
				
		} else {
			//if (solved)
			//	undef_size++;
		}

	};

	// second stage - adding inequalities


	// sorting...
	REAL ** sort_begin = (REAL**) malloc( sizeof(REAL*) * matrix_size );
	REAL ** sort_end = sort_begin + matrix_size;
	REAL ** ptr;
    
	for (i = 0; i < matrix_size; i++) {
		ptr = sort_begin + i;
		*ptr = ( method_X->begin() + i );
	}
	
	std::sort(sort_begin, sort_end, ptr_vector_less);

	v = new vec(matrix_size);

	i = 0;
	int cnt;
	for (cnt = 0; cnt < hst->size(); cnt++) {
		REAL from    = *(hst->X1_begin + cnt);
		REAL to      = *(hst->X2_begin + cnt);
		REAL hst_val = *(hst->Z_begin + cnt);
		REAL fit_val = (*fit_hst)(cnt)*100/REAL(matrix_size - undef_size);
		REAL fit_val2 = ((*fit_hst)(cnt)+1)*100/REAL(matrix_size - undef_size);

		bool fitted = (
			( fit_val > hst_val ) ||
			(( fabs(fit_val-hst_val) > fabs(fit_val2-hst_val) ) && (fit_val2 > hst_val))
			      );

		while ( !fitted ) {

			if (i == matrix_size)
				break;

			int pos = *(sort_begin + i) - method_X->begin();
			REAL z = **(sort_begin + i);
			i++;

			if (mask->get(pos) == false)
				continue;

			if (z < from) {
				(*v)(pos) = from*mult;
				points++;
				(*fit_hst)(cnt) += 1;
			}

			if (z >= to) {
				(*v)(pos) = to*mult;
				points++;
				(*fit_hst)(cnt) += 1;
			}
				
			fit_val = (*fit_hst)(cnt)*100/REAL(matrix_size - undef_size);
			fit_val2 = ((*fit_hst)(cnt)+1)*100/REAL(matrix_size - undef_size);

			fitted = (
				( fit_val > hst_val ) ||
				(( fabs(fit_val-hst_val) > fabs(fit_val2-hst_val) ) && (fit_val2 > hst_val))
			         );


			if ( (z >= from) && (z < to) ) {
				bool jopa = true;
			}

		}
	}

	for (;i < matrix_size; i++) {
		int pos = *(sort_begin + i) - method_X->begin();
		mask->set_false(pos);
	}
	
	for (cnt = 0; cnt < fit_hst->size(); cnt++) {
		(*fit_hst)(cnt) *= 100/REAL(matrix_size - undef_size);
	}
	
	free(sort_begin);
	delete fit_hst;

	matr_ones * T = new matr_ones(mult, matrix_size, mask, method_mask_solved, method_mask_undefined);
	matrix = T;
	*/

	/*
	REAL ** sort_begin = (REAL**) malloc( sizeof(REAL*) * matrix_size );
	REAL ** sort_end = sort_begin + matrix_size;
	int cnt;
	REAL ** ptr;
    
	for (cnt = 0; cnt < matrix_size; cnt++) {
		ptr = sort_begin + cnt;
		*ptr = ( method_X->begin() + cnt );
	}
	
	std::sort(sort_begin, sort_end, ptr_vector_less);

	REAL z;

	cnt = 0;
	int undef_size = method_mask_undefined->true_size();

	delete mask;
	mask = new bitvec(matrix_size);
	mask->init_false();

	v = new vec(matrix_size);
	
	int i;
	for (i = 0; i < hst->size(); i++) {
		REAL from    = *(hst->X1_begin + i);
		REAL to      = *(hst->X2_begin + i);
		REAL hst_val = *(hst->Z_begin + i);

		int interv_cnt = 0;

		while ( interv_cnt*100/REAL(matrix_size - undef_size) < hst_val ) {

			if ( cnt == matrix_size )
				break;
			
			z = **(sort_begin + cnt);
			int pos = *(sort_begin + cnt) - method_X->begin();
			cnt++;

			if (method_mask_undefined->get(pos))
				continue;
			
			if (( z >= from ) && ( z < to )) {
				interv_cnt++;
				continue;
			}

			if (method_mask_solved->get(pos))
				continue;

			mask->set_true(pos);

			if (z < from) {
				(*v)(pos) = from*mult;
				points++;
				interv_cnt++;
			}

			if (z >= to) {
				(*v)(pos) = to*mult;
				points++;
				interv_cnt++;
			}
			
			
		}
		
		if ( cnt == matrix_size )
			break;

	};


	free(sort_begin);
	
	matr_ones * T = new matr_ones(mult, matrix_size, mask, method_mask_solved, method_mask_undefined);
	matrix = T;
	*/

	bool solvable = (points > 0);

	solvable = wrap_sums(matrix, v) || solvable;
	return solvable;

};

bool f_hist::solvable_without_cond(const bitvec * mask_solved,
				   const bitvec * mask_undefined,
				   const extvec * X)
{
	size_t matrix_size = method_basis_cntX*method_basis_cntY;

	size_t i;
	for (i = 0; i < matrix_size; i++) {
		
		if (mask_solved->get(i))
			continue;
		if (mask_undefined->get(i))
			continue;

		return false;

	}
	
	return true;
};

void f_hist::mark_solved_and_undefined(bitvec * mask_solved, bitvec * mask_undefined, bool i_am_cond) {
	
	return;

};

bool f_hist::minimize() {

	return false;
};

}; // namespace surfit;

