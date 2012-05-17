

 
#include "Header.h"
#include "format_type.h"
#include "add_list.h"
#include "FunctionsPrt.h"

int add_list(node_t **List, node_t **WTAList, char option)
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
	node_t *Node, *WTAnode, *WTAChild;

	if((*List) == NULL){
/*
 * specified list does not exist
 */
		Warning("add_list: : NULL list");
		return -1;
	}

	if((*WTAList) == NULL){
/*
 * specified list does not exist
 */
		Warning("add_list: : NULL list");
		return -2;
	}
		Node        = *List;
		WTAnode     = *WTAList;

	if( (*List)->child == NULL){
/*
 * List does not have children
 */
		if(option == 'b'){
/*
 * add node before WTAList
 */		
			Node->parent  = WTAnode->parent;
			Node->prev    = WTAnode->prev;
			Node->next    = WTAnode;

			WTAnode->prev = Node;
/*
 * increase counter of number of items in parent list
 */
			((*WTAList)->parent)->ndim = 0;
			return 1;
		}
		else
		{

/*
 * add node after WTAList
 */
			Node->parent = WTAnode->parent;
			Node->prev    = WTAnode;
			Node->next    = WTAnode->next;

			WTAnode->next = Node;
/*
 * increase counter of number of items in parent list
 */
			((*WTAList)->parent)->ndim = 0; 
			return 1;
		}
	}
	else
	{
/*
 * WTAList is DIR type
 */

		WTAChild = (*WTAList)->child;

		if(option == 'b'){
/*
 * add node at the beginning of the line of children
 */
			Node->parent  = (*WTAList);
			Node->prev    = NULL;
			Node->next    = WTAChild;

			WTAChild->prev = Node;
			WTAnode->child = Node;    /* (*WTAList)->child */
			
/*
 * increase counter of number of items in parent list
 */
			((*WTAList)->parent)->ndim = 0;
			return 1;
		}
		else
		{
/*
 * loop until the end of line is reached
 */
			while(WTAChild->next != NULL)
				WTAChild = WTAChild->next;
/*
 * WTAChild now points at the last list in node, att new node behind it
 */

			Node->prev   = WTAChild;
			Node->next   = NULL;
			Node->parent = WTAnode;

			WTAChild->next = Node;
/*
 * increase counter of number of items in parent list
 */
			((*WTAList)->parent)->ndim = 0;
			return 1;
		}
	}
/*
 * something went wrong, did not identify where to add node
 */
return 0;
}

