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
 *     Function find_list.c
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

#include "find_list.h"
#include "FunctionsPrt.h"

static lmsize_t m3l_FindList(lmint_t, node_t *, lmchar_t *, lmsize_t *, find_t *, opts_t *);
static lmint_t m3l_AddRecord(node_t *, lmsize_t *, find_t *);
static lmusignchar_t m3l_CompStatement(lmchar_t *, lmchar_t *, lmchar_t *, opts_t *);
static lmusignchar_t m3l_EvalSearchPatt(lmchar_t *, lmchar_t *, opts_t *);


// lmsize_t nalloc;
// find_t *Founds;
/*
 * this function is similar to FindListPointer 
 * with an exception that all options have to be specified
 * the reason for doing so is the **Found_Nodes and nalloc variables
 * decalred above
 */

find_t *m3l_find(lmint_t call, node_t *List, lmchar_t *search_term, opts_t *Popt)
{
/*
 * allocate find_t pointer and first element
 * if no founds, free it
 */
        lmsize_t nalloc;
	find_t *Founds;

	nalloc = 0;

	if ( (Founds = (find_t *)malloc( sizeof(find_t))) == NULL)
		Perror("malloc");
	
	if ( (Founds->Found_Nodes = (find_str_t **)malloc( sizeof(find_str_t *))) == NULL)
		Perror("malloc");
	
	if ( ( Founds->Found_Nodes[0] =(find_str_t *)malloc( sizeof(find_str_t))) == NULL)
		Perror("malloc");
	
	Founds->founds = 0;
	Founds->founds = m3l_FindList(call, List, search_term, &nalloc, Founds, Popt);
	
	if ( Founds->founds == 0){
		free(Founds->Found_Nodes[0]);
		free(Founds->Found_Nodes);
		free(Founds); 
		Founds = NULL;
		return (find_t *)NULL;
	}
/*
 * save the root node of the search
 */
	Founds->Home_Node = List;
	
	return Founds;
}

/*
 * function is a pointer to pointer type 
 */
lmsize_t m3l_FindList(lmint_t call, node_t *List, lmchar_t *search_term, lmsize_t *nalloc, find_t *Founds, opts_t *Popt)
{
/*
 * function looks for items with given pattern and option
 * upon calling, function passes a parameter int call which is an indicater if the function call is 
 * initial (1) or recursive ( != 1). If 1, the name of the node is ignored and not added to the list
 * This node is considered primary parent DIR
 *
 * if called with recursive option, it traveres entire tree, otherwise it lists the current node children only
 */
	node_t *Tmpnode;
	
	if(List == NULL){
/*
 * Node is FILE type, loop over next nodes
 */
		Warning("WriteData: NULL list");
			return -1;
	}
	 
	if(Popt != NULL && Popt->opt_r == 'r'){
/*
 * List entire tree
 */ 
		if(List->child == NULL){
/*
 * loop over next nodes
 */
			Tmpnode = List;
			if ( m3l_CompStatement(search_term, Tmpnode->name, Tmpnode->type, Popt) == '1'){
				if ( m3l_AddRecord(Tmpnode, nalloc,Founds) != 1)
				Warning("Error adding record to the list");
			}
		}
		else
		{
/*
 * List is DIR type, call FindList recursively
 */
			if( call != 1){
/*
 * not initial call, ie. do not list the main parent node, you want to list what is in it
 */
				if ( m3l_CompStatement(search_term, List->name, List->type, Popt) == '1'){
					if ( m3l_AddRecord(List, nalloc,Founds) != 1)
						Warning("Error adding record to the list");	
				}
			}
/*
 * recursive calling, go to the first child node and loop over list in the same level as child
 */
			Tmpnode = List->child;
			while(Tmpnode != NULL){
				if( strncmp(Tmpnode->type, "LINK", 4 ) == 0  && Popt->opt_L == 'L'){
					m3l_FindList(2, Tmpnode->child, search_term, nalloc, Founds,Popt);
				}
				else{
					m3l_FindList(2, Tmpnode, search_term, nalloc, Founds,Popt);
				}
				Tmpnode = Tmpnode->next;
			}
		}
		return *nalloc;
	}
	else
	{
/*
 * list only children in this node
 * start with listing parent node
 */
		if( call != 1){
/*
 * if initial call, do not list the main parent node, you want to list what is in it
 */
			if ( m3l_CompStatement(search_term, List->name, List->type, Popt) == '1'){
				if ( m3l_AddRecord(List, nalloc,Founds) != 1)
					Warning("Error adding record to the list");	
			}
		}
/*
 * if this node has children go to their level and list
 */

		if(List->child != NULL){
			Tmpnode = List->child ;
			while(Tmpnode != NULL){
/*
 * List first one
 */
				if ( m3l_CompStatement(search_term, Tmpnode->name, Tmpnode->type, Popt) == '1'){
					if ( m3l_AddRecord(Tmpnode, nalloc,Founds) != 1)
						Warning("Error adding record to the list");
				}
/*
 * go to next 
 */
				Tmpnode = Tmpnode->next;
			}
		}
	return *nalloc;
	}
}


lmint_t m3l_AddRecord(node_t *Tmpnode, lmsize_t *nalloc, find_t *Founds)
{
	find_str_t **realloc_find;
/*
 * function adds new record (*Tmpnode) to list of found records - Found_Nodes
 * If Found_Nodes does not contain any data set (nalloc = 0) use malloc
 * otherwise realloc 
 * Upon successful return 1
 */
	if(*nalloc == 0){
/*
 * first hit
 * Allocate first *Found_Nodes and store address of found node in it. 
 * Increase counter of found nodes by 1
 */
// 		if ( ( Founds->Found_Nodes[0] =(find_str_t *) malloc( sizeof(find_str_t))) == NULL)
// 			Perror("malloc");
		Founds->Found_Nodes[0]->List = Tmpnode;
			(*nalloc)++;
	}	
	else
/*
 * adding additional hit to already existing set
 * Reallocate Found_Nodes, and store address of found node in last member of Found_Nodes   (*(Found_Nodes+(nalloc-1)) = Found_Nodes[nalloc-1])
 * Increase counter of found nodes by 1
 */
	{
		(*nalloc)++;
		realloc_find = NULL;
		if ( (realloc_find =(find_str_t **)realloc(Founds->Found_Nodes, *nalloc * sizeof(find_str_t *))) == NULL){
			Perror("realloc");
		}
		else{
			Founds->Found_Nodes = realloc_find;
		}
		
		if ( (Founds->Found_Nodes[*nalloc-1] =(find_str_t *)malloc( sizeof(find_str_t))) == NULL)
			Perror("malloc");
		Founds->Found_Nodes[*nalloc-1]->List = Tmpnode;
	}
	return 1;
}

/*
 * compares statements with options
 */

lmusignchar_t m3l_CompStatement(lmchar_t *search_term, lmchar_t *pattern, lmchar_t *type, opts_t *Popts)
{
	if ( m3l_EvalSearchPatt(search_term, pattern, Popts) == '1'){
/*
 * patterns are identical
 * check if additional otpions are required
 */
/*
 * DIR types only
 */
		if(Popts != NULL && Popts->opt_d == 'd'){
			if( strncmp("DIR",type, 3) == 0 ){
				return '1';
			}
			else
			{
				return '0';
			}
		}
/*
 * LINKS types only
 */
		else if (Popts != NULL && Popts->opt_l == 'l'){
			if( strncmp("LINK",type, 4) == 0 ){
				return '0';
			}
			else
			{
				return '1';
			}
		}
/*
 * FILE types only
 */
		else if (Popts != NULL && Popts->opt_f == 'f'){
			if( strncmp("DIR",type, 3) == 0 ){
				return '0';
			}
			else
			{
				return '1';
			}
		}
/*
 * no additional options specified
 */
	return '1';	
	}
	
	return '0';
}

lmusignchar_t  m3l_EvalSearchPatt(lmchar_t *search_term, lmchar_t *pattern, opts_t *Popts)
{

	lmchar_t *Ppattern;
	lmsize_t len1, len2;
/*
 * duplicate pattern and of required by option convert upper case to lower case
 */
	if ( (Ppattern = strdup(pattern)) == NULL)
		Perror("strdup");

	if(Popts != NULL && Popts->opt_i == 'i') 
	       Ppattern = StrToLower(Ppattern);
	
	len1 = strlen(Ppattern);
	len2 = strlen(search_term);
/*
 * find if search term metches patter and return 0
 * if IGNORE (I) specified, reverese retrun value
 * upon leaving, free *Ppattern 
 */
	if( (strncmp(search_term, Ppattern, strlen(Ppattern)) == 0 && len1 == len2)   ||  *search_term == '*'){
		if(Popts != NULL && Popts->opt_I == 'I'){
			free(Ppattern);
			return '0';
		}
		else
		{
			free(Ppattern);
			return '1';
		}
	}
	else
	{
		if(Popts != NULL && Popts->opt_I == 'I'){
			free(Ppattern);
			return '1';
		}
		else
		{
			free(Ppattern);
			return '0';
		}	
	}
}





void m3l_DestroyFound(find_t **Founds)
{
/*
 * function destroys filed allocted by function find
 */
	lmsize_t i;
	if(*Founds == NULL)return ;
			
	for(i=0; i< (*Founds)->founds; i++){
		free( (*Founds)->Found_Nodes[i] );
//		FoundNodes[i]=NULL;
	}
	
	free( (*Founds)->Found_Nodes);	
	(*Founds)->Found_Nodes = NULL;
	(*Founds)->Home_Node = NULL; 
	free((*Founds));	
	(*Founds) = NULL; 
	
	return;
}	
