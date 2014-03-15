/*
 *     Copyright (C) 2012  Adam Jirasek
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU Lesser General Public License for more details.
 * 
 *     You should have received a copy of the GNU Lesser General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *     
 *     contact: libm3l@gmail.com
 * 
 */



/*
 *     Function locate_list.c
 *
 *     Date: 2012-06-24
 * 
 * 
 *
 *
 *     Description:
 * 
 *
 *     Input parameters:
 * 
 *
 *     Return value:
 * 
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 *
 */





#include "libm3l_header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "locate_list.h"
#include "FunctionsPrt.h"
#include "find_list.h"

static lmint_t m3l_match_test(node_t *, get_arg_t, lmsize_t);
static lmint_t m3l_match_single_test(node_t *, get_arg_t, lmsize_t);
static find_t *m3l_locator(find_t *, path_t *, path_t *, opts_t *);

extern lmint_t optind;
static lmint_t verbose_flag;

/*
 * function uses Find_list to find all possible items with given name
 * and then filters the subset using additional info (location etc.)
 */

find_t *m3l_locator_caller(node_t *List, const lmchar_t *path, const lmchar_t *path_loc, opts_t *Popts)
{
	path_t *parsed_path, *parsed_path_loc;
	lmchar_t *search_term;
	lmsize_t i;
	find_t *Founds, *Founds_Loc;
	node_t *Tmp_node;
/*
 * parse path
 */
	if( (parsed_path = m3l_parse_path(path)) == NULL){
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
			Warning("Wrong absolute path");
 			m3l_destroy_pars_path(&parsed_path);
			return (find_t *)NULL;
		}
	}
	else{

		for(i=0; i<parsed_path->seg_count; i++){
			if(strncmp(parsed_path->path[i], "..", 2) == 0){
				if ( (Tmp_node = Tmp_node->parent) == NULL){
					Warning("Wrong path");
					m3l_destroy_pars_path(&parsed_path);
					return (find_t *)NULL;
				}
			}
		}
	}

	if( strncmp(Tmp_node->type, "DIR", 3) != 0){
		Warning("List in locate is not DIR");
		m3l_destroy_pars_path(&parsed_path);
		return (find_t *)NULL;
	}
/* 
 * this function returns back found_t **pointer which has "founds" number of items
 * do not forget to free it when you do not need it
 */
	if ( (search_term = strdup(parsed_path->path[parsed_path->seg_count-1])) == NULL)
			Perror("strdup");
	if(Popts != NULL && Popts->opt_i == 'i')search_term = StrToLower(search_term);

	if ( (Founds = m3l_Find_caller(2, Tmp_node, search_term, Popts)) == NULL){
		free(search_term);
		m3l_destroy_pars_path(&parsed_path);
//  		Warning("Locator: No Founds");
		return (find_t *)NULL;
	}
	else
	{
/*
 * write the values of the find result
  * call locator to select sets
 */		
		if( (parsed_path_loc = m3l_parse_path(path_loc)) == NULL){
			free(search_term);
			m3l_destroy_pars_path(&parsed_path);
			Error("Path2 failed");
			return (find_t *)NULL;
		}
		
		if(parsed_path->seg_count != parsed_path_loc->seg_count){
			m3l_destroy_pars_path(&parsed_path_loc);
			free(search_term);
			m3l_destroy_pars_path(&parsed_path);
			Error("Number of items in path different from location specification");  /* NOTE - in later versions, use one symbol '*' can be used for all paths segments */
			return (find_t *)NULL;
		}
	
 		Founds_Loc = m3l_locator(Founds, parsed_path, parsed_path_loc, Popts);
	
		free(search_term);
		m3l_DestroyFound(&Founds);
		m3l_destroy_pars_path(&parsed_path);
		m3l_destroy_pars_path(&parsed_path_loc);		
		return Founds_Loc;
	}
}


find_t *m3l_locator(find_t *Founds, path_t *parsed_path, path_t *parsed_path_loc, opts_t *Popt)
{
/*
 * function looks for subset in nodel List
 * and specified by path and location
 * before this function is invoked, the initial Founds subset must be
 * created by calling Founds function
 */	
	node_t *Tmp, *Tmppar, *Tm_prev;
	lmsize_t *HelpNodeI;
	lmsize_t i, j, k, counter;
	path_t **parsed_path_Ffounds;
	get_arg_t argsstr;
	find_t *RetFound;
	lmsize_t tot_match, len1, len2;
	lmchar_t *node_path;
/*
 * allocate field for positive match and for segments of path for each element of Found
 */
	if ( (HelpNodeI = malloc(Founds->founds * sizeof(lmsize_t))) == NULL)
		Perror("malloc");
/*
 * NOTE improve efficiency by avoiding testing elements with path different from lookup path
 */
	if ( (parsed_path_Ffounds = (path_t **)malloc(Founds->founds * sizeof(path_t *))) == NULL)
		Perror("malloc");
	
	for(i=0; i < Founds->founds; i++){
/*
 * define path of the found node i
 */		
		if( (node_path = m3l_Path(Founds->Found_Nodes[i]->List, Founds->Home_Node)) == NULL){
			Error(" Path error");
			m3l_destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
			for(j=0; j <=i; j++)
				m3l_destroy_pars_path(&parsed_path_Ffounds[j]);
			free(parsed_path_Ffounds);
			return (find_t *)NULL;
		}
/*
 * parse the path
 */		
		if( (parsed_path_Ffounds[i] =  m3l_parse_path(node_path)) == NULL){
			Error(" Path error");
			m3l_destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
			for(j=0; j <=i; j++)
				m3l_destroy_pars_path(&parsed_path_Ffounds[j]);
			free(parsed_path_Ffounds);
			return (find_t *)NULL;
		}
/*
 * compare number of segments of path to lookup path
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
		argsstr = m3l_get_arguments(parsed_path_loc->path[i]);
		
		if(argsstr.retval == -1){
			Error("argstr error");
			m3l_destroy_pars_path(&parsed_path_loc);
			free(HelpNodeI);
			free(node_path);
			for(j=0; j <Founds->founds; j++)
				m3l_destroy_pars_path(&parsed_path_Ffounds[j]);
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
					
						HelpNodeI[j]  = m3l_match_test(Tmp,argsstr, counter);
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
			m3l_destroy_pars_path(&parsed_path_Ffounds[j]);
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
			m3l_destroy_pars_path(&parsed_path_Ffounds[j]);
		free(parsed_path_Ffounds);
/*
 * return list of positive mathes
 */
		return RetFound;
	}
}

lmint_t m3l_match_test(node_t *List, get_arg_t argsstr, lmsize_t counter)
{
/*
 * find if what is to be comapred is set or subset
 */
	node_t *Tmpnode;
	lmint_t retval;
	lmsize_t len1, len2;

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
/*
 * check that the name of the node is 
 * equal to required name of node
 */
				len1 = strlen(Tmpnode->name);
				len2 = strlen(argsstr.s_name);
				
				if(len1 == len2 && strncmp(Tmpnode->name, argsstr.s_name, len1) == 0 ){
					if( (retval = m3l_match_single_test(Tmpnode,  argsstr, counter)) == 1) 
						return 1;
				}
				Tmpnode = Tmpnode->next;
			}
			return 0;
		}
	}
	else{
/*
 * set
 */
		retval =  m3l_match_single_test(List, argsstr, counter);
		return retval;
	}
	
	return 0;
}

lmint_t m3l_match_single_test(node_t *List, get_arg_t argsstr, lmsize_t counter)
{
	lmchar_t c;
	lmsize_t len1, len2;
	c = (lmint_t)argsstr.arg;
	
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
	return 0;
}
