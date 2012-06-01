/*
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "mv_list.h"
// #include "add_list.h"
#include "Locator.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"

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
	node_t *NewList;
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
  * free existing data set
 */
		if ( Free_data_str(TList) != 0){
			Error("Free_data_str");
			return -1;
		}
// 		if (  cp_recrt_list(TList, SList) < 1){
// 			Error("Copying list");
// 			return -1;
// 		}
		
		return 1;
	}
	else{
/*
 * copy content of the list
 */
		if(strncmp( (*SList)->type, "DIR", 3) == 0){
/*
 * SList is DIR, traverese and copy item-by-item
 */
// 			if ( (NewList = cp_crt_list(SList, Popts)) == NULL){
// 				Error("Copying list");
// 				return -1;
// 			}
		}
		else{
/*
 * Slist is FILE type, skip traversing and copy item directly
 */
// 			if ( (NewList = cp_crt_list_item(SList)) == NULL){
// 				Error("Copying list");
// 				return -1;
// 			}
		}
/*
 * add a new node to the DIR list
 */
		if ( add_list(&NewList, TList, Popts) < 0){
			Warning("Error cp_list copy");
			return -1;
		}
	}
}


