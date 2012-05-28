/*
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "cp_list.h"
#include "add_list.h"
#include "Locator.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"

#include "Cat.h"


static int cp_list(int , node_t *, node_t **, opts_t *);
static node_t *cp_crt_list(node_t *);
static int cp_recrt_list(node_t ** , node_t *);
static int cp_list_content(node_t **, node_t *);

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
 * check if data set exists
 */
	if(SList == NULL){
		Warning("Cp: NULL source list");
		return -1;
	}
	
	if(*TList == NULL){
		Warning("Cp: NULL target list");
		return -1;
	}
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
	
	
	printf(" number of found nodes is %ld   %ld\n", SFounds->founds, TFounds->founds);
	
	cp_tot_nodes = 0;
		
	for(i=0; i< SFounds->founds; i++){
		
		if( (cp_nodes = (size_t) cp_list(init_call, SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  Popts )) < 0){
			Warning("problem in cp_list");
		}
		else{
			cp_tot_nodes += cp_nodes;
		}
	}
			
	DestroyFound(&TFounds);
	DestroyFound(&SFounds);
	return cp_tot_nodes;
}


int cp_list(int call, node_t *SList, node_t **TList, opts_t *Popts)
{
	int ncp;
	node_t *NewList;
/*
 * copy source (Slist) to target (Tlist)
 */
	if(strncmp( (*TList)->type, "DIR", 3) != 0){
/*
 * check that source is not DIR type
 */
		if(strncmp( SList->type, "DIR", 3) == 0){
			Warning(" cp_list: cannot overwrite non-DIR  with DIR ");
			return -1;
		}
/*
 * copy content of the list
 */
		if (  cp_recrt_list(TList, SList) < 1){
			Error("Copying list");
			return -1;
		}
		
		return 1;
	}
	else{
/*
 * copy content of the list
 */
		if ( (NewList = cp_crt_list(SList)) == NULL){
			Error("Copying list");
			return -1;
		}
 		Cat(SList, "--all", "-P", "-L", "*", (char *)NULL);
 				printf(" %p \n", NewList);
 		Cat(NewList, "--all", "-P", "-L", "*", (char *)NULL);
/*
 * add a new node to the DIR list
 */
		if ( (ncp = add_list(&NewList, TList, Popts)) < 0){
			Warning("Error cp_list copy");
			return -1;
		}
	}
}


node_t *cp_crt_list(node_t *Slist)
{
	
	node_t *Pnode;
	tmpstruct_t TMPSTR;
	size_t i;
/* 
 * copy name, type, number iof dimensions, dimensions
 */
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", Slist->name) < 0){
		Perror("snprintf");
		return (node_t *)NULL;
	}
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", Slist->type) < 0){
		Perror("snprintf");
		return (node_t *)NULL;
	}
	if( (TMPSTR.ndim = Slist->ndim) > 0){
	
		if( (TMPSTR.dim=(size_t *)malloc(TMPSTR.ndim * sizeof(size_t))) == NULL){
			Perror("malloc");
			return (node_t *)NULL;
		}
		
		for(i=0; i<TMPSTR.ndim; i++)
			TMPSTR.dim[i] = Slist->fdim[i];
	}
/*
 * create new node
 */
	if( (Pnode = AllocateNode(TMPSTR)) == NULL){
		Error("Allocate");
		return (node_t *)NULL;
	}
/*
 * NOTE - here you have to take care of link information
 */

	free(TMPSTR.dim);
	TMPSTR.dim = NULL;

	if( cp_list_content(&Pnode, Slist) != 0){
		Error("cp_list_content");
		return (node_t *)NULL;
	}
		
	return Pnode;
}


int cp_recrt_list(node_t ** Tlist, node_t *Slist){
	
	node_t *Pnode;
	tmpstruct_t TMPSTR;
	size_t i;
/* 
 * copy name, type, number iof dimensions, dimensions
 */
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", Slist->name) < 0){
		Perror("snprintf");
		return -1;
	}
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", Slist->type) < 0){
		Perror("snprintf");
		return -1;
	}
		
	if(  (TMPSTR.ndim = Slist->ndim) > 0){
/*
 * if list is not DIR get dimensions
 */
		if( (TMPSTR.dim=(size_t *)malloc(TMPSTR.ndim * sizeof(size_t))) == NULL){
			Perror("malloc");
			return -1;
		}
		
		for(i=0; i<TMPSTR.ndim; i++)
			TMPSTR.dim[i] = Slist->fdim[i];
	}
/*
 * re-create Tlist node
 * first - free existing data set
 */
	if ( Free_data_str(Tlist) != 0){
		Error("Free_data_str");
		return -1;
	}
/*
 * allocate new ->data in the node
 */
	if ( AllocateNodeData(Tlist, TMPSTR) != 0){
		Error("AllocateNodeData");
		return -1;
	}
	
	free(TMPSTR.dim);
	TMPSTR.dim = NULL;
/*
 * NOTE - here you have to take care of link information
 */

/*
 * copy data to new node_t
 */
	if( cp_list_content(Tlist, Slist) != 0)
		Error("cp_list_content");

	return 1;
}



int cp_list_content(node_t **Pnode, node_t *Slist)
{
/*
 * copy content of the list
 */
	size_t i, tot_dim;
		
	
	(*Pnode)->ndim = Slist->ndim;

	tot_dim = 1;
	for(i=0; i<Slist->ndim; i++){
		(*Pnode)->fdim[i] = Slist->fdim[i];
		tot_dim = tot_dim * Slist->fdim[i];
	}
		
	if (strncmp(Slist->type,"LD",2) == 0){  /* long double */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.ldf[i] = Slist->data.ldf[i];
	}
	else if(strncmp(Slist->type,"D",1) == 0){  /* double */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.df[i] = Slist->data.df[i];
	}
	else if(strncmp(Slist->type,"F",1) == 0){  /* float */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.f[i] = Slist->data.f[i];
	}
/*
 * chars, do not serialize, write as they are
 */
	else if (strncmp(Slist->type,"SC",2) == 0){  /* signed char */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.sc[i] = Slist->data.sc[i];
	}
	else if(strncmp(Slist->type,"UC",2) == 0){  /* unsigned char */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.uc[i] = Slist->data.uc[i];
	}
	else if(strncmp(Slist->type,"C",1) == 0){  /* char */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.c[i] = Slist->data.c[i];
	}
/*
 * integers
 */
	else if(strncmp(Slist->type,"ULLI",4) == 0){  /* unsigned long long  int */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.ulli[i] = Slist->data.ulli[i];
	}
	else if(strncmp(Slist->type,"SLLI",4) == 0){  /* signed long long  int */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.slli[i] = Slist->data.slli[i];
	}
	else if(strncmp(Slist->type,"LLI",3) == 0){  /* unsigned short int */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.lli[i] = Slist->data.lli[i];
	}
	else if(strncmp(Slist->type,"ULI",3) == 0){  /* unsigned long int */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.uli[i] = Slist->data.uli[i];
	}
	else if(strncmp(Slist->type,"USI",3) == 0){  /* unsigned short int */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.usi[i] = Slist->data.usi[i];
	}
	else if(strncmp(Slist->type,"SI",2) == 0){  /* short int */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.si[i] = Slist->data.si[i];
	}
	else if(strncmp(Slist->type,"UI",2) == 0){  /* unsigned int */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.ui[i] = Slist->data.ui[i];
	}
	else if(strncmp(Slist->type,"LI",2) == 0){  /* long  int */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.li[i] = Slist->data.li[i];
	}
	else if(strncmp(Slist->type,"I",1) == 0){  /* int */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.i[i] = Slist->data.i[i];
	}
/*
 * counters
 */
	else if(strncmp(Slist->type,"ST",2) == 0){  /* size_t */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.st[i] = Slist->data.st[i];
	}
	else if(strncmp(Slist->type,"PTRDF",1) == 0){  /* ptrdf_t */
		for(i=0; i<tot_dim; i++)
			(*Pnode)->data.ptrdf[i] = Slist->data.ptrdf[i];
	}
	
	return 0;
}
