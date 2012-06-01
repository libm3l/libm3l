/*
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "mv_list.h"
#include "add_list.h"
#include "Locator.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"
#include "rm_list.h"

static int mv_list(int , node_t **, node_t **, opts_t *);
/*
 * function links list. If the list has children, it deletes them before removing list.
 * called recursivelly
 * upon return, returns number of deleted lists, upon failure returns -1
 */

size_t mv_caller(node_t **SList, const char *s_path, const char *s_path_loc, node_t **TList, const char *t_path, const char *t_path_loc, opts_t *Popts)
{
/*
 * function is a caller of the cp functions
 */

/* NOTE - check that Tfounds and SFounds are identical */
 
	size_t i, mv_tot_nodes, mv_nodes;
	find_t *SFounds, *TFounds;
	int init_call;
	char *name;
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
 * call locator to locate the target node
 */
	if ( (SFounds = locator_caller( *SList, s_path, s_path_loc, Popts)) == NULL){
		Warning("ln: NULL SFounds");
		return 0;
	}
/*
 * locate target; if target == NULL, just rename the node(s)
 */
	if ( (TFounds = locator_caller( *TList, t_path, t_path_loc, Popts)) == NULL){
/*
 * check that the parent direcotry is identical, if not return -1
 * check only one node is to be renamed, occurs if one source and target path is ./
 */
		if(strncmp(t_path_loc, "./", 2) == 0){
			for(i=0; i< SFounds->founds; i++){
				name = SFounds->Found_Nodes[i]->List->name;
				bzero(name, sizeof(name));
				if( snprintf(name,sizeof(name),"%s",t_path) < 0)
					Perror("snprintf");
			}
			DestroyFound(&SFounds);
			return 1;
		}
	}
	else{
/*
 * check that target node is only 1
 */
		if(TFounds->founds != 1){
			Warning("mv: multiple target nodes");
			DestroyFound(&TFounds);
			return -1;
		}
/*
 * check that if there is more then one source, the target node is DIR type
 */
		if( SFounds->founds > 1 && strncmp(TFounds->Found_Nodes[0]->List->type, "DIR", 3) != 0){
			Warning("cp: target node is not DIR");
			DestroyFound(&TFounds);
			DestroyFound(&SFounds);
			return -1;
		}

		mv_tot_nodes = 0;
			
		for(i=0; i< SFounds->founds; i++){
			
			if( (mv_nodes = (size_t) mv_list(init_call, &SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  Popts )) < 0){
				Warning("problem in ln_list");
			}
			else{
				mv_tot_nodes += mv_nodes;
			}
		}
				
		DestroyFound(&TFounds);
		DestroyFound(&SFounds);
		return mv_tot_nodes;
	}
}


int mv_list(int call, node_t **SList, node_t **TList, opts_t *Popts)
{
/*
 * function links list SList to a target list TList
 * 	if the SList is FILE, TLIST has to be file too. The data sunion and ->fdim of the TList target nodes is furst freed, then reallocated
 *	and then SList data union + ->fdim and ->ndim are copied.
 * 	if the TList is DIR the Slist is re-created and added to the TList. In case SList is a DIR, the copying is done by 
 *	traversing the list and copying it item-by-item
 */
	node_t *Prev, *Par, *Next;
	node_t *TPrev, *TPar, *TNext;
/*
 * save neighbours of the Slist
 */
		Next = (*SList)->next;
		Prev = (*SList)->prev;
		Par  = (*SList)->parent;

// 		(*SList)->next = NULL;
// 		(*SList)->prev = NULL;
// 		(*SList)->parent = NULL;
/*
 * copy source (Slist) to target (Tlist)
 */
	if(strncmp( (*TList)->type, "DIR", 3) != 0){
/*
 * *Tlist is FILE, check that SList is not DIR
 */
		if(strncmp( (*SList)->type, "DIR", 3) == 0){
			Warning(" cp_list: cannot overwrite non-DIR  with DIR ");
			return -1;
		}
/*
 * Save neighbours of the Tlist
 */
		TNext = (*TList)->next;
		TPrev = (*TList)->prev;
		TPar  = (*TList)->parent;
		
		bzero((*SList)->name, sizeof((*SList)->name));
		if( snprintf((*SList)->name, sizeof((*SList)->name),"%s", (*TList)->name) < 0){
			Perror("snprintf");
			return -1;
		}
/*
 * remove TList
 */		
		if ( rm_list(1, TList) < 1 ){
			Error("mv_list: rm_list");
			return -1;
		}
/*
 * Place Slist where TList was before
 */
		(*SList)->next    = TNext;
		(*SList)->prev    = TPrev;
		(*SList)->parent  = TPar;
		
		if((*SList)->next != NULL) TNext->prev = (*SList);
		if((*SList)->prev != NULL) TPrev->next = (*SList);
/*
 * if moved node is in the same directory as target node
 * dicrease number of items in the directory
 */
 		if(Par != TPar)
			TPar->ndim++;
	}
	else{
/*
 * add a new node to the DIR list
 */
		if ( add_list(SList, TList, Popts) < 0){
			Warning("Error mv_list copy");
			return -1;
		}
	}
/*
 * connect chain after removed SList
 * if Slist has a parent, dicrease number of nodes in it
 */

// 	printf("PPPPPPPPPP %p  %p  %p\n", Par, Prev, Next);

	if(Par != NULL) Par->ndim--;
	if(Par->ndim > 0){
/*
 * still nodes in directory
 */		
		if(Next != NULL && Prev != NULL){
/*
 * List is in the middle
 */
			Prev->next = Next;
			Next->prev = Prev;
		}
		else if(Next == NULL){
/*
 * List is the only child in DIR (this should actually never happen as the value of Par->ndim == 0
 */
			Prev->next = NULL;
			Par->child = NULL;
			Par->ndim = 0;
		}
		else if(Prev == NULL){
			Next->prev = NULL;
			Par->child = Next;
		}
	}
	else{
/*
 * after move, DIR is empty
 */
		Par->child = NULL;
	}
	return 1;
}


