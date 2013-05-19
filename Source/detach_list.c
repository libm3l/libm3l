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
 *     Function detach_list.c
 *
 *     Date: 2013-02-09
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
 *
 *
 *     Description
 * 
 */




 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "detach_list.h"
#include "locate_list.h"
#include "FunctionsPrt.h"
#include "find_list.h"

/*
 * function detach list. 
 */

find_t *m3l_detach_caller(node_t **List, const lmchar_t *path, const lmchar_t *path_loc, opts_t *Popts)
{
	lmsize_t i, detached_nodes;
	find_t *Founds;
	lmint_t init_call;
/*
 * call locator to locate nodes to be deleted
 */
	if ( (Founds = m3l_locator_caller( *List, path, path_loc, Popts)) == NULL){
		return 0;
	}
	else
	{
/* 
 * if number of founds = 1 and found list is idenital to List, list can not be detached
 */
		if(Founds->founds == 1 && Founds->Found_Nodes[0]->List == (*List)){
			Warning("Detach_list: List does not have parent, can not be detached");
			m3l_DestroyFound(&Founds);
			return NULL;
		}
				
		detached_nodes = 0;
		
		for(i=0; i< Founds->founds; i++){
						
			if( (detached_nodes = m3l_detach_list(init_call, &Founds->Found_Nodes[i]->List, Popts)) < 0){
				Warning("problem in rm_list");
			}
			else{
				detached_nodes++;
			}
		}
		
		if(detached_nodes != Founds->founds){
			printf("Number of detached nodes: %ld \n",detached_nodes);
			printf("Number of nodes nodes indentified for detaching: %ld \n",Founds->founds);
			Warning("Detach_list: number of detached nodes different from number of identified nodes");
		}
				
		return Founds;
	}
}


lmsize_t m3l_detach_list(lmint_t call, node_t **List, opts_t *Popts)
{
/*
 * function detaches all items in List
 */
	node_t *PAR, *CLD, *NEXT, *PREV, *CURR,  *Tmpnode, *Tmpnode1, *TmpH;
	lmsize_t rmnodes, i;

	if((*List) == NULL){
/*
 * specified list does not exist
 */
		Warning("detach_list: : NULL list");
		return -1;
	}
/*
 * check if list has parent node
 */
	
	if((*List)->parent == NULL){
/*
 * specified list does not have parent, return 0
 */
		return 0;
	}
/*
 * List does not have children
 */
	PAR  =  (*List)->parent;
	CLD  =  (*List)->child;
	NEXT =  (*List)->next;
	PREV =  (*List)->prev;
	CURR =  (*List);

	if(PREV == NULL && NEXT == NULL){
/*
 * the only node in DIR list
 *	set number if items in parent list 0
 *	set cld pointer of the parent list to NULL
 */
			((*List)->parent)->child = NULL;
		}
		else if (PREV == NULL)
		{
/*
 * first node in DIR list
 *	set prev pointer of the next node to NULL, this node will be head of the list
 *	set child pointer of the parent list pointing to next node
 */ 
			NEXT->prev = NULL;
			((*List)->parent)->child = NEXT;
		}
 		else if(NEXT == NULL){
/*
 * last node in DIR list
 *	set the next pointer of the previuous node to NULL, will be the last node in list
 */
			PREV->next = NULL;
		}
		else
		{ 
/*
 * node is neither first or last
 *	set the next pointer of the previous node to nexnode
 *	set the previuous pointer of the next node to previuous
 */
			PREV->next = NEXT;
			NEXT->prev = PREV;
		}
/*
 * substract 1 from number of items in the parent node
 */
		(*List)->parent->ndim--;
/*
 * now detach node - set its parent, prev and next to NULL
 */
		(*List)->parent = NULL;
		(*List)->prev   = NULL;
		(*List)->next   = NULL;

/*
 * return value 1 meaning one detached node
 */
		return 1;
}