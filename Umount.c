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
 *     Function Umount.c
 *
 *     Date: 2012-06-24
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
 *     Date		Version		Patch number		CLA 
 *     Description
 *
 */




 
#include "Header.h"
#include "format_type.h"


#include "Umount.h"
#include "FunctionsPrt.h"
#include "rm_list.h"


/*
 * this function deletes the master head node.
 * before doing so, remove all data sets in this node and 
 * then remove it.
 */

int m3l_Umount(node_t **List)
{
/*
 * delete everything in **List node
 */
	printf("Unmounting node %s at %p\n", (*List)-> name, (*List));
/*
 * when calling rm_list, specify 2 as if the function was not called initially
 * When called initially, the function consideres List as a parent dir and does not remove it
 * In this routine, the function is supposed to remove List so it needs to be called with 2
 */ 
	if( m3l_rm_list(2, List, (opts_t *)NULL) < 0){
		Error("Unable to unmount node \n");
		return -1;
	}
	printf("Unmounting finished\n");

	return 1;
}
