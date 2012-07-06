/*
 *     Copyright (C) 2012  Adam Jirasek
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU Lesser General Public License for more details.
 * 
 *     You should have received a copy of the GNU Lesser General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *     
 *     contact: libm3l@gmail.com
 * 
 */



/*
 *     Function Mount.c
 *
 *     Author: Adam Jirasek
 *     Date: 2012-06-30
 * 
 * 
 *     Description:
 * 
 *
 *     Input parameters:
 * 
 *
 *     Return value:
 * 
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		Author			Descritpion
 *
 */




 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Mount.h"
#include "FunctionsPrt.h"
#include "udf_rm.h"


/*
 * function creates the master head node of a specified name
 */

node_t *m3l_Mount(const char *name)
{
	tmpstruct_t TMPSTR;
	node_t *Dnode;
/*
 * copy name of the node to the TMPSTR
 */
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", name) < 0)
		Perror("snprintf");
/*
 * set type of node DIR
 */
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", "DIR") < 0)
		Perror("snprintf");
/*
 * node does not contain any data, set ndim = 0
 */
	TMPSTR.ndim = 0;
/*
 * allocate node and if success, return it otherwise return NULL
 */	
	if( (Dnode = m3l_AllocateNode(TMPSTR, NULL)) == NULL){
		Error("Allocate");
		return NULL;
	}
	
	return Dnode;
}
