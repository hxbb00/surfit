
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

#ifndef __surfit_functionals_included__
#define __surfit_functionals_included__

#include <vector>

namespace surfit {

class matr;
class vec;
class d_curv;
class grid_line;
class data;
class bitvec;
class intvec;

#define F_USUAL	0x00000001
#define F_FAULT	0x00000010
#define F_CONDI 0x00000100  // this is a condition

/*! \class functional
    \brief abstract concept of functional
*/
class SURFIT_EXPORT functional {
public:
	
	//! constructor
	functional(const char * newname, int itype);
	
protected:
	//! destructor
	virtual ~functional();

public:
	//! destructor
	void release();

	//! makes system of linear equations and solves it with respect to \ref functionals_add and \ref functionals_cond members
	virtual bool minimize() = 0;
	
	//! fills matrix and right-side vector of system of linear equations and returns flag of it solvability 
	virtual bool make_matrix_and_vector(matr *& matrix, vec *& v) = 0;
	
	//! makes changes to mask_solved and mask_undefined
	virtual void mark_solved_and_undefined(bitvec * mask_solved, bitvec * mask_undefined, bool i_am_cond) = 0;

	//! returns true if functional minimization possible (with respect to conditional functionals)
	virtual bool solvable(const bitvec * mask_solved, 
			      const bitvec * mask_undefined,
			      const vec * X);

	//! returns true if functional minimization possible (with no respect to conditional functionals)
	virtual bool solvable_without_cond(const bitvec * mask_solved, 
					   const bitvec * mask_undefined,
					   const vec * X) = 0;
	
	//! sets functional name to newname 
	void setName(const char * newname);
	//! sets functional name to newname 
	void setNameF(const char *tmplt, ...);

	//! returns functional name 
	const char * getName() const;

	//! returns manager name; manager, that serves this functional
	virtual const char * getManagerName() const = 0;

	//! returns functional type
	int getType() const;

	//! frees some private data
	virtual void drop_private_data();

	//! cleanups all allocated memory 
	virtual void cleanup();

	//! adds functional to \ref functionals_add array
	void add_functional(functional * isum, REAL weight);

	//! prints some info about functional
	void info();

	//! returns number of referenced \ref data objects
	int get_data_count() const;
	//! returns referenced \ref data object
	const data * get_data(int pos) const;

	//
	// working with conditional functionals
	//

	//! calls \ref mark_solved_and_undefined for all funtionals in \ref functionals_cond array
	void cond_mark_solved_and_undefined(bitvec * mask_solved, bitvec * mask_undefined);
	
	//! makes one matrix and one right-side vector for all functionals in \ref functionals_cond array
	bool cond_make_matrix_and_vector(matr *& matrix, vec *& v, bitvec * mask);

	//! adds functional to \ref functionals_cond array
	void cond_add(functional * icond);

	//! moves all functionals from fnc's \ref functionals_cond array to own \ref functionals_cond array
	void cond_move(functional * fnc);

	//! returns true if \ref functionals_cond array isn't empty
	bool cond() const;

	//! returns number of elements in \ref functionals_cond array
	int cond_size() const;

	//! returns functional from \ref functionals_cond array
	functional * cond_get(int pos);

	//! returns true in case of all functional in \ref functionals_cond are solvable
	bool cond_solvable(const bitvec * mask_solved, 
			   const bitvec * mask_undefined,
			   const vec * X);

	//! removes functional from \ref functionals_cond array
	void cond_erase(int pos);

	//! removes all functionals from \ref functionals_cond array
	void cond_erase_all();

protected:

	//! returns number of \ref data refered to this functional only
	virtual int this_get_data_count() const = 0;
	//! returns \ref data refered to this functional only
	virtual const data * this_get_data(int pos) const = 0;

	//! returns number of \ref data refered to functionals in \ref functionals_add array
	int get_add_data_count() const;
	//! returns \ref data refered to functionals in \ref functionals_add array
	const data * get_add_data(int pos) const;

	//! modifies system of linear equations with respect to functionals from \ref functionals_add and \ref weights
	bool wrap_sums(matr *& matrix, vec *& v);
	//! calls \ref mark_solved_and_undefined for each functional in \ref functionals_add array
	void mark_sums(bitvec * mask_solved, bitvec * mask_undefined);
	
	//! name of functional
	char * name;
	
	//! array of functionals that should be added with \ref weights
	std::vector<functional *> * functionals_add;
	
	//! weights for \ref functionals_add
	vec * weights;

	//! array of conditions for functional minimization
	std::vector<functional *> * functionals_cond;

protected:

	//! functional type
	int type;

};

/*! \class faultable
    \brief fault line functional
*/
class SURFIT_EXPORT faultable : public functional {
public:
	//! constructor
	faultable(const char * newname, int itype);
	//! destructor
	virtual ~faultable();
	//! adds fault line to \ref faults array
	virtual void add_fault(const d_curv * flt);

protected:
	
	//! array of fault lines
	std::vector<const d_curv *> * faults;
	//! fault lines projected on \ref d_grid
	grid_line * gfaults;
};

}; // namespace surfit;

#endif

