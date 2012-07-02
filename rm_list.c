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
 *     contact: clibm3l@gmail.com
 * 
 */



/*
 *     Function rm_list.c
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

#include "rm_list.h"
#include "locate_list.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "find_list.h"

/*
 * function deletes list. If the list has children, it deletes them before removing list.
 * called recursivelly
 * upon return, returns number of deleted lists, upon failure returns -1
 */

size_t rm_caller(node_t **List, const char *path, const char *path_loc, opts_t *Popts)
{
	size_t i, rm_tot_nodes, rm_nodes;
	find_t *Founds;
	int init_call;
/*
 * call locator to locate nodes to be deleted
 */
	if ( (Founds = locator_caller( *List, path, path_loc, Popts)) == NULL){
		return 0;
	}
	else
	{
/*
 * if required to keep the original node
 */
		if(Popts->opt_k == 'k'){
			init_call = 1;
		}
		else{
			init_call = 2;
/* 
 * if number of founds = 1 and found list is idenital to List, then 
 * remove entire list tree. In that case put *List = NULL
 * identical to Umount
 */
			if(Founds->founds == 1 && Founds->Found_Nodes[0]->List == (*List)) (*List) = NULL;
		}
				
		rm_tot_nodes = 0;
		
		for(i=0; i< Founds->founds; i++){
						
			if( (rm_nodes = rm_list(init_call, &Founds->Found_Nodes[i]->List, Popts)) < 0){
				Warning("problem in rm_list");
			}
			else{
				rm_tot_nodes += rm_nodes;
			}
		}				
				
		DestroyFound(&Founds);
		return rm_tot_nodes;
	}
}


size_t rm_list(int call, node_t **List, opts_t *Popts)
{
/*
 * function removes all items in List
 * if initial call (call==1) it does not remove the List dir as it considers it 
 * as a parent directory. When removing List is required (as in case of umount) 
 * the value of call during initial call has to be larger then 1
 */
	node_t *PAR, *CLD, *NEXT, *PREV, *CURR,  *Tmpnode, *Tmpnode1, *TmpH;
	size_t rmnodes, i;

	if((*List) == NULL){
/*
 * specified list does not exist
 */
		Warning("rm_list: : NULL list");
		return -1;
	}

// 	if(strncmp(List->type, "LINK",4) != 0){
/*
 * List is LINK or does not have children
 */
	if( (*List)->child == NULL || strncmp((*List)->type, "LINK", 4) == 0){
/*
 * List does not have children
 */
		PAR  =  (*List)->parent;
 		CLD  =  (*List)->child;
		NEXT =  (*List)->next;
		PREV =  (*List)->prev;
		CURR = (*List);

		if(PREV == NULL && NEXT == NULL){
/*
 * the only node in DIR list
 *	set number if items in parent list 0
 *	set cld pointer of the parent list to NULL
 * 
 * 	Specific situation is when the node is a master head node and everything in it has been 
 * 	already removed. This node parent is NULL
 */    
			if((*List)->parent != NULL){
/*
 * node is not a master head node
 */
				((*List)->parent)->child = NULL;
			}
			else
			{
/*
 * remove the master head node and return
 */
				if(Free(List) != 0)
					Error("Free");
/*
 * return value 1 meanign one deleted node
 */
				return 1;	
			}
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
 * if list is not empty LINK, nullify it's child and send info to link target about it
 */
		if( (*List)->child != NULL){
/*
 * NOTE - the linknode info is just NULLed but not freed
 */
			for(i=0; i<CLD->lcounter; i++){
				if(CLD->linknode[i]->List == CURR) CLD->linknode[i]->List = NULL;
/*
 * if required, clean-up the reference field
 */
				if(Popts != NULL && Popts->opt_c == 'c' && CLD != NULL) ln_cleanemptylinksref(&CLD);
			}
			(*List)->child = NULL;
		}
/*
 * if node is linked to other nodes, get the info about it to links
 */
		for(i=0; i<(*List)->lcounter; i++){
			TmpH =  (*List)->linknode[i]->List;
			if(  TmpH != NULL){
				 TmpH->child = NULL;
				 TmpH->ndim = 0;
			}
		}
/*
 * free the node and substract 1 from number of items in the parent node
 */
		(*List)->parent->ndim--;
		
		if(Free(List) != 0)
			Error("Free");
/*
 * return value 1 meanign one deleted node
 */
		return 1;
	}
	else
	{
/*
 * List has children
 *	set the counter of deleted nodes to 0
 *	set the temporary pointer to first child node
 */
		rmnodes = 0;
		
		if(strncmp( (*List)->type, "LINK", 4) == 0){
			Tmpnode =  *List;
		}
		else{;	
			Tmpnode =  (*List)->child;
		}
/*
 * remove all nodes in list, if node is DIR type, remove its children first (recursive call to rm_list)
 * increase counter of removed nodes
 */
		while(Tmpnode != NULL){
			Tmpnode1 = Tmpnode->next;
			rmnodes = rmnodes + rm_list(2, &Tmpnode, Popts);
			Tmpnode = Tmpnode1;
		}
/*
 * Free parent node
 * If initial call, do not remove
 */
 		if(call > 1){
			if( (*List)->ndim == 0){
				rmnodes = rmnodes + rm_list(2, List, Popts);
			}
			else
			{
				Warning("DIR is not empty, not removing it");
					return -2;
			}
 		}

		return rmnodes;
	}
// 	}
/*
 * if, from whatever reason, the function gets here
 * return back unspecified error  (-2)
 */
	return -3;
}
