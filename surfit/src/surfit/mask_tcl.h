
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

#ifndef __surfit__mask_tcl__
#define __surfit__mask_tcl__

namespace surfit {

class d_grid;
class boolvec;
class datafile;
class d_mask;
class strvec;

//
// SAVE_LOAD
//

/*! \ingroup tcl_mask_save_load
    \fn bool mask_load(const char * filename, const char * mask_name = NULL);
    
    \par Tcl syntax:
    mask_load \ref file "filename" \ref str "mask_name"

    \par Description:
    loads \ref d_mask "mask" from surfit datafile. If no mask_name specified, then first mask will be loaded.
    \param filename surfit datafile
    \param maskname name for mask (optional)
*/
boolvec * mask_load(const char * filename, const char * mask_name = NULL);

/*! \ingroup tcl_mask_save_load
    \fn bool mask_save(const char * filename, const char * mask_name = "*");

    \par Tcl syntax:
    mask_save \ref file "filename" \ref str "mask_name"

    \par Description:
    saves first \ref d_mask "masks" with name matching \ref str "mask_name" to surfit datafile
*/
boolvec * mask_save(const char * filename, const char * mask_name = "*");

//
// math
//

/*! \ingroup tcl_mask_math
    \fn bool mask_getValue(REAL x, REAL y, const char * mask_name = "*");

    \par Tcl syntax:
    mask_getValue x y \ref str "mask_name"

    \par Description:
    calculates \ref d_mask "mask" value at point (x,y) for the first \ref d_mask "mask" with name matching \ref str "mask_name"
*/
boolvec * mask_getValue(REAL x, REAL y, const char * mask_name = "*");

/*! \ingroup tcl_mask_math
    \fn void mask_and(const char * mask1_name = "*", const char * mask2_name = "*");

    \par Tcl syntax:
    mask_and \ref str "mask1_name" \ref str "mask2_name"

    \par Description:
    makes AND operation:
    mask1 = mask1 AND mask2 
*/
void mask_and(const char * mask1_name = "*", const char * mask2_name = "*");

/*! \ingroup tcl_mask_math
    \fn void mask_not(const char * mask1_name = "*", const char * mask2_name = "*");

    \par Tcl syntax:
    mask_not \ref str "mask1_name" \ref str "mask2_name"
    
    \par Description:
    makes NOT operation:
    mask1 = NOT mask2
*/
void mask_not(const char * mask1_name = "*", const char * mask2_name = "*");

/*! \ingroup tcl_mask_math
    \fn void mask_or(const char * mask1_name = "*", const char * mask2_name = "*");

    \par Tcl syntax:
    mask_or \ref str "mask1_name" \ref str "mask2_name"
    
    \par Description: 
    makes OR operation:
    mask1 = mask1 OR mask2
*/
void mask_or(const char * mask1_name = "*", const char * mask2_name = "*");

/*! \ingroup tcl_mask_math
    \fn void mask_xor(const char * mask1_name = "*", const char * mask2_name = "*");
    
    \par Tcl syntax:
    mask_xor \ref str "mask1_name" \ref str "mask2_name"
      
    \par Description:
    makes XOR operation:
    mask1 = mask1 XOR mask2
*/
void mask_xor(const char * mask1_name = "*", const char * mask2_name = "*");

//
// CONVERTING
//

/*! \ingroup tcl_mask_conv
    \fn bool mask_to_surf(const char * mask_name = "*");

    \par Tcl syntax:
    mask_to_surf \ref str "mask_name"

    \par Description:
    makes \ref d_surf "surface" from \ref d_mask "mask"
*/
void mask_to_surf(const char * mask_name = "*");

//
// OTHER
//

/*! \ingroup tcl_mask_other
    \fn void mask_from_surf(const char * surface_name = "*");

    \par Tcl syntax:
    mask_from_surf \ref str "surface_name"

    \par Description:
    makes \ref d_mask "mask" from \ref d_surf "surface". Resulting mask will have
    false values for all cells, where surface have undefined values.
*/
void mask_from_surf(const char * surface_name = "*");

/*! \ingroup tcl_mask_other
    \fn bool mask_apply_to_surf(const char * mask_name = "*", const char * surface_name = "*");

    \par Tcl syntax:
    mask_apply_to_surf \ref str "mask_name" \ref str "surface_name"

    \par Description:
    sets undefined values for all  \ref d_surf "surface" cells where \ref d_mask "mask" have false values.
*/
boolvec * mask_apply_to_surf(const char * mask_name = "*", const char * surface_name = "*");

/*! \ingroup tcl_mask_other
    \fn const char * mask_getName(const char * mask_name = "*");

    \par Tcl syntax:
    mask_getName \ref str "mask_name"

    \par Description:
    returns name of the \ref d_mask "mask"
*/
strvec * mask_getName(const char * mask_name = "*");

/*! \ingroup tcl_mask_other
    \fn const char * mask_getId(const char * mask_name = "*");

    \par Tcl syntax:
    mask_getId \ref str "mask_name"

    \par Description:
    returns unique \ref d_mask "mask" identificator
*/
intvec * mask_getId(const char * mask_name = "*");

/*! \ingroup tcl_mask_other
    \fn void mask_setName(const char * new_name, const char * mask_name = "*");
    
    \par Tcl syntax:
    mask_setName "new_name" \ref str "mask_name"

    \par Description:
    sets name for the \ref d_mask "mask"
*/
void mask_setName(const char * new_name, const char * mask_name = "*");

/*! \ingroup tcl_mask_other
    \fn void mask_del(const char * mask_name = "*");

    \par Tcl syntax:
    mask_del \ref str "mask_name"

    \par Description:
    removes \ref d_mask mask from memory
*/
void mask_del(const char * mask_name = "*");

/*! \ingroup tcl_mask_other
    \fn int mask_size();

    \par Tcl syntax:
    mask_size

    \par Description:
    returns number of \ref d_mask masks in memory
*/
int mask_size();

/*! \ingroup tcl_mask_other
    \fn void masks_info(const char * mask_name = "*");

    \par Tcl syntax:
    masks_info \ref str "mask_name"

    \par Description:
    prints information about \ref d_mask masks in memory
*/
void mask_info(const char * mask_name = "*");

}; // namespace surfit

#endif



