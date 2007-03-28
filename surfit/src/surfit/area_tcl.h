
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

#ifndef __surfit_area_tcl_included__
#define __surfit_area_tcl_included__

namespace surfit {

///////////////////////
// saving and loading

/*! \ingroup tcl_area_save_load
    \fn bool area_read(const char * filename, const char * areaname=NULL, 
	       int col1=1, int col2=2,
	       const char * delimiters=" ", int skip_lines = 0, int grow_by=250);
    
    \par Tcl syntax:
    area_read \ref file "filename" "area_name" first_column_position second_column_position "delimiters" skip_lines

    \par Description:
    reads \ref d_area "area" from formatted text file and sets its name to "areaname"
    
    \param filename \ref file name of formatted text file
    \param areaname name for \ref d_area "area" dataset
    \param col1 column with X coordinates
    \param col2 column with Y coordinates
    \param skip_lines number of lines to skip header
    \param delimiters delimiters between columns. May be " ", "\t", "," or other symbols
    \param grow_by =250
    \par Example
    area_read "C:\\area.txt" my_area
    area_read "C:\\area2.txt" my_area2 2 3 "\t"
*/
bool area_read(const char * filename, const char * areaname=NULL	, 
	       int col1=1, int col2=2,
	       const char * delimiter=" \t", int skip_lines = 0, int grow_by=250);

/*! \ingroup tcl_area_save_load
    \fn bool area_load(const char * filename, const char * area_name = NULL);
    
    \par Tcl syntax:
    area_load \ref file "filename" "area_name"

    \par Description:
    loads \ref d_area "area" from surfit datafile. if no areaname specified, 
    then loads the first area in the file.

    \param filename surfit datafile \ref file filename
    \param area_name \ref d_area "area" name for load
    
    \par Example
    area_load "C:\\area.dat" my_area
*/
bool area_load(const char * filename, const char * area_name = NULL);

/*! \ingroup tcl_area_save_load
    \fn bool area_write(const char * filename, const char * area_name, const char * delimiter);
    
    \par Tcl syntax:
    area_write "filename" \ref str "area_name" "delimiter"

    \par Description:
    writes \ref d_area "area" to formatted text file

    \param filename text file filename
    \param delimiter delimiter between columns. May be " ", "\t", "," or other symbols
    
    \par Example
    area_write "C:\\area.txt" my_area
*/
bool area_write(const char * filename, const char * area_name = "*", const char * delimiter = "\t");

/*! \ingroup tcl_area_save_load
    \fn bool area_save(const char * filename, const char * area_name = "*");
    
    \par Tcl syntax:
    area_save "filename" \ref str "area_name"

    \par Description:
    saves \ref d_area "area" to surfit datafile

    \par Example
    area_save "C:\\area.dat" my_area
*/
boolvec * area_save(const char * filename, const char * area_name = "*");

//////////////
// other

/*! \ingroup tcl_area_other
    \fn const char * area_getName(const char * area_name = "*");
    
    \par Tcl syntax:
    area_getName \ref str "area_name"

    \par Description:
    returns name of \ref d_area 
*/
const char * area_getName(const char * area_name = "*");

/*! \ingroup tcl_area_other
    \fn bool area_setName(const char * new_name, const char * area_name = "*");
    
    \par Tcl syntax:
    area_setName new_name \ref str "area_name"

    \par Description:
    sets name of \ref d_area 
*/
bool area_setName(const char * new_name, const char * area_name = "*");

/*! \ingroup tcl_area_other
    \fn bool area_delall();
    
    \par Tcl syntax:
    area_delall

    \par Description:
    removes all areas from memory
*/
bool area_delall();

/*! \ingroup tcl_area_other
    \fn bool area_del(const char * area_name = "*");
        
    \par Tcl syntax:
    area_del \ref str "area_name"

    \par Description:
    removes \ref d_area from at 'area_name' position

*/
bool area_del(const char * area_name = "*");

/*! \ingroup tcl_area_other
    \fn int area_size();
    
    \par Tcl syntax:
    area_size

    \par Description:
    returns number of areas in memory 
*/
int area_size();

bool area_invert(const char * area_name = "*");

/*! \ingroup tcl_area_other
    \fn void areas_info();
    
    \par Tcl syntax:
    areas_info

    \par Description:
    prints information about all areas  
*/
void areas_info();

}; // namespace surfit;

#endif

