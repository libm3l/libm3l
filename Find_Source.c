/*
 * function frees memory of the node 
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Find_Source.h"
#include "FunctionsPrt.h"

static SIZE_T FindList(int, node_t *, char *, opts_t *);
static AddRecord(node_t *);
static unsigned char CompStatement(char *, char *, char *, opts_t *);
static unsigned char EvalSearchPatt(char *, char *, opts_t *);


size_t nalloc;
find_t **Found_Nodes;
/*
 * this function is similar to FindListPointer 
 * with an exception that all options have to be specified
 * the reason for doing so is the **Found_Nodes and nalloc variables
 * decalred above
 */

find_t **Find_caller(node_t *List, size_t *founds, char *search_term, opts_t *Popt)
{
/*
 * allocate Found_Nodes ** pointer
 */
	nalloc = 0;

	if ( (Found_Nodes = (find_t **) malloc( sizeof(find_t **))) == NULL)
		Perror("malloc");
	
	*founds = 0;
	*founds = FindList(1, List, search_term, Popt);
	
	if ( *founds == 0){
		free(Found_Nodes); Found_Nodes = NULL;
		return (find_t **)NULL;
	}
	
	return Found_Nodes;
}

/*
 * function is a pointer to pointer type 
 */
SIZE_T FindList(int call, node_t *List, char *search_term, opts_t *Popt)
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
 
	if(Popt->opt_r == 'r'){
/*
 * List entire tree
 */
		if(List == NULL){
/*
 * Node is FILE type, loop over next nodes
 */
			Warning("WriteData: NULL list");
			return -1;
		}
 
		if(List->child == NULL){
/*
 * loop over next nodes
 */
		Tmpnode = List;

			if(Tmpnode != NULL){
				while(Tmpnode != NULL){
					if(Tmpnode->child != NULL){
/*
 * recursive calling, search this DIR too
 */					
 						FindList(2, Tmpnode, search_term, Popt);
/*
 * go to next 
 */
						Tmpnode = Tmpnode->next;
					}
					else
					{
						if ( CompStatement(search_term, Tmpnode->name, Tmpnode->type, Popt) == '1'){
							if ( AddRecord(Tmpnode) != 1)
								Warning("Error adding record to the list");
						}
/*
 * go to next 
 */
						Tmpnode = Tmpnode->next;
					}
				}
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
				if ( CompStatement(search_term, List->name, List->type, Popt) == '1'){
					if ( AddRecord(List) != 1)
						Warning("Error adding record to the list");	
				}
			}
/*
 * recursive calling, go to the first chld node and loop over list in the same level as child
 */
			Tmpnode = List->child;
			FindList(2, Tmpnode, search_term, Popt);
		}
	return nalloc;
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
			if ( CompStatement(search_term, List->name, List->type, Popt) == '1'){
				if ( AddRecord(List) != 1)
					Warning("Error adding record to the list");	
			}
		}
/*
 * if this node has chidlren go to their level and list
 */

		if(List->child != NULL){
			Tmpnode = List->child ;
			while(Tmpnode != NULL){
/*
 * List first one
 */
				if ( CompStatement(search_term, Tmpnode->name, Tmpnode->type, Popt) == '1'){
					if ( AddRecord(Tmpnode) != 1)
						Warning("Error adding record to the list");
				}
/*
 * go to next 
 */
				Tmpnode = Tmpnode->next;
			}
		}
	return nalloc;
	}
}


int AddRecord(node_t *Tmpnode)
{
/*
 * function adds new record (*Tmpnode) to list of found records - Found_Nodes
 * If Found_Nodes does not contain any data set (nalloc = 0) use malloc
 * otherwise realloc 
 * Upon successful return 1
 */
	if(nalloc == 0){
/*
 * first hit
 * Allocate first *Found_Nodes and store address of found node in it. 
 * Increase counter of found nodes by 1
 */
		if ( ( Found_Nodes[0] =(find_t *) malloc( sizeof(find_t *))) == NULL)
			Perror("malloc");
		Found_Nodes[0]->List = Tmpnode;
			nalloc++;
	}	
	else
/*
 * adding additional hit to already existing set
 * Reallocate Found_Nodes, and store address of found node in last member of Found_Nodes   (*(Found_Nodes+(nalloc-1)) = Found_Nodes[nalloc-1])
 * Increase counter of found nodes by 1
 */
	{
		nalloc++;
//						printf("reallocating %ld\n", nalloc);
		if ( (Found_Nodes =(find_t **) realloc(Found_Nodes, nalloc * sizeof(find_t **))) == NULL)
			Perror("realloc");
		if ( (Found_Nodes[nalloc-1] =(find_t *) malloc( sizeof(find_t *))) == NULL)
			Perror("malloc");
		Found_Nodes[nalloc-1]->List = Tmpnode;
	}
	return 1;
}

void DestroyFound(find_t **FoundNodes, size_t founds)
{
/*
 * function destroys filed allocted by function Find_caller
 */
	size_t i;
	
	for(i=0; i<founds; i++)
	{
		free(FoundNodes[i]);
//		FoundNodes[i]=NULL;
	}
	
	free(FoundNodes);	
	FoundNodes = NULL; 
}	


/*
 * compares statements with options
 */

unsigned char CompStatement(char *search_term, char *pattern, char *type, opts_t *Popts)
{
	if ( EvalSearchPatt(search_term, pattern, Popts) == '1'){
/*
 * patterns are identical
 * check if additional otpions are required
 */
/*
 * DIR types only
 */
		if(Popts->opt_d == 'd'){
			if( strncmp("DIR",type, 3) == 0 ){
				return '1';
			}
			else
			{
				return '0';
			}
		}
/*
 * FILE types only
 */
		else if (Popts->opt_f == 'f'){
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

unsigned char  EvalSearchPatt(char *search_term, char *pattern, opts_t *Popts)
{

	char *Ppattern;
/*
 * duplicate pattern and of required by option convert upper case to lower case
 */
	if ( (Ppattern = strdup(pattern)) == NULL)
		Perror("strdup");

	if(Popts->opt_i == 'i') 
	       Ppattern = StrToLower(Ppattern);
/*
 * find if search term metches patter and return 0
 * if IGNORE (I) specified, reverese retrun value
 * upon leaving, free *Ppattern 
 */
	if( strncmp(search_term, Ppattern, strlen(Ppattern)) == 0   ||  *search_term == '*'){
		if(Popts->opt_I == 'I'){
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
		if(Popts->opt_I == 'I'){
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