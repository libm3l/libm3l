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
#include "udf_rm.h"


static int cp_list(int , node_t *, node_t **, opts_t *);
static node_t *cp_list_content(node_t *SList);

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
		if ( (NewList = cp_list_content(SList)) == NULL){
			Error("Copying list");
			return -1;
		}
		
		return 1;
	}
	else{
/*
 * copy content of the list
 */
		if ( (NewList = cp_list_content(SList)) == NULL){
			Error("Copying list");
			return -1;
		}
/*
 * add a new node to the DIR list
 */
		if ( (ncp = add_list(&NewList, TList, Popts)) < 0){
			Warning("Error cp_list copy");
			return -1;
		}
	}
}


node_t *cp_list_content(node_t *Slist){
	
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
	TMPSTR.ndim = Slist->ndim;

	if( (TMPSTR.dim=(size_t *)malloc(TMPSTR.ndim * sizeof(size_t))) == NULL){
		Perror("malloc");
		return (node_t *)NULL;
	}
	
	for(i=0; i<TMPSTR.ndim; i++)
		TMPSTR.dim[i] = Slist->fdim[i];
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


/*
 * copy content of the list
 */
		if (strncmp((*Lnode)->type,"LD",2) == 0){  /* long double */
			free( (*Lnode)->data.ldf);
			(*Lnode)->data.ldf = NULL;
		}
		else if(strncmp((*Lnode)->type,"D",1) == 0){  /* double */
			free( (*Lnode)->data.df);
			(*Lnode)->data.df = NULL;
		}
		else if(strncmp((*Lnode)->type,"F",1) == 0){  /* float */
			free( (*Lnode)->data.f);
			(*Lnode)->data.f = NULL;
		}
/*
 * chars, do not serialize, write as they are
 */
		else if (strncmp((*Lnode)->type,"SC",2) == 0){  /* signed char */
			free( (*Lnode)->data.sc);
			(*Lnode)->data.sc = NULL;
		}
		else if(strncmp((*Lnode)->type,"UC",2) == 0){  /* unsigned char */
			free( (*Lnode)->data.uc);
			(*Lnode)->data.uc = NULL;
		}
		else if(strncmp((*Lnode)->type,"C",1) == 0){  /* char */
			free( (*Lnode)->data.c);
			(*Lnode)->data.c = NULL;
		}
/*
 * integers
 */
		else if(strncmp((*Lnode)->type,"ULLI",4) == 0){  /* unsigned long long  int */
			free( (*Lnode)->data.ulli);
			(*Lnode)->data.ulli = NULL;
		}
		else if(strncmp((*Lnode)->type,"SLLI",4) == 0){  /* signed long long  int */
			free( (*Lnode)->data.slli);
			(*Lnode)->data.slli = NULL;
		}
		else if(strncmp((*Lnode)->type,"LLI",3) == 0){  /* unsigned short int */
			free( (*Lnode)->data.lli);
			(*Lnode)->data.lli = NULL;
		}
		else if(strncmp((*Lnode)->type,"ULI",3) == 0){  /* unsigned long int */
			free( (*Lnode)->data.uli);
			(*Lnode)->data.uli = NULL;
		}
		else if(strncmp((*Lnode)->type,"USI",3) == 0){  /* unsigned short int */
			free( (*Lnode)->data.usi);
			(*Lnode)->data.usi = NULL;
		}
		else if(strncmp((*Lnode)->type,"SI",2) == 0){  /* short int */
			free( (*Lnode)->data.si);
			(*Lnode)->data.si = NULL;
		}
		else if(strncmp((*Lnode)->type,"UI",2) == 0){  /* unsigned int */
			free( (*Lnode)->data.ui);
			(*Lnode)->data.ui = NULL;
		}
		else if(strncmp((*Lnode)->type,"LI",2) == 0){  /* long  int */
			free( (*Lnode)->data.li);
			(*Lnode)->data.li = NULL;
		}
		else if(strncmp((*Lnode)->type,"I",1) == 0){  /* int */
			free( (*Lnode)->data.i);
			(*Lnode)->data.i = NULL;
		}
/*
 * counters
 */
		else if(strncmp((*Lnode)->type,"ST",2) == 0){  /* size_t */
			free( (*Lnode)->data.st);
			(*Lnode)->data.st = NULL;
		}
		else if(strncmp((*Lnode)->type,"PTRDF",1) == 0){  /* ptrdf_t */
			free( (*Lnode)->data.ptrdf);
			(*Lnode)->data.ptrdf = NULL;
		}
	}

	return Pnode;
}
