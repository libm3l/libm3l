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

static int match_test(node_t *, get_arg_t, size_t);
static find_t *locator(find_t *, path_t *, path_t *, opts_t *);

extern int optind;
static int verbose_flag;

find_t *locator_caller(node_t *List, const char *path, const char *path_loc, opts_t *Popts)
{
	path_t *parsed_path, *parsed_path_loc; // **parsed_path_founds;
	char *search_term, *node_path;
	size_t i; // j;
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
		printf(" --%s--", parsed_path->path[i]);
	printf("\n");
	
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
		if( (parsed_path_loc = parse_path(path_loc)) == NULL){
			free(search_term);
			destroy_pars_path(&parsed_path);
			Error("Path2 failed");
			return (find_t *)NULL;
		}
		if(parsed_path->seg_count != parsed_path_loc->seg_count){
			destroy_pars_path(&parsed_path_loc);
			free(search_term);
			destroy_pars_path(&parsed_path);
			Error("Number of items in path different from location specification");  /* NOTE - in later versions, ust one symbol '*' can be used for all paths segments */
			return (find_t *)NULL;
		}
		
 		Founds_Loc = locator(Founds, parsed_path, parsed_path_loc, Popts);
	
//		NOTE: if(word != NULL) free(word);
		free(search_term);
		DestroyFound(&Founds);
		destroy_pars_path(&parsed_path);
		destroy_pars_path(&parsed_path_loc);		
		return Founds_Loc;
	}
}


//find_t *locator(find_t *Founds, path_t *parsed_path, const char *path_loc, opts_t *Popt)
find_t *locator(find_t *Founds, path_t *parsed_path, path_t *parsed_path_loc, opts_t *Popt)
{
/*
 * function looks for subset in nodel List
 * and specified by path and location
 * before this function is invoked, the initial Founds subset must be
 * created by calling Founds function
 */	
	node_t *Tmp, *Tmppar, *Tm_prev;
	size_t *HelpNodeI;
	size_t i, j, k, counter;
	path_t *parsed_path_founds;
	get_arg_t argsstr;
	find_t *RetFound;
	size_t tot_match, len1, len2;
	char *node_path;
/*
 * parse path location specification; IMP: do not forget destroy_pars_path(&parsed_path) once not needed
 */
//  	if ( (parsed_path_loc = parse_path(path_loc)) == NULL){
// 		Error("Path - failed");
// 		return (find_t *)NULL;
// 	}
// 
// 	if(parsed_path->seg_count != parsed_path_loc->seg_count){
// 		destroy_pars_path(&parsed_path_loc);
// 		Error("Number of items in path different from location specification");  /* NOTE - in later versions, ust one symbol '*' can be used for all paths segments */
// 	}
/*
 * allocate field for positive match and for segments of path for each element of Found
 */
	printf(" Here \n");

	if ( (HelpNodeI = malloc(Founds->founds * sizeof(size_t))) == NULL)
		Perror("malloc");

	for(i=0; i < Founds->founds; i++)
  		HelpNodeI[i] = 1;
	
/*	if ( (parsed_path_founds = (path_t **)malloc(Founds->founds * sizeof(path_t **))) == NULL)
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
		for (j=0; j< parsed_path_founds[i]->seg_count; j++)
			printf("-%s-", parsed_path_founds[i]->path[j]);
		printf("\n");
		free(node_path);
	}

	for(i=0; i < Founds->founds; i++) destroy_pars_path(&parsed_path_founds[i]);
	free(parsed_path_founds);
	free(HelpNodeI);
	destroy_pars_path(&parsed_path_loc);
	return NULL;*/
/*
 * loop over all levels in path, segment by segment and determine
 * if match is positive or negative
 * - save parent and current node pointer in temporary values - used for couter
 * - set counter 1
 */
	Tmppar  = Founds->Found_Nodes[0]->List->parent;
	Tm_prev = Founds->Found_Nodes[0]->List;
	counter = 1;

	for(i=0; i<parsed_path_loc->seg_count; i++){
/*
 * get arguments for path segment
 */
//  		printf(" Segment_loc %d is %s\n", i, parsed_path_loc->path[i]);
		argsstr = get_arguments(parsed_path_loc->path[i]);

		if(argsstr.retval == -1){
			Error("argstr error");
			destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
// 			for(j=0; j <Founds->founds; j++)
// 				destroy_pars_path(&parsed_path_founds[j]);
// 			free(parsed_path_founds);
			return (find_t *)NULL;
		}
/*
 * loop over founds and check for match, If HelpNodeI == 0, the node is already marked as negative match
 */
		for(j = 0; j< Founds->founds; j++){

			if( HelpNodeI[j] == 1){

				node_path = Path(Founds->Found_Nodes[j]->List, Founds->Home_Node);
				parsed_path_founds =  parse_path(node_path);

// 				printf(" Trying to find a match %d %d \n", i,j);
/*
 * node is considered as possible match
 */
/*
 * compare i-th segment of the path, if match, go with tests of location, Test of length equality too
 */				
				len1 = strlen(parsed_path->path[i]);
				len2 = strlen(parsed_path_founds->path[i]);
				
// 				printf(" parsed_path_founds %ld   %s \n", len2, parsed_path_founds->path[i]);
				
				if(len1 == len2 && strncmp(parsed_path->path[i], parsed_path_founds->path[i], len1) == 0){
/*
 * segments are equal, check locator
 */
					Tmp = Founds->Found_Nodes[j]->List;
/*
 * find node_t pointer corresponding to path segment
 */
					for(k=i+1; k<parsed_path_loc->seg_count; k++)
						Tmp = Tmp->parent;
/*
 * get counter, increment for each in the same DIR, set 0 if different DIR
 */
// 					printf("%s %p  %p \n", Tmp->name , Tmp->parent, Tmppar);
/*
 * if parent of the node is the same as previuous node parent..
 */
					if(Tmppar == Tmp->parent){
 /*
  * check if the node is the same as pevious or not, if not, increase counter (the same name, different pointer situation)
  */
						if (Tmp != Tm_prev)counter++;
					}
					else{
						counter = 1;
						Tmppar = Tmp->parent;
						Tm_prev = Tmp;
					}

//     					printf("Counter is %d\n", counter);

 					HelpNodeI[j]  = match_test(Tmp,argsstr, counter);
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
				free(node_path);
				destroy_pars_path(&parsed_path_founds);
			}
		}
	}
// 	printf(" after cycle \n");
/*
 * free parsed_path 
 */	
// 	destroy_pars_path(&parsed_path_loc);
/*
 * count how many matches are positive
 */
	tot_match = 0;
	for(j = 0; j< Founds->founds; j++){
		if(HelpNodeI[j] == 1) tot_match++;
	}
//  	printf(" number of positive matches is %d\n", tot_match);

	if(tot_match == 0){
/*
 * not any positive match
 */
		free(HelpNodeI);
// 		for(j=0; j <Founds->founds; j++)
// 			destroy_pars_path(&parsed_path_founds[j]);
// 		free(parsed_path_founds);
		return (find_t *)NULL;
	}
	else{
		
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
		
		counter = 0;
		for(j = 0; j< Founds->founds; j++){
			if( HelpNodeI[j] == 1){
	 			printf(" List found in match is %p  %s\n", Founds->Found_Nodes[j]->List, Founds->Found_Nodes[j]->List->name);
				RetFound->Found_Nodes[counter++]->List=Founds->Found_Nodes[j]->List;}
		}
/*
 * save the number of positive founds
 */
		RetFound->founds = tot_match;
/*
 * free memory
 */
		free(HelpNodeI);
// 		for(j=0; j <Founds->founds; j++)
// 			destroy_pars_path(&parsed_path_founds[j]);
// 		free(parsed_path_founds);
/*
 * return list of positive mathes
 */
	printf(" Here - returning \n");

		return RetFound;
	}
}



int match_test(node_t *List, get_arg_t argsstr, size_t counter)
{
	char c;
	c = (int)argsstr.arg;
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
		switch ( c ){
			
			case '*':
				return 1;
			break;
	
			case 'V':  /* Value */
// isgreaterequal, isgreater				
			break;
		
		}
		
		return 0;
		
	}
	
	return 0;
	
}