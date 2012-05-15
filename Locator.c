/*
 * Modifications:
 * version	name		date		description (event. patch number)
 */

#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Locator.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"

static int match_test(node_t *, get_arg_t);
static find_t *locator(find_t *, path_t *, const char *, opts_t *);

extern int optind;
static int verbose_flag;

find_t *locator_caller(node_t *List, const char *path, const char *path_loc, opts_t *Popts)
{
	path_t *parsed_path;
	char *search_term, *node_path;
	size_t i;
	find_t *Founds, *Founds_Loc;
	node_t *Tmp_node;
/*
 * parse path
 */
	if( (parsed_path = parse_path(path)) == NULL){
		Error("Error in path");
		return (find_t *)NULL;
	}
	
	for (i=0; i< parsed_path->seg_count; i++)
/*
 * call find function with specified options
 * First look if ../ are in path or if path is absolute path
 * set initial node, if path contains ../ go to higher lever
 */
	Tmp_node = List;

	if(parsed_path->abspath == 'A'){
		
		while(Tmp_node->parent != NULL)Tmp_node = Tmp_node->parent;
/*
 * check if first segment is identical to name of initial node
 */
		if(strncmp(Tmp_node->name, parsed_path->path[0], strlen(Tmp_node->name)) != 0){
			Error("Wrong absolute path");
			destroy_pars_path(&parsed_path);
			return (find_t *)NULL;
		}
	}
	else{

		for(i=0; i<parsed_path->seg_count; i++){
			if(strncmp(parsed_path->path[i], "..", 2) == 0){
				if ( (Tmp_node = Founds->Home_Node->parent) == NULL){
					Error("Wrong path");
					destroy_pars_path(&parsed_path);
					return (find_t *)NULL;
				}
			}
		}
	}

	if( strncmp(Tmp_node->type, "DIR", 3) != 0){
		Warning("List in locate is not DIR");
		free(search_term);
		destroy_pars_path(&parsed_path);
		return (find_t *)NULL;
	}
/* 
 * this function returns back found_t **pointer which has "founds" number of items
 * do not forget to free it when you do not need it
 */
	if ( (search_term = strdup(parsed_path->path[parsed_path->seg_count-1])) == NULL)
			Perror("strdup");
	if(Popts->opt_i == 'i')search_term = StrToLower(search_term);
		
	if ( (Founds = Find_caller(Tmp_node, search_term, Popts)) == NULL){
		free(search_term);
		destroy_pars_path(&parsed_path);
		return (find_t *)NULL;
	}
	else
	{
/*
 * write the values of the find result
 */
		printf(" number of founds is %ld \n", Founds->founds);
		for (i=0; i< Founds->founds; i++){
			printf("Name of found subset is --- pointer is %p\n", Founds->Found_Nodes[i]->List);
			
 			if( (node_path = Path(Founds->Found_Nodes[i]->List, Founds->Home_Node)) != NULL){
				printf(" Path is %s   %p   %p \n", node_path , Founds->Found_Nodes[i]->List, Founds->Home_Node);
				free(node_path);
			}
			
		}
/*
 * call locator to select sets
 */		
		printf(" Going to locator\n");
		if ( (Founds_Loc = locator(Founds, parsed_path, path_loc, Popts)) == NULL){
			printf(" After NULL locator\n");

			DestroyFound(&Founds);
			destroy_pars_path(&parsed_path);
			free(search_term);
			return (find_t *)NULL;
		}
		
		printf(" After locator\n");


	}	
//		NOTE: if(word != NULL) free(word);
	free(search_term);
	DestroyFound(&Founds);
 	destroy_pars_path(&parsed_path);	
	
	return Founds_Loc;
}


find_t *locator(find_t *Founds, path_t *parsed_path, const char *path_loc, opts_t *Popt)
{
/*
 * function looks for subset in nodel List
 * and specified by path and location
 * before this function is invoked, the initial Founds subset must be
 * created by calling Founds function
 */	
	node_t *Tmp, *Tmppar;
	size_t *HelpNodeI;
	size_t i, j, k, counter;
	path_t *parsed_path_loc, **parsed_path_founds;;
	get_arg_t argsstr;
	find_t *RetFound;
	size_t tot_match;
	char *node_path;
/*
 * parse path location specification; IMP: do not forget destroy_pars_path(&parsed_path) once not needed
 */
 	if ( (parsed_path_loc = parse_path(path_loc)) == NULL){
		Error("Path - failed");
		return (find_t *)NULL;
	}

	if(parsed_path->seg_count != parsed_path->seg_count){
		destroy_pars_path(&parsed_path_loc);
		Error("Number of items in path different from location specification");  /* NOTE - in later versions, ust one symbol '*' can be used for all paths segments */
	}
	
		for (i=0; i< parsed_path_loc->seg_count; i++)
		printf(" Segment_loc %d is %s\n", i, parsed_path_loc->path[i]);
		printf(" number of founds is %d   %d   %d\n", Founds->founds, parsed_path->seg_count ,parsed_path->seg_count );
/*
 * allocate field for positive match and for segments of path for each element of Found
 */
	if ( (HelpNodeI = malloc(Founds->founds * sizeof(size_t))) == NULL)
		Perror("malloc");
	if ( (parsed_path_founds = (path_t **)malloc(Founds->founds * sizeof(path_t **))) == NULL)
		Perror("malloc");
	
	for(i=0; i < Founds->founds; i++){
  		HelpNodeI[i] = 1;
		if( (node_path = Path(Founds->Found_Nodes[i]->List, Founds->Home_Node)) == NULL){
			Error(" Path error");
			destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
			for(j=0; j <=i; j++)
				destroy_pars_path(&parsed_path_founds[j]);
			free(parsed_path_founds);
			return (find_t *)NULL;
		}
		if( (parsed_path_founds[i] =  parse_path(node_path)) == NULL){
			Error(" Path error");
			destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
			for(j=0; j <=i; j++)
				destroy_pars_path(&parsed_path_founds[j]);
			free(parsed_path_founds);
			return (find_t *)NULL;
		}
		printf(" Path is %s  \n", node_path );
		free(node_path);
	}
	
	printf("End of cycle\n");	


	for(i=0; i < Founds->founds; i++){
		for (j=0; j< parsed_path_founds[i]->seg_count; j++)
			printf("-%s-", parsed_path_founds[i]->path[j]);
		printf("\n");
	}

// 	exit(0);
/*
 * loop over all levels in path, segment by segment and determine
 * if match is positive or negative
 */

	Tmppar = Founds->Found_Nodes[0]->List->parent;
	counter = 1;

	for(i=0; i<parsed_path_loc->seg_count; i++){
/*
 * get arguments for path segment
 */
 		printf(" Segment_loc %d is %s\n", i, parsed_path_loc->path[i]);
		argsstr = get_arguments(parsed_path_loc->path[i]);

		if(argsstr.retval == -1){
			Error("argstr error");
			destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
			for(j=0; j <Founds->founds; j++)
				destroy_pars_path(&parsed_path_founds[j]);
			free(parsed_path_founds);
			return (find_t *)NULL;
		}
/*
 * loop over founds and check for match
 */
		for(j = 0; j< Founds->founds; j++){

			if( HelpNodeI[j] == 1){
/*
 * node is considered as possible match
 */
/*
 * compare i-th segment of the path, if match, go with tests of location
 */
				if(strncmp(parsed_path->path[i], parsed_path_founds[j]->path[i], strlen(parsed_path->path[i])) == 0){
/*
 * segments are equal, check locator
 * find node_t pointer corresponding to path segment
 */
					Tmp = Founds->Found_Nodes[j]->List;

					for(k=i+1; k<parsed_path_loc->seg_count; k++)
						Tmp = Tmp->parent;
/*
 * get counter, increment for each in the same DIR, set 0 if different DIR
 */

					printf("%s %p  %p \n", Tmp->name , Tmp->parent, Tmppar);

					if(Tmppar == Tmp->parent){
						counter++;
					}
					else{
						counter = 1;
						Tmppar = Tmp->parent;
					}

					printf("Counter is %d\n", counter);

// 					HelpNodeI[j]  = match_test(Tmp,argsstr);
/*
 * argsstr.first if S or s, deal with subset
 * argsstr.s_name - is argsstr.first == ('s' || 'S') - specifies name of subset name
 * argsstr.arg - type of argument to be used
 * argsstr.args - value of argument to be used
 */
				}
				else
				{
					HelpNodeI[j]  = 0;
				}
			}
		}
	}
	exit(0);
/*
 * free parsed_path 
 */	
	destroy_pars_path(&parsed_path_loc);
/*
 * count how many matches are positive
 */
	tot_match = 0;
	for(j = 0; j< Founds->founds; j++){
		if(HelpNodeI[j] == 1) tot_match++;
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
		if( HelpNodeI[j] == 1)
			RetFound->Found_Nodes[j]->List=Founds->Found_Nodes[j]->List;
	}
	RetFound->founds = tot_match;
		
	free(HelpNodeI);
	free(node_path);
	for(j=0; j <Founds->founds; j++)
		destroy_pars_path(&parsed_path_founds[j]);
	free(parsed_path_founds);

	return RetFound;
}



int match_test(node_t *List, get_arg_t argsstr)
{
/*
 * find if what is to be comapred is set or subset
 */
	if( argsstr.first == ('s' || 'S')){
		
		return 0;
	}
	else{
/*
 * set
 */
		switch ( (int)argsstr.arg){
	
			case 'V':  /* Value */
				
			break;
		
		}
		
		return 0;
		
	}
	
	return 0;
	
}