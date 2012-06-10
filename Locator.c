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
static int match_single_test(node_t *, get_arg_t, size_t);
static find_t *locator(find_t *, path_t *, path_t *, opts_t *);

extern int optind;
static int verbose_flag;

find_t *locator_caller(node_t *List, const char *path, const char *path_loc, opts_t *Popts)
{
	path_t *parsed_path, *parsed_path_loc;
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
/*
 * call find function with specified options
 * First look if ../ are in path or if path is absolute path
 * set initial node, if path contains ../ go to higher lever
 */
	Tmp_node = List;

	if(parsed_path->abspath == 'A'){
		
		while(Tmp_node->parent != NULL)Tmp_node = Tmp_node->parent;
/*
 * check if first segment is identical to name of initial node or is '*' or ~
 */
		if(strncmp(Tmp_node->name, parsed_path->path[0], strlen(Tmp_node->name)) != 0 && 
	           strncmp(parsed_path->path[0], "*", 1) != 0 && strncmp(parsed_path->path[0], "~", 1) != 0){
			Error("Wrong absolute path");
			destroy_pars_path(&parsed_path);
			return (find_t *)NULL;
		}
	}
	else{

		for(i=0; i<parsed_path->seg_count; i++){
			if(strncmp(parsed_path->path[i], "..", 2) == 0){
				if ( (Tmp_node = Tmp_node->parent) == NULL){
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

	if ( (Founds = Find_caller(2, Tmp_node, search_term, Popts)) == NULL){
		free(search_term);
		destroy_pars_path(&parsed_path);
		Warning("Locator: No Founds");
		return (find_t *)NULL;
	}
	else
	{
/*
 * write the values of the find result
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
			Error("Number of items in path different from location specification");  /* NOTE - in later versions, use one symbol '*' can be used for all paths segments */
			return (find_t *)NULL;
		}
			
 		Founds_Loc = locator(Founds, parsed_path, parsed_path_loc, Popts);
	
		free(search_term);
		DestroyFound(&Founds);
		destroy_pars_path(&parsed_path);
		destroy_pars_path(&parsed_path_loc);		
		return Founds_Loc;
	}
}


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
	path_t **parsed_path_Ffounds;
	get_arg_t argsstr;
	find_t *RetFound;
	size_t tot_match, len1, len2;
	char *node_path;
/*
 * allocate field for positive match and for segments of path for each element of Found
 */
	if ( (HelpNodeI = malloc(Founds->founds * sizeof(size_t))) == NULL)
		Perror("malloc");
/*
 * NOTE improve efficiency by avoiding testing elements with path different from lookup path
 */
	if ( (parsed_path_Ffounds = (path_t **)malloc(Founds->founds * sizeof(path_t *))) == NULL)
		Perror("malloc");
	
	for(i=0; i < Founds->founds; i++){
  		
		if( (node_path = Path(Founds->Found_Nodes[i]->List, Founds->Home_Node)) == NULL){
			Error(" Path error");
			destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
			for(j=0; j <=i; j++)
				destroy_pars_path(&parsed_path_Ffounds[j]);
			free(parsed_path_Ffounds);
			return (find_t *)NULL;
		}
		if( (parsed_path_Ffounds[i] =  parse_path(node_path)) == NULL){
			Error(" Path error");
			destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
			for(j=0; j <=i; j++)
				destroy_pars_path(&parsed_path_Ffounds[j]);
			free(parsed_path_Ffounds);
			return (find_t *)NULL;
		}
/*
 * compare number of segments of path to loopup path
 * if different, exclude element from further testing
 */
		if(parsed_path_Ffounds[i]->seg_count == parsed_path_loc->seg_count){
			HelpNodeI[i] = 1;
		}
		else
		{
			HelpNodeI[i] = 0;
		}
		free(node_path);
	}
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
		argsstr = get_arguments(parsed_path_loc->path[i]);
		
		if(argsstr.retval == -1){
			Error("argstr error");
			destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
			for(j=0; j <Founds->founds; j++)
				destroy_pars_path(&parsed_path_Ffounds[j]);
			free(parsed_path_Ffounds);
			return (find_t *)NULL;
		}
/*
 * loop over founds and check for match, If HelpNodeI == 0, the node is already marked as negative match
 */
		for(j = 0; j< Founds->founds; j++){

			if( HelpNodeI[j] == 1){
/*
 * node is considered as possible match
 */
/*
 * if part of path is '*' look for all matches
 * if number of path (required and compared to) are not equal, exclude node
 * othewise keep index unchanged
 */
 				if( strncmp(parsed_path->path[i], "*", 1) == 0){
					if(parsed_path_Ffounds[j]->seg_count != parsed_path->seg_count) HelpNodeI[j] = 0;
				}
				else{
/*
 * compare i-th segment of the path, if match, go with tests of location, Test of length equality too
 * check for special cases when path starts with ./ or ~/
 */	
					len1 = strlen(parsed_path->path[i]);
					len2 = strlen(parsed_path_Ffounds[j]->path[i]);
					
					if( (len1 == len2 && strncmp(parsed_path->path[i], parsed_path_Ffounds[j]->path[i], len1) == 0) || 
					    (strncmp(parsed_path->path[i], ".", 1) == 0 && len1 == 1 && i == 0) ||
					    (strncmp(parsed_path->path[i], "~", 1) == 0	&& i == 0)){
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
 * if parent of the node is the same as previuous node parent..
 */
						if(Tmp != NULL){
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
						}
					
// 					printf(" Argument to comapre are '%s' \n",argsstr.args); 
// 					printf(" Name Argument to comapre are '%c' \n",argsstr.arg); 

					
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
				}
			}
		}
	}
/*
 * count how many matches are positive
 */
	tot_match = 0;
	for(j = 0; j< Founds->founds; j++){
		if(HelpNodeI[j] == 1) tot_match++;
	}

	if(tot_match == 0){
/*
 * not any positive match
 */
		free(HelpNodeI);
		for(j=0; j <Founds->founds; j++)
			destroy_pars_path(&parsed_path_Ffounds[j]);
		free(parsed_path_Ffounds);
		return (find_t *)NULL;
	}
	else{
		
/*
 * allocate find_t structure and fill it
 */
		if ( (RetFound = (find_t *) malloc( sizeof(find_t))) == NULL)
			Perror("malloc");
		
		if ( (RetFound->Found_Nodes = (find_str_t **) malloc( tot_match * sizeof(find_str_t *))) == NULL)
			Perror("malloc");
				
		for(j = 0; j< tot_match; j++){
			if ( ( RetFound->Found_Nodes[j] =(find_str_t *) malloc( sizeof(find_str_t))) == NULL)
				Perror("malloc");
		}
		
		counter = 0;
		for(j = 0; j< Founds->founds; j++){
			if( HelpNodeI[j] == 1){
				RetFound->Found_Nodes[counter++]->List=Founds->Found_Nodes[j]->List;
			}
		}
/*
 * save the number of positive founds
 */
		RetFound->founds = tot_match;
/*
 * free memory
 */
		free(HelpNodeI);
		for(j=0; j <Founds->founds; j++)
			destroy_pars_path(&parsed_path_Ffounds[j]);
		free(parsed_path_Ffounds);
/*
 * return list of positive mathes
 */
		return RetFound;
	}
}

int match_test(node_t *List, get_arg_t argsstr, size_t counter)
{
/*
 * find if what is to be comapred is set or subset
 */
	node_t *Tmpnode;
	int retval;

	if( argsstr.first == 'S' || argsstr.first == 's' ){
/*
 * locator specifies subset, loop over subsets until first positive match
 */
		if ( (Tmpnode = List->child) == NULL){
			Error("Wrong location - node is FILE type");
			return 0;
		}
		else{
			while(Tmpnode != NULL){
				if( (retval = match_single_test(Tmpnode,  argsstr, counter)) == 1) return 1;
				Tmpnode = Tmpnode->next;
			}
			return 0;
		}
	}
	else{
/*
 * set
 */
		retval =  match_single_test(List, argsstr, counter);
		return retval;
	}
	
	return 0;
}

int match_single_test(node_t *List, get_arg_t argsstr, size_t counter)
{
	char c;
	size_t len1, len2;
	c = (int)argsstr.arg;

	switch ( c ){
			
		case '*':
			return 1;
		break;
	
		case 'V':  /* Value */
/*
 * get type of argument
 */
			if(strncmp(List->type,"C",1) == 0){
				len1 = strlen(List->data.c);
				len2 = strlen(argsstr.args);
				if(len1 == len2 && strncmp(List->data.c, argsstr.args, len1) == 0 ){
					return 1;
				}
				else{
					return 0;
				}
			}
			else{
// 				Warning("Only chars can be used");
				return 0;
			}
		break;
			
		case 'N':  /* Name */
			len1 = strlen(List->name);
			len2 = strlen(argsstr.args);
			if(len1 == len2 &&  strncmp(List->name, argsstr.args, len1) == 0){
				return 1;
			}
			else{
				return 0;
			}
		break;

		case 't':  /* type */
			len1 = strlen(List->type);
			len2 = strlen(argsstr.args);
			if(len1 == len2 &&  strncmp(List->name, argsstr.args, len1) == 0){
				return 1;
			}
			else{
				return 0;
			}
		break;

		case 'n':  /* count */
			len1 = Strol(argsstr.args);
			if( counter == len1){
				return 1;
			}
			else{
				return 0;
			}
		break;
	}
}
