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
 *     Function cp_list.c
 *
 *     Author: Adam Jirasek
 *     Date: 2012-06-30
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
 *     Date		Version		Patch number		Author			Descritpion
 *
 */




 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "cp_list.h"
#include "add_list.h"
#include "locate_list.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "find_list.h"

static int m3l_cp_list(int , node_t **, node_t **, char *, opts_t *);
static node_t *m3l_cp_crt_list( node_t **, opts_t *);
static node_t *m3l_cp_crt_list_item(node_t **, opts_t *);
static int m3l_cp_recrt_list(node_t ** , node_t **, char *, opts_t *);
static int m3l_cp_list_content(node_t **, node_t *);

/*
 * function deletes list. If the list has children, it deletes them before removing list.
 * called recursivelly
 * upon return, returns number of deleted lists, upon failure returns -1
 */

size_t m3l_cp_caller(node_t **SList, const char *s_path, const char *s_path_loc, node_t **TList, const char *t_path, const char *t_path_loc, opts_t *Popts)
{
/*
 * function is a caller of the cp functions
 */
	size_t i,j,k,l , cp_tot_nodes, cp_nodes, len;
	find_t *SFounds, *TFounds;
	int init_call;
	char *name, *path, *path_loc, *newname;
	const char *pc;
	node_t *Tmpnode, *TmpnodePar;
/*
 * check if data set exists
 */
	if((*SList) == NULL){
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
	if ( (SFounds = m3l_locator_caller( (*SList), s_path, s_path_loc, Popts)) == NULL){
		Warning("Cp: NULL SFounds");
		return 0;
	}
/*
 * check only one node is to be copied to the same directory  (ie. path is onlu ./ (dotslash)
 */
	len = strlen(t_path_loc);
	if(strncmp(t_path_loc, "./", 2) == 0   &&  len == 2){		
		
		for(i=0; i< SFounds->founds; i++){
/*
 * check if the parent directory exist
 */			
			TmpnodePar = SFounds->Found_Nodes[i]->List->parent;

			if(TmpnodePar == NULL){
				Warning("can not copy to NULL dir");
			}
			else{
				Tmpnode = SFounds->Found_Nodes[i]->List;
				
				if(Tmpnode == TmpnodePar){
					Warning("mv_list: can not move node to itself");
				}
				else{
					if( (cp_nodes = (size_t) m3l_cp_list(init_call, &Tmpnode, &TmpnodePar,  (char *)t_path, Popts ) ) < 0){
						Warning("problem in ln_list");
					}
					else{
					cp_tot_nodes += cp_nodes;
					}
				}
			}
		}
		i = SFounds->founds;
		m3l_DestroyFound(&SFounds);
		return i;
	}
	else{
/*
 * locate target; if target == NULL, just rename the node(s)
 */
		if ( (TFounds = m3l_locator_caller( *TList, t_path, t_path_loc, Popts)) == NULL){
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
					m3l_DestroyFound(&SFounds);
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
				if ( (TFounds = m3l_locator_caller( *TList, path, path_loc, Popts)) == NULL){
					free(path);
					free(path_loc);	
					free(newname);
					m3l_DestroyFound(&SFounds);
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
					m3l_DestroyFound(&SFounds);
					m3l_DestroyFound(&TFounds);
					return -1;
				}
	
				cp_tot_nodes = 0;
				for(i=0; i< SFounds->founds; i++){
/*
 * copy and change the name of the list
 */
					if(SFounds->Found_Nodes[i]->List == TFounds->Found_Nodes[0]->List){
						Warning("mv_list: can not move node to itself");
					}
					else{
						if( (cp_nodes = (size_t) m3l_cp_list(init_call, &SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  newname, Popts ) ) < 0){
							Warning("problem in ln_list");
						}
						else{
							cp_tot_nodes += cp_nodes;
						}
					}
				}
/*
 * free borrowed memory
 */
				free(path);
				free(path_loc);
				free(newname);
				m3l_DestroyFound(&SFounds);
				m3l_DestroyFound(&TFounds);
				return cp_tot_nodes;
			}
			else{
/*
 * target does not exist
 */
				m3l_DestroyFound(&SFounds);
				return -1;
			}
		}
		else{
/*
 * check that target node is only 1
 */
			if(TFounds->founds != 1){
				Warning("cp: multiple target nodes");
				m3l_DestroyFound(&TFounds);
				m3l_DestroyFound(&SFounds);
				return -1;
			}
/*
 * check that if there is more then one source, the target node is DIR type
 */
			if( SFounds->founds > 1 && strncmp(TFounds->Found_Nodes[0]->List->type, "DIR", 3) != 0){
				Warning("cp: target node is not DIR");
				m3l_DestroyFound(&TFounds);
				m3l_DestroyFound(&SFounds);
				return -1;
			}
			
			cp_tot_nodes = 0;
				
			for(i=0; i< SFounds->founds; i++){
					
				
				if(SFounds->Found_Nodes[i]->List == TFounds->Found_Nodes[0]->List){
					Warning("mv_list: can not move node to itself");
				}
				else{
					if( (cp_nodes = (size_t) m3l_cp_list(init_call, &SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List, (char *) NULL, Popts )) < 0){
						Warning("problem in cp_list");
					}
					else{
						cp_tot_nodes += cp_nodes;
					}
				}
			}
					
			m3l_DestroyFound(&TFounds);
			m3l_DestroyFound(&SFounds);
			return cp_tot_nodes;
		}
	}
}


int m3l_cp_list(int call, node_t **SList, node_t **TList, char *NewName, opts_t *Popts)
{
/*
 * function copies list SList to a target list TList
 * 	if the SList is FILE, TLIST has to be file too. The data sunion and ->fdim of the TList target nodes is furst freed, then reallocated
 *	and then SList data union + ->fdim and ->ndim are copied.
 * 	if the TList is DIR the Slist is re-created and added to the TList. In case SList is a DIR, the copying is done by 
 *	traversing the list and copying it item-by-item
 */
	node_t *NewList, *Tmpnode, *TmpnodePrev, *TmpnodeNext;
	char *name;
/*
 * copy source (Slist) to target (Tlist)
 */
	if(strncmp( (*TList)->type, "DIR", 3) != 0){
/*
 * *Tlist is FILE, check that SList is not DIR
 */
		if(strncmp( (*SList)->type, "DIR", 3) == 0){
			Warning(" cp_list: cannot overwrite non-DIR  with DIR ");
			return -1;
		}
/*
 * copy content of the list
 */
		if(strncmp((*SList)->type, "LINK", 4) != 0){
/*
 * node is not link
 */
			if (  m3l_cp_recrt_list(TList, SList, NewName, Popts) < 1){
				Error("Copying list");
				return -1;
			}
		}
		else{
/*
 * node is link
 */
			if(Popts->opt_L = 'L'){
/*
 * dereference link
 */
				Tmpnode = (*SList)->child;
				TmpnodeNext = Tmpnode->next;
				TmpnodePrev = Tmpnode->prev;
				Tmpnode->next = NULL;
				Tmpnode->prev = NULL;
				
				if (  m3l_cp_recrt_list(TList, &Tmpnode, NewName, Popts) < 1){
					Error("Copying list");
					return -1;
				}
				Tmpnode->next = TmpnodeNext;
				Tmpnode->prev = TmpnodePrev;
				
				if( m3l_AllocateLinkInfo(&((*SList)->child), TList, Popts) < 0){
					Error("AllocateLinkInfo");
					return -1;
				}
			}
			else{
/*
 * copy node as usual
 */
				if (  m3l_cp_recrt_list(TList, SList, NewName, Popts) < 1){
					Error("Copying list");
					return -1;
				}	
			}
		}
		
		return 1;
	}
	else{
/*
 * copy content of the list
 */
		if(strncmp( (*SList)->type, "DIR", 3) == 0){
/*
 * SList is DIR, traverese and copy item-by-item
 */
			if ( (NewList = m3l_cp_crt_list(SList, Popts)) == NULL){
				Error("Copying list");
				return -1;
			}
		}
		else{
/*
 * Slist is FILE type, skip traversing and copy item directly
 */
			if ( (NewList = m3l_cp_crt_list_item(SList, Popts)) == NULL){
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
		if ( m3l_add_list(&NewList, TList, Popts) < 0){
			Warning("Error cp_list copy");
			return -1;
		}
	}
}

node_t *m3l_cp_crt_list(node_t **List, opts_t *Popts)
{
/*
 * function creates list. It is called if the target is DIR.
 * If SList is DIR, copying is done by traversin entire SList and copying each item
 */
	node_t *Tmpnode, *NewList, *RetNode;
	size_t i;

	if(*List == NULL){
		Warning("WriteData: NULL list");
		return (node_t *) NULL;
	}
/*
 * if initial call, create node, per default it will be DIR
 */
	if( (RetNode = m3l_cp_crt_list_item(List,Popts)) == NULL){
		Error(" cp_crt_list_item");
		return (node_t *) NULL;
	}

	Tmpnode = (*List)->child;
/*
 * loop over nodes
 */
	while( Tmpnode != NULL){
/*
 * List is DIR, call cp_crt_list recursivelly
 */
		if(strncmp(Tmpnode->type, "DIR", 3) == 0){
			if ( (NewList = m3l_cp_crt_list(&Tmpnode, Popts)) == NULL){
				Error("Copying list");
				return (node_t *) NULL;
			}
		}
		else{
/*
 * list is nod DIR
 * copy content
 */
			if ( (NewList = m3l_cp_crt_list_item(&Tmpnode, Popts)) == NULL){
				Error("Copying list");
				return (node_t *) NULL;
			}
		}
/*
 * add list (RetNode) to parent (NewList)
 */
			if ( m3l_add_list(&NewList, &RetNode, Popts) < 0){
				Warning("Error cp_list copy");
				return (node_t *) NULL;
			}
	
		Tmpnode = Tmpnode->next;
	}

	return RetNode;
}

node_t *m3l_cp_crt_list_item(node_t **Slist, opts_t *Popts)
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
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", (*Slist)->name) < 0){
		Perror("snprintf");
		return (node_t *)NULL;
	}
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", (*Slist)->type) < 0){
		Perror("snprintf");
		return (node_t *)NULL;
	}

	TMPSTR.ndim = (*Slist)->ndim;
	TMPSTR.dim  = NULL;
	
	if(strncmp((*Slist)->type, "DIR", 3) != 0 && strncmp((*Slist)->type, "LINK", 4) != 0){
	
		if( (TMPSTR.dim=(size_t *)malloc(TMPSTR.ndim * sizeof(size_t))) == NULL){
			Perror("malloc");
			return (node_t *)NULL;
		}
		
		for(i=0; i<TMPSTR.ndim; i++)
			TMPSTR.dim[i] = (*Slist)->fdim[i];
/*
 * create new node
 */
		if( (Pnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
			Error("Allocate");
			return (node_t *)NULL;
		}
/*
 * NOTE - here you have to take care of link information
 */

		free(TMPSTR.dim);
		TMPSTR.dim = NULL;
	
		if( m3l_cp_list_content(&Pnode, (*Slist)) != 0){
			Error("cp_list_content");
			return (node_t *)NULL;
		}
	}
	else{
/*
 * List is DIR type
 * create new node, upon creation, initialize the number of items in DIR = 0, will be automatically updated by add_list
 * if list is LINK, set dimensions to 1
 */
		TMPSTR.ndim = 0;
		
		if( (Pnode = m3l_AllocateNode(TMPSTR,Popts)) == NULL){
			Error("Allocate");
			return (node_t *)NULL;
		}
		
		if(strncmp((*Slist)->type, "LINK", 4) == 0){
			Pnode->ndim = 1;
			Pnode->child = (*Slist)->child;
			if( m3l_AllocateLinkInfo(&(*Slist)->child, Pnode) < 0){
				Error("AllocateLinkInfo");
				return (node_t *)NULL;
			}
		}

	}
		
	return Pnode;
}


int m3l_cp_recrt_list(node_t **Tlist, node_t **Slist, char *NewName, opts_t *Popts){
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
		if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", (*Slist)->name) < 0){
			Perror("snprintf");
			return -1;
		}
	}

	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", (*Slist)->type) < 0){
		Perror("snprintf");
		return -1;
	}
	TMPSTR.dim = NULL;
	TMPSTR.ndim = (*Slist)->ndim;

	if(strncmp((*Slist)->type, "DIR", 3) != 0){
/*
 * Tlist node is not DIR
 * if list is not DIR get dimensions
 */
		if(strncmp((*Slist)->type, "LINK", 4) != 0){
			if( (TMPSTR.dim=(size_t *)malloc(TMPSTR.ndim * sizeof(size_t))) == NULL){
				Perror("malloc");
				return -1;
			}
			
			for(i=0; i<TMPSTR.ndim; i++)
				TMPSTR.dim[i] = (*Slist)->fdim[i];
/*
 * re-create Tlist node
 * first - free existing data set
 */
			if ( m3l_Free_data_str(Tlist) != 0){
				Error("Free_data_str");
				return -1;
			}
		}
/*
 * allocate new ->data in the node
 */
		if ( m3l_AllocateNodeData(Tlist, TMPSTR,Popts) != 0){
			Error("AllocateNodeData");
			return -1;
		}
	
		if(TMPSTR.dim == NULL)  free(TMPSTR.dim);
		TMPSTR.dim = NULL;
/*
 * copy data to new node_t
 */
		if(strncmp((*Slist)->type, "LINK", 4) != 0){
			if( m3l_cp_list_content(Tlist, (*Slist)) != 0)
				Error("cp_list_content");
		}
		else{
/*
 * list is LINK, copy address of source child to target child
 */
			(*Tlist)->child = (*Slist)->child;
			if( m3l_AllocateLinkInfo(Slist, (*Tlist)) < 0){
				Error("AllocateLinkInfo");
				return -1;
			}
		}
/*
 * NOTE - here you have to take care of link information
 */
	}
	else{
		Warning("Can not copy DIR to FILE");
		return -1;
	}

	return 1;
}

	

int m3l_cp_list_content(node_t **Pnode, node_t *Slist)
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