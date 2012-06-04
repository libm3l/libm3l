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

static int cp_list(int , node_t *, node_t **, char *, opts_t *);
static node_t *cp_crt_list( node_t *, opts_t *);
static node_t *cp_crt_list_item(node_t *);
static int cp_recrt_list(node_t ** , node_t *, char *);
static int cp_list_content(node_t **, node_t *);

/*
 * function deletes list. If the list has children, it deletes them before removing list.
 * called recursivelly
 * upon return, returns number of deleted lists, upon failure returns -1
 */

size_t cp_caller(node_t *SList, const char *s_path, const char *s_path_loc, node_t **TList, const char *t_path, const char *t_path_loc, opts_t *Popts)
{
/*
 * function is a caller of the cp functions
 */
	size_t i,j,k,l , cp_tot_nodes, cp_nodes;
	find_t *SFounds, *TFounds;
	int init_call;
	char *name, *path, *path_loc, *newname;
	const char *pc;
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
 * check location of sources
 */
	if ( (SFounds = locator_caller( SList, s_path, s_path_loc, Popts)) == NULL){
		Warning("Cp: NULL SFounds");
		return 0;
	}
/*
 * check only one node is to be copied to the same directory
 */
	if(strncmp(t_path_loc, "./", 2) == 0){
		for(i=0; i< SFounds->founds; i++){
			name = SFounds->Found_Nodes[i]->List->name;
			bzero(name, sizeof(name));
			if( snprintf(name,MAX_NAME_LENGTH,"%s",t_path) < 0)
				Perror("snprintf");
			
/* NOTE - need check !!!!! */			if( (cp_nodes = (size_t) cp_list(init_call, SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  newname, Popts ) ) < 0){
						Warning("problem in ln_list");
			}
			else{
				cp_tot_nodes += cp_nodes;
			}
		}
		i = SFounds->founds;
		DestroyFound(&SFounds);
		return i;
	}
	else{
/*
 * locate target; if target == NULL, just rename the node(s)
 */
		if ( (TFounds = locator_caller( *TList, t_path, t_path_loc, Popts)) == NULL){
/*
 * check it the direcotry exist, if it does, the name is new name
 */
			pc = t_path;
			i = 0;
			k = 0;
/*
 * count number of '\'
 */
			while(*pc != '\0'){
				if(*pc++ == '/')i++;
				k++;
				}
/*
 * if larger then 1 
 */
			if(i > 1){
				pc = t_path;
	
				if(  ( path = (char *)malloc( (k+1)*sizeof(char))) == NULL){
					Perror("malloc");
					return -1;
				}
				if(  (newname = (char *)malloc( (k+1)*sizeof(char))) == NULL){
					Perror("malloc");
					return -1;
				}
/*
 * get path up to new name
 */
				j = 0;
				l = 0;
				while(*pc != '\0'){
					if(*pc == '/')j++;
					if(j == i )break;
					path[l++] = *pc++;
				}
				path[l] = '\0';
/*
 * get the last argument which is newname
 */
				l = 0;
				pc++;
				while(*pc != '\0'){
					newname[l++] = *pc++;}
				newname[l] = '\0';
/*
 * get path location
 */
				pc = t_path_loc;
				while(*pc++ != '\0')k++;
				if(  ( path_loc= (char *)malloc( (k+1)*sizeof(char))) == NULL){
					Perror("malloc");
					free(path);
					free(path_loc);
					free(newname);
					DestroyFound(&SFounds);
					return -1;
				}
				j = 0;
				l = 0;
				pc = t_path_loc;
				while(*pc != '\0'){
					if(*pc == '/')j++;
					if(j == i )break;
					path_loc[l++] = *pc++;
				}
				path_loc[l] = '\0';
/*
 * make new find for parent dir of the new name
 */
				if ( (TFounds = locator_caller( *TList, path, path_loc, Popts)) == NULL){
					free(path);
					free(path_loc);	
					free(newname);
					DestroyFound(&SFounds);
					return -1;
				}
/*
 * check the found is DIR
 */
				pc = TFounds->Found_Nodes[0]->List->type;
				if(strncmp(pc, "DIR", 3) != 0 || TFounds->founds > 1){
					Warning("Wrong or not existing target");
					free(path);
					free(path_loc);
					free(newname);
					DestroyFound(&SFounds);
					DestroyFound(&TFounds);
					return -1;
				}
	
				cp_tot_nodes = 0;
				for(i=0; i< SFounds->founds; i++){
/*
 * copy and change the name of the list
 */
					if( (cp_nodes = (size_t) cp_list(init_call, SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  newname, Popts ) ) < 0){
						Warning("problem in ln_list");
					}
					else{
						cp_tot_nodes += cp_nodes;
					}
				}
/*
 * free borrowed memory
 */
				free(path);
				free(path_loc);
				free(newname);
				DestroyFound(&SFounds);
				DestroyFound(&TFounds);
				return cp_tot_nodes;
			}
			else{
/*
 * target does not exist
 */
				DestroyFound(&SFounds);
				return -1;
			}
		}
		else{
/*
 * check that target node is only 1
 */
			if(TFounds->founds != 1){
				Warning("cp: multiple target nodes");
				DestroyFound(&TFounds);
				DestroyFound(&SFounds);
				return -1;
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
				
				if( (cp_nodes = (size_t) cp_list(init_call, SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List, (char *) NULL, Popts )) < 0){
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
	}
}


int cp_list(int call, node_t *SList, node_t **TList, char *NewName, opts_t *Popts)
{
/*
 * function copies list SList to a target list TList
 * 	if the SList is FILE, TLIST has to be file too. The data sunion and ->fdim of the TList target nodes is furst freed, then reallocated
 *	and then SList data union + ->fdim and ->ndim are copied.
 * 	if the TList is DIR the Slist is re-created and added to the TList. In case SList is a DIR, the copying is done by 
 *	traversing the list and copying it item-by-item
 */
	node_t *NewList;
	char *name;
/*
 * copy source (Slist) to target (Tlist)
 */
	if(strncmp( (*TList)->type, "DIR", 3) != 0){
/*
 * *Tlist is FILE, check that SList is not DIR
 */
		if(strncmp( SList->type, "DIR", 3) == 0){
			Warning(" cp_list: cannot overwrite non-DIR  with DIR ");
			return -1;
		}
/*
 * copy content of the list
 */
		if (  cp_recrt_list(TList, SList, NewName) < 1){
			Error("Copying list");
			return -1;
		}
		
		return 1;
	}
	else{
/*
 * copy content of the list
 */
		if(strncmp( SList->type, "DIR", 3) == 0){
/*
 * SList is DIR, traverese and copy item-by-item
 */
			if ( (NewList = cp_crt_list(SList, Popts)) == NULL){
				Error("Copying list");
				return -1;
			}
		}
		else{
/*
 * Slist is FILE type, skip traversing and copy item directly
 */
			if ( (NewList = cp_crt_list_item(SList)) == NULL){
				Error("Copying list");
				return -1;
			}
		}
/*
 * if list has a different name then original list, rename the list
 */			
		if(NewName != NULL){
			bzero(NewList->name, sizeof(name));
			if( snprintf(NewList->name,MAX_NAME_LENGTH,"%s",NewName) < 0){
				Perror("snprintf");
				return -1;
			}
		}
/*
 * add a new node to the DIR list
 */
		if ( add_list(&NewList, TList, Popts) < 0){
			Warning("Error cp_list copy");
			return -1;
		}
	}
}

node_t *cp_crt_list(node_t *List, opts_t *Popts)
{
/*
 * function creates list. It is called if the target is DIR.
 * If SList is DIR, copying is done by traversin entire SList and copying each item
 */
	node_t *Tmpnode, *NewList, *RetNode;
	size_t i;

	if(List == NULL){
		Warning("WriteData: NULL list");
		return (node_t *) NULL;
	}
/*
 * if initial call, create node, per default it will be DIR
 */
	if( (RetNode = cp_crt_list_item(List)) == NULL){
		Error(" cp_crt_list_item");
		return (node_t *) NULL;
	}

	Tmpnode = List->child;
/*
 * loop over nodes
 */
	while( Tmpnode != NULL){
/*
 * List is DIR, call cp_crt_list recursivelly
 */
		if(strncmp(Tmpnode->type, "DIR", 3) == 0){
			if ( (NewList = cp_crt_list(Tmpnode, Popts)) == NULL){
				Error("Copying list");
				return (node_t *) NULL;
			}
		}
		else{
/*
 * list is nod DIR
 * copy content
 */
			if ( (NewList = cp_crt_list_item(Tmpnode)) == NULL){
				Error("Copying list");
				return (node_t *) NULL;
			}
		}
/*
 * add list (RetNode) to parent (NewList)
 */
			if ( add_list(&NewList, &RetNode, Popts) < 0){
				Warning("Error cp_list copy");
				return (node_t *) NULL;
			}
	
		Tmpnode = Tmpnode->next;
	}

	return RetNode;
}

node_t *cp_crt_list_item(node_t *Slist)
{
/*
 * function creates a single list and copy the Slist into it
 * List can be both DIR and FILE type
 */
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
	TMPSTR.dim  = NULL;
	
	if(strncmp(Slist->type, "DIR", 3) != 0){
	
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

		free(TMPSTR.dim);
		TMPSTR.dim = NULL;
	
		if( cp_list_content(&Pnode, Slist) != 0){
			Error("cp_list_content");
			return (node_t *)NULL;
		}
	}
	else{
/*
 * List is DIR type
 * create new node, upon creation, initialize the number of items in DIR = 0, will be automatically updated by add_list
 */
 		TMPSTR.ndim = 0;
		if( (Pnode = AllocateNode(TMPSTR)) == NULL){
			Error("Allocate");
			return (node_t *)NULL;
		}	
	}
		
	return Pnode;
}


int cp_recrt_list(node_t ** Tlist, node_t *Slist, char *NewName){
/*
 * function realloc the existing list. Used if both source and target lists are FILE types
 *	first the target list data union and ->fdim is freed, then reallocated and Slist data union, ->fdim and ->ndim is sopied to 
 *	Tlist
 */
	node_t *Pnode;
	tmpstruct_t TMPSTR;
	size_t i;
/* 
 * copy name, type, number iof dimensions, dimensions
 * if list has a different name then original list, rename the list
 */			
	if(NewName != NULL){
		if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s",NewName) < 0){
			Perror("snprintf");
			return -1;
		}
	}
	else{
		if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", Slist->name) < 0){
			Perror("snprintf");
			return -1;
		}
	}

	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", Slist->type) < 0){
		Perror("snprintf");
		return -1;
	}
	TMPSTR.dim = NULL;
	TMPSTR.ndim = Slist->ndim;

	if(strncmp(Slist->type, "DIR", 3) != 0){
/*
 * Tlist node is not DIR
 * if list is not DIR get dimensions
 */
		if( (TMPSTR.dim=(size_t *)malloc(TMPSTR.ndim * sizeof(size_t))) == NULL){
			Perror("malloc");
			return -1;
		}
		
		for(i=0; i<TMPSTR.ndim; i++)
			TMPSTR.dim[i] = Slist->fdim[i];
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
	
		if(TMPSTR.dim == NULL)  free(TMPSTR.dim);
		TMPSTR.dim = NULL;
/*
 * copy data to new node_t
 */
		if( cp_list_content(Tlist, Slist) != 0)
			Error("cp_list_content");
/*
 * NOTE - here you have to take care of link information
 */
	}

	return 1;
}



int cp_list_content(node_t **Pnode, node_t *Slist)
{
/*
 * fuction copies content of the list ie.
 *	->ndim
 *	->fdim[]
 *	->data.*[]
 */
	size_t i, tot_dim;
/*
 * copy number if dimensions and each dimension size
 */
	(*Pnode)->ndim = Slist->ndim;

	tot_dim = 1;
	if((*Pnode)->fdim == NULL){
		Error("cp_list_content fdim does not exist");
		return -1;
	}

	for(i=0; i<Slist->ndim; i++){
		(*Pnode)->fdim[i] = Slist->fdim[i];
		tot_dim = tot_dim * Slist->fdim[i];
	}
/*
 * depending on type of FILE copy fata union content
 */		
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
/*
 * upon success, return 0
 */	
	return 0;
}