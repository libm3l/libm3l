

/*
 * author: Adam Jirasek
 * date:   1-May-2012
 * version - 0.0
 */

/*
 * Modifications:
 * version	name		date		description (event. patch number)
 */

#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Locator.h"
#include "Find.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"

static int match_test(node_t *, get_arg_t);

extern int optind;
static int verbose_flag;

find_t *locator(size_t call, find_t *Founds, const char *path_loc, opts_t *Popt)
{
/*
 * function looks for subset in nodel List
 * and specified by path and location
 * before this function is invoked, the Founds subset must be
 * created by calling Founds function
 */	
	typedef struct tmpinfo {
		node_t *Tmpf;
		int found_positive;
	} tmpinfo_t;
	
	tmpinfo_t *HelpNodeI;
	
	size_t i, j;
	path_t *parsed_path_loc;
	get_arg_t argsstr;
	find_t *RetFound;
	size_t tot_match;
/*
 * parse path location specification; IMP: do not forget destroy_pars_path(&parsed_path) once not needed
 */
 	if ( (parsed_path_loc = parse_path(path_loc)) == NULL){
		Error("Paths - failed");
		return (find_t *)NULL;
	}
/*
 * allocate tmp field and fill it by initial data
 */
	if ( (HelpNodeI = (tmpinfo_t *)malloc(Founds->founds * sizeof(tmpinfo_t *))) == NULL)
		Perror("malloc");
	for(i=0; i<Founds->founds; i++){
		HelpNodeI[i].Tmpf = Founds->Home_Node;
		HelpNodeI[i].found_positive = 1;
	}
/*
 * loop over all levels in path, segment by segment and determine
 * if match is positive or negative
 */
	for(i=0; i<parsed_path_loc->seg_count; i++){
/*
 * get arguments for path segment
 */
		argsstr = get_arguments(parsed_path_loc->path[i]); /* NOTE - need to add test of succesfull return from function, if this is the case, do not forget to free 
									destroy_pars_path(&parsed_path_loc) and HelpNodeI*/
/*
 * loop over founds and check for match
 */
		for(j = 0; j< Founds->founds; j++){
		
			if(HelpNodeI[j].found_positive == 1){
				if( (HelpNodeI[j].found_positive = match_test(HelpNodeI[j].Tmpf,argsstr)) == 1)
					HelpNodeI[j].Tmpf = HelpNodeI[j].Tmpf->child;
			}
/*
 * argsstr.first if S or s, deal with subset
 * argsstr.s_name - is argsstr.first == ('s' || 'S') - specifies name of subset name
 * argsstr.arg - type of argument to be used
 * argsstr.args - value of argument to be used
 */
		}
	}
/*
 * free parsed_path 
 */	
	destroy_pars_path(&parsed_path_loc);
/*
 * count how many matches are positive
 */
	tot_match = 0;
	for(j = 0; j< Founds->founds; j++){
		if(HelpNodeI[j].found_positive == 1) tot_match++;
	}
/*
 * allocate find_t structure and fill it
 */
	if ( (RetFound = (find_t *) malloc( sizeof(find_t *))) == NULL)
		Perror("malloc");
	
	if ( (RetFound->Found_Nodes = (find_str_t **) malloc( sizeof(find_str_t **))) == NULL)
		Perror("malloc");
	
	for(j = 0; j< tot_match; j++){
		if ( ( RetFound->Found_Nodes[j] =(find_str_t *) malloc( sizeof(find_str_t *))) == NULL)
			Perror("malloc");
	}
	
	for(j = 0; j< Founds->founds; j++){
		if( HelpNodeI[j].found_positive == 1)
			RetFound->Found_Nodes[j]->List=Founds->Found_Nodes[j]->List;
	}
	RetFound->founds = tot_match;
		
	free(HelpNodeI);
}



int match_test(node_t *List, get_arg_t argsstr)
{
/*
 * find if what is to be comapred is set or subset
 */
	if( argsstr.first == ('s' || 'S')){
		
		
	}
	else{
/*
 * set
 */
		switch ( (int)argsstr.arg){
	
			case 'V':  /* Value */
				
			break;
		
		}
	}
	
}