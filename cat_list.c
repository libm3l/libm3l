/*
 * function frees memory of the node 
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "cat_list.h"


extern void Warning(const char *);
static void PrintListInfo(node_t *, opts_t *);

/*
 * routine writes linked list structure
 */

int cat_list(int call, node_t *List, opts_t *Popts)
{
/*
 * functions print the list or entire tree on screen
 *  - if the specified node is DIR type, it prints all files in that directory
 *  - if specified node is FILE type, it prints only list and its content
 * 
 * upon first call, the value of int call should be set to 1
 *
 * options specifies how much info is going to be print out
 *
 * Upon return function returns:
 *    1 - if sucess
 *    0 of no scenario where and how to add a node was found
 *   -1 if node which is to be added (*List) is NULL
 */
	node_t *Tmpnode; 
 
	if(List == NULL){
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
					if(cat_list(2, Tmpnode, Popts) != 0){
						Warning("Write data problem");
						return -1;
					}

					Tmpnode = Tmpnode->next;
				}
				else
				{
					PrintListInfo(Tmpnode, Popts);
/*
 * if the node in initial call is FILE, print just info about this node
 * and return. Avoids printing all next nodes. This is done only if node in initial
 * call is DIR type
 */
					if(call == 1) return 0;
					Tmpnode = Tmpnode->next;
				}
			}
		}
	}
	else
	{
/*
 * do not print the head node
 */
//		if (call != 1) PrintListInfo(List, option, optionp);
		PrintListInfo(List, Popts);
		
		if(call == 1){
			Tmpnode = List->child;
			if(cat_list(2, Tmpnode, Popts) != 0){
				Warning("Write data problem");
				return -1;
			}
		}
		else if(call == 2 && Popts->opt_L == 'L'){
			Tmpnode = List->child;
			if(cat_list(2, Tmpnode, Popts) != 0){
				Warning("Write data problem");
				return -1;
			}
		}
	}
	return 0;
}


void PrintListInfo(node_t *List, opts_t *Popts)
{
size_t i, maxdim, tabs;
node_t *Tmp;

	if( Popts->opt_s == 'a'){
		if(List->parent == NULL){
			if(Popts->opt_p == 'p'){
				printf("H-%s\t%p\t", List->type, List);
			}
			else
			{
				printf("H-%s\t", List->type);
			}
		}
		else
		{
			if(Popts->opt_p == 'p'){
				printf(" -%s\t%p\t", List->type, List);
			}
			else
			{
				printf(" -%s\t", List->type);
			}
		}
		
		Tmp = List;
		tabs = 0;
		while(Tmp->parent != NULL)
		{
			printf("\t");
			Tmp = Tmp->parent;
			tabs++;
		}
		printf(" %s \t\t %ld\t", List->name, List->ndim);
		if(strncmp(List->type,"DIR",3) != 0){
		maxdim = 1;
			for (i=0; i<List->ndim; i++){
				printf("%ld   ", List->fdim[i]);
				maxdim = maxdim * List->fdim[i] ;
			}
		printf("\n");

		for(i=0; i<=tabs+3; i++){
			printf("\t");
		}		
/*
 * decide what type of data is here, at the moment, only integers
 */
			for ( i=0; i<maxdim; i++)
				printf("%d ",List->data.i[i]);
		}
		printf("\n");
	}
	else if (Popts->opt_s == 'd'){
		if(List->parent == NULL){
			if(Popts->opt_p == 'p'){
				printf("H-%s\t%p\t", List->type, List);
			}
			else
			{
				printf("H-%s\t", List->type);
			}
		}
		else
		{
			if(Popts->opt_p == 'p'){
				printf(" -%s\t%p\t", List->type, List);
			}
			else
			{
				printf(" -%s\t", List->type);
			}
		}
		
		Tmp = List;
		while(Tmp->parent != NULL)
		{
			printf("\t");
			Tmp = Tmp->parent;
		}
		printf(" %s \t\t %ld\t", List->name, List->ndim);
		if(strncmp(List->type,"DIR",3) != 0){
			for (i=0; i<List->ndim; i++)
				printf("%ld   ", List->fdim[i]);
			}
		printf("\n");
	}
	else if (Popts->opt_s == 'q'){
		printf("%s\t%s\n", List->type, List->name);
	}
	else
	{
		printf("%s\t", List->name);
	}
	return;
}