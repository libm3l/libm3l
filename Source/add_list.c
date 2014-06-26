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
 *     Function add_list.c
 *
 *     Date: 2012-06-24
 * 
 * 
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
 
#include "libm3l_header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "add_list.h"
#include "locate_list.h"
#include "FunctionsPrt.h"
#include "find_list.h"

lmint_t m3l_add(node_t **SList, node_t **TList, const lmchar_t *t_path, const lmchar_t *t_path_loc, opts_t *Popts)
{
/*
 * function is a  of the cp functions
 */

/* NOTE - check that Tfounds and SFounds are identical */

	if(*SList == *TList){
		Warning("add_list: can not add node to itself");
		return -1;
	}

	lmsize_t addlist, len;
	find_t *TFounds;
/*
 * check if data set exists
 */
	if(*SList == NULL){
		Warning("ln: NULL source list");
		return -1;
	}

	if(*TList == NULL){
		Warning("ln: NULL target list");
		return -1;
	}
/*
 * check only one node is to be moved to the Tlist
 */
	len = strlen(t_path_loc);
	if(strncmp(t_path_loc, "./", 2) == 0 && len == 2){
		if( strncmp( (*TList)->type, "DIR", 3) == 0){
			addlist = m3l_add_list(SList, TList,Popts);
			return addlist;
		}
		else{
			Warning("add_list: List is not DIR");
			return -1;
		}
	}
	else{
/*
 * locate target; if target == NULL, just rename the node(s)
 */
		if ( (TFounds = m3l_locate( *TList, t_path, t_path_loc, Popts)) == NULL){
			Warning("add_list: no correct target specified");
			return -1;
		}
		else{
/*
 * check that target node is only 1
 */
			if(TFounds->founds != 1){
				Warning("mv: multiple target nodes");
				m3l_DestroyFound(&TFounds);
				return -1;
			}
					
			if( (addlist = m3l_add_list(SList, &TFounds->Found_Nodes[0]->List,  Popts )) < 0){
				Warning("problem in ladd_list");
			}
					
			m3l_DestroyFound(&TFounds);
			return addlist;
		}
	}
}







lmint_t m3l_add_list(node_t **List, node_t **WTAList, opts_t *Popt)
{
/*
 * function adds list to the list tree
 * Input is a node to add (**List) and node where to add (**WTAList)
  * if the node where to add is DIR, the node is added to the DIR either at the beginning (option b) or at the end (option e)
  * if the node where to add is FILE type, the node is add before that node (b) or after that node (a).
  * in each case the count of parent DIR is increased by 1
  *
  * Upon return function returns:
  *    1 - if sucess
  *   0 of no scenario where and how to add a node was found
  *   -1 if node which is to be added (**List) is NULL
  *  -2 of node specifying where to add nodel (**WTAList) is NULL
 */
	node_t *WTAChild;

	if((*List) == NULL){
/*
 * specified list does not exist
 */
		Warning("add_list: : NULL list");
		return -1;
	}
/*
 * check that the list does not have parent
 */
// 	if( (*List)->parent != NULL){;
// 		Warning("add_list: list already a member");
// 		return -1;
// 	}

	if((*WTAList) == NULL){
/*
 * specified list does not exist
 */
		Warning("add_list: : NULL list");
		return -2;
	}
		
	if(strncmp( (*WTAList)->type, "DIR", 3) != 0){
/*
 * List is not DIR type
 */
		if(Popt != NULL && Popt->opt_b == 'b'){
/*
 * add node before WTAList
 */		
			(*List)->parent  = (*WTAList)->parent;
			(*List)->prev    = (*WTAList)->prev;
			(*List)->next    = (*WTAList);

			(*WTAList)->prev = (*List);
/*
 * increase counter of number of items in parent list
 */
			((*WTAList)->parent)->ndim++;
			return 1;
		}
		else
		{

/*
 * add node after WTAList
 */
			(*List)->parent  = (*WTAList)->parent;
			(*List)->prev    = (*WTAList);
			(*List)->next    = (*WTAList)->next;

			(*WTAList)->next = (*List);
/*
 * increase counter of number of items in parent list
 */
			((*WTAList)->parent)->ndim++; 
			return 1;
		}
	}
	else
	{
/*
 * WTAList is DIR type
 */
		WTAChild = (*WTAList)->child;

		if(Popt != NULL && Popt->opt_b == 'b'){
/*
 * add node at the beginning of the line of children
 */
			(*List)->parent  = (*WTAList);
			(*List)->prev    = NULL;
			(*List)->next    = WTAChild;

			if(WTAChild != NULL)WTAChild->prev = (*List);
			(*WTAList)->child = (*List);
			
/*
 * increase counter of number of items in parent list
 */
			(*WTAList)->child = (*List);
			(*WTAList)->ndim++;
			return 1;
		}
		else
		{
/*
 * loop until the end of line is reached
 */
			if(WTAChild != NULL){
				while(WTAChild->next != NULL)
					WTAChild = WTAChild->next;
				
				WTAChild->next = (*List);
			}
			else{
				(*WTAList)->child = (*List);
			}
/*
 * WTAChild now points at the last list in node, att new node behind it
 */
			(*List)->prev   = WTAChild;
			(*List)->next   = NULL;
			(*List)->parent = (*WTAList);
/*
 * increase counter of number of items in parent list
 */
			(*WTAList)->ndim++;
			return 1;
		}
	}
/*
 * something went wrong, did not identify where to add node
 */
return -3;
}

