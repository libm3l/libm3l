/*
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "cp_list.h"
// #include "rm_list.h"
#include "Locator.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"

static size_t cp_list(int , node_t **);


/*
 * function deletes list. If the list has children, it deletes them before removing list.
 * called recursivelly
 * upon return, returns number of deleted lists, upon failure returns -1
 */

size_t cp_caller(node_t *SList, const char *s_path, const char *s_path_loc, node_t **TList, const char *t_path, const char *t_path_loc, opts_t *Popts)
{
	size_t i, cp_tot_nodes, cp_nodes;
	find_t *SFounds, *TFounds;
	int init_call;
/*
 * call locator to locate the target node
 */
	if ( (TFounds = locator_caller( *TList, t_path, t_path_loc, Popts)) == NULL){
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
	
	
	cp_tot_nodes = 0;
	
	for(i=0; i< SFounds->founds; i++){
		if( (cp_nodes = cp_list(init_call, &SFounds->Found_Nodes[i]->List)) < 0){
			Warning("problem in rm_list");
		}
		else{
			cp_tot_nodes += cp_nodes;
			cp_nodes = 0;
		}
	}
			
			
	DestroyFound(&TFounds);
	DestroyFound(&SFounds);
	return cp_tot_nodes;
	
}


size_t cp_list(int call, node_t **List)
{

}
