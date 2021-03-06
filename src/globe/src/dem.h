
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

#ifndef __globe__dem__
#define __globe__dem__

#include <vector>
#include <limits.h>

#include "surfit_data.h"

namespace surfit {

class shortvec;
class bitvec;
class datafile;
class d_grid;
class d_dem;

//! constructor for \ref d_dem
GLOBE_EXPORT
d_dem * create_dem(shortvec *icoeff, d_grid *igrd, 
		   const char * demname = 0, short dem_undef_value = SHRT_MAX);

//! Digital Elevation Model, based on the equidistant \ref d_grid with values set at the centers of the cells. 
class GLOBE_EXPORT d_dem : public data {
protected:
	//! constructor
	d_dem(shortvec *icoeff, d_grid *igrd, 
	      const char * demname = 0, short dem_undef_value = SHRT_MAX);

	//! destructor
	virtual ~d_dem();
	
public:

	//! constructor
	friend GLOBE_EXPORT
	d_dem * create_dem(shortvec *icoeff, d_grid *igrd, 
			   const char * demname, short dem_undef_value);

	//! calculates surface value at point (x,y)
	virtual short getValue(REAL x, REAL y) const;
	
	//! calculates surface value at point (x,y) using bilinear interpolation
	virtual REAL getInterpValue(REAL x, REAL y) const;

	//! returns DEM value at I,J node
	virtual short getValueIJ(size_t i, size_t j) const;

	//! calculates surface mean value for rect
	virtual REAL getMeanValue(REAL x_from, REAL x_to, REAL y_from, REAL y_to) const;

	//! returns minimum X-coordinate for the surface
	virtual REAL getMinX() const;

	//! returns maximum X-coordinate for the surface
	virtual REAL getMaxX() const;

	//! returns minimum Y-coordinate for the surface
	virtual REAL getMinY() const;

	//! returns maximum Y-coordinate for the surface
	virtual REAL getMaxY() const;

	//! calculates minimum and maximum Z values 
	virtual bool getMinMaxZ(REAL & minZ, REAL & maxZ) const;

	//! returns amount of cells for X direction
	virtual size_t getCountX() const;

	//! returns amount of cells for Y direction
	virtual size_t getCountY() const;

	//! returns distance between of centers of cells in X direction
	virtual REAL getStepX() const;

	//! returns distance between of centers of cells in Y direction
	virtual REAL getStepY() const;

	//! calculates coordinates of the center of the (i,j) cell
	virtual void getCoordNode(size_t i, size_t j, REAL & x, REAL & y) const;

	//! returns X-coordinate of the center of the (i,j) cell
	virtual REAL getCoordNodeX(size_t i) const;

	//! returns Y-coordinate of the center of the (i,j) cell
	virtual REAL getCoordNodeY(size_t j) const;

	//! returns i-cell number for X-coordinate
	virtual size_t get_i(REAL x) const;
	
	//! returns j-cell number for Y-coordinate
	virtual size_t get_j(REAL y) const;

	//! compares two dems by their grids
	virtual bool compare_grid(const d_dem * fnc) const;

	//! returns amount of cells with defined values
	virtual size_t defined() const;

	//! writes tag for saving dem to datafile 
	virtual bool writeTags(datafile * df) const;

	//! this = this + fnc
	virtual void plus(const d_dem * fnc);
	
	//! this = this - fnc, undef means no operation
	virtual void minus(const d_dem * fnc);

	//! this = this * fnc
	virtual void mult(const d_dem * fnc);
	
	//! this = this / fnc
	virtual void div(const d_dem * fnc);
	
	//! this = fnc
	virtual void set(const d_dem * fnc);

	//! this = this + val
	virtual void plus(short val);

	//! this = this - val
	virtual void minus(short val);

	//! this = this * val
	virtual void mult(short val);

	//! this = this / val
	virtual void div(short val);

	//! this = val
	virtual void set(short val);

	virtual bool bounds(REAL & minx, REAL & maxx, REAL & miny, REAL & maxy) const;

	//
	// wavelets section
	//

	//! makes one wavelet-decomposition
	virtual bool decompose();

	//! calls decompose() while difference between calc_approx_norm() is lower than eps
	virtual bool auto_decompose(REAL eps, int norm = 0);
	
	//! makes one wavelet-reconstruction
	virtual bool reconstruct();

	//! calls reconstruct() until all details will be removed
	virtual bool full_reconstruct();

	//! returns details size
	virtual size_t details_size() const;

	//! calculates norm
	virtual REAL calc_approx_norm(int norm_type) const;

	//! grid for basis surface placements
	d_grid * grd;

	//! coefficients for each basis surface
	shortvec * coeff;

	//! data for wavelets decomposition/reconstruction
	std::vector<shortvec *> * coeffs_store;
	//! data for wavelets decomposition/reconstruction
	std::vector<d_grid *> * grids_store;
	//! data for wavelets decomposition/reconstruction
	int store_pos;
	
	//! all values in coeff equal to undef_value interprets as undefined
	short undef_value;


};

//! collection of \ref d_dem objects
class GLOBE_EXPORT dems_container : public objects_container<d_dem>
{
public:
	virtual void push_back(d_dem * elem);
};

//! storage for dems
extern GLOBE_EXPORT dems_container * globe_dems;

}; // namespace surfit;

#endif

