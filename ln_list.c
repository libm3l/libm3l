/*
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "ln_list.h"
#include "add_list.h"
#include "Locator.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"

/*
 * function links list. If the list has children, it deletes them before removing list.
 * called recursivelly
 * upon return, returns number of deleted lists, upon failure returns -1
 */

size_t ln_caller(node_t *SList, const char *s_path, const char *s_path_loc, node_t **TList, const char *t_path, const char *t_path_loc, opts_t *Popts)
{
/*
 * function is a caller of the cp functions
 */
	size_t i, ln_tot_nodes, ln_nodes;
	find_t *SFounds, *TFounds;
	int init_call;
/*
 * check if data set exists
 */
	if(SList == NULL){
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
	if ( (TFounds = locator_caller( *TList, t_path, t_path_loc, Popts)) == NULL){
		Warning("ln: NULL TFounds");
		return 0;
	}
/*
 * check that target node is only 1
 */
	if(TFounds->founds != 1){
		Warning("cp: multiple target nodes");
		DestroyFound(&TFounds);
		return -1;
	}
	
	if ( (SFounds = locator_caller( SList, s_path, s_path_loc, Popts)) == NULL){
		DestroyFound(&TFounds);
		Warning("ln: NULL SFounds");
		return 0;
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
	
	
// 	printf(" number of found nodes is %ld   %ld\n", SFounds->founds, TFounds->founds);
	
	ln_tot_nodes = 0;
		
	for(i=0; i< SFounds->founds; i++){
		
// 		if( (ln_nodes = (size_t) ln_list(init_call, SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  Popts )) < 0){
// 			Warning("problem in ln_list");
// 		}
// 		else{
// 			ln_tot_nodes += ln_nodes;
// 		}
	}
			
	DestroyFound(&TFounds);
	DestroyFound(&SFounds);
	return ln_tot_nodes;
}

