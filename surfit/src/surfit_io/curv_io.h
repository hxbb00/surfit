
/*------------------------------------------------------------------------------
 *	$Id$
 *
 *	Copyright (c) 2002-2006 by M. V. Dmitrievsky
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

namespace surfit {

class d_curv;

SURFIT_IO_EXPORT
d_curv * _curv_load_bln(FILE * file, int & orient);

SURFIT_IO_EXPORT
/*! \ingroup internal_curv
    \fn d_curv * _curv_load_shp(const char * filename, const char * curvname);
    \brief loads \ref curv from ERSI shape file
*/
d_curv * _curv_load_shp(const char * filename, const char * curvname);

SURFIT_IO_EXPORT
/*! \ingroup internal_curv
    \fn bool _curvs_load_shp(const char * filename);
    \brief loads all curves from ESRI shape file
*/
bool _curvs_load_shp(const char * filename);

SURFIT_IO_EXPORT
bool _curv_save_bln(const d_curv * crv, FILE * file, int orient);

SURFIT_IO_EXPORT
/*! \ingroup internal_curv
    \fn bool _curv_save_shp(const d_curv * contour, const char * filename);
    \brief saves \ref curv to ERSI shape file
*/
bool _curv_save_shp(const d_curv * contour, const char * filename);

}; // namespace surfit;
