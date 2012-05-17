/*
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "rm_list.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"

/*
 * function deletes list. If the list has children, it deletes them before removing list.
 * called recursivelly
 * upon return, returns number of deleted lists, upon failure returns -1
 */

SIZE_T rm_list(int call, node_t **List)
{
/*
 * function removes all items in List
 * if initial call (call==1) it does not remove the List dir as it considers it 
 * as a parent directory. When removing List is required (as in case of umount) 
 * the value of call during initial call has to be larger then 1
 */
	node_t *PAR, *CLD, *NEXT, *PREV, *Tmpnode, *Tmpnode1;
	size_t rmnodes;

	if((*List) == NULL){
/*
 * specified list does not exist
 */
		Warning("rm_list: : NULL list");
		return -1;
	}

	if( (*List)->child == NULL){
/*
 * List does not have children
 */
		PAR  =  (*List)->parent;
		CLD  =  (*List)->child;
		NEXT =  (*List)->next;
		PREV =  (*List)->prev;

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
			(*List)->child = NULL;
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
			rmnodes = rmnodes + rm_list(2, &Tmpnode);
			Tmpnode = Tmpnode1;
		}
/*
 * Free parent node
 * If initial call, do not remove
 */
 		if(call > 1){
			if( (*List)->ndim == 0){
				rmnodes = rmnodes + rm_list(2, List);
			}
			else
			{
				Warning("DIR is not empty, not removing it");
					return -2;
			}
 		}

		return rmnodes;
	}
/*
 * if, from whatever reason, the function gets here
 * return back unspecified error  (-2)
 */
	return -3;
}
