/*
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "ln_list.h"
#include "add_list.h"
#include "Locator.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"

size_t ln_list(int , node_t **, node_t **, char*, opts_t * );

/*
 * function links list. If the list has children, it deletes them before removing list.
 * called recursivelly
 * upon return, returns number of deleted lists, upon failure returns -1
 */

size_t ln_caller(node_t *SList, const char *s_path, const char *s_path_loc, node_t **TList, const char *t_path, const char *t_path_loc, opts_t *Popts)
{
/*
 * function is a caller of the cp functions
 */
	size_t i,j,k,l , ln_tot_nodes, ln_nodes;
	find_t *SFounds, *TFounds;
	int init_call;
	char *name, *path, *path_loc, *newname;
	const char *pc;
	node_t *Tmpnode, *TmpnodePar;
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
/*
 * check if the parent directory exist
 */			
			TmpnodePar = SFounds->Found_Nodes[i]->List->parent;

			if(TmpnodePar == NULL){
				Warning("can not copy to NULL dir");
			}
			else{
				Tmpnode = SFounds->Found_Nodes[i]->List;			
			
				if( (ln_nodes = (size_t) ln_list(init_call, &Tmpnode, &TmpnodePar,  (char *)t_path, Popts ) ) < 0){
					Warning("problem in ln_list");
				}
				else{
				ln_tot_nodes += ln_nodes;
				}
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
	
				ln_tot_nodes = 0;
				for(i=0; i< SFounds->founds; i++){
/*
 * copy and change the name of the list
 */
					if( (ln_nodes = (size_t) ln_list(init_call, &SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  newname, Popts ) ) < 0){
						Warning("problem in ln_list");
					}
					else{
						ln_tot_nodes += ln_nodes;
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
				return ln_tot_nodes;
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
			
			ln_tot_nodes = 0;
				
			for(i=0; i< SFounds->founds; i++){
				
				if( (ln_nodes = (size_t) ln_list(init_call, &SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List, (char *) NULL, Popts )) < 0){
					Warning("problem in ln_list");
				}
				else{
					ln_tot_nodes += ln_nodes;
				}
			}
					
			DestroyFound(&TFounds);
			DestroyFound(&SFounds);
			return ln_tot_nodes;
		}
	}
}

size_t ln_list(int call, node_t **Slist, node_t **Tlist, char* NewName, opts_t *Popts){
	
	return 1;
}


node_t *ctr_link_node(node_t **Slist, node_t **Tlist, char *NewName){
/*
 * function creates link node, 
 * parent of the node is Tlist, child of the node is Slist (Link)
 */
	node_t *Pnode;
	tmpstruct_t TMPSTR;
	size_t i;
	
	if(NewName != NULL){
		if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s",NewName) < 0){
			Perror("snprintf");
			return (node_t *) NULL;
		}
	}
	else{
		if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", (*Slist)->name) < 0){
			Perror("snprintf");
			return (node_t *) NULL;
		}
	}

	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", (*Slist)->type) < 0){
		Perror("snprintf");
		return (node_t *) NULL;
	}
	TMPSTR.dim = NULL;	
/*
 * create new node
 */
	if( (Pnode = AllocateNode(TMPSTR)) == NULL){
		Error("Allocate");
		return (node_t *)NULL;
	}
	
	Pnode->parent = (*Tlist);
	Pnode->child  = (*Slist);
	
	(*Tlist)->ndim++;
// 	(*Slist)->linknode.......
	
	return Pnode;
	
}