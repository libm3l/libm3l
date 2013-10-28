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
 *     Function ln_list.c
 *
 *     Date: 2012-06-30
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

#include "ln_list.h"
#include "add_list.h"
#include "locate_list.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "find_list.h"
#include "find_list.h"
#include "rm_list.h"


static lmsize_t m3l_ln_list(lmint_t , node_t **, node_t **, lmchar_t*, opts_t * );
static lmint_t ln_m3l_recrt_list(node_t **, node_t **, lmchar_t *);
static node_t *m3l_ln_crt_list(node_t **, lmchar_t *, opts_t *);

/*
 * function links list. If the list has children, it deletes them before removing list.
 * Source list is linked to Target->child 
 * When the Source is linked to target, strucure Slist->inknode is allocated and filled by Target
 * called recursivelly
 * upon return, returns number of deleted lists, upon failure returns -1
 */

lmsize_t m3l_ln_caller(node_t **SList, const lmchar_t *s_path, const lmchar_t *s_path_loc, node_t **TList, const lmchar_t *t_path, const lmchar_t *t_path_loc, opts_t *Popts)
{
/*
 * function is a caller of the ln functions
 */
	lmsize_t i,j,k,l , ln_tot_nodes, ln_nodes,len;
	find_t *SFounds, *TFounds;
	lmint_t init_call;
	lmchar_t *name, *path, *path_loc, *newname;
	const lmchar_t *pc;
	node_t *Tmpnode, *TmpnodePar;
	opts_t *Popts_Tlist, opts;
/*
 * check if data set exists
 */	
	if(*TList == NULL){
		Warning("Cp: NULL target list");
		return -1;
	}
	
	if (Popts->opt_linkscleanemptlinks == 'e' || Popts->opt_linkscleanemptrefs == 'c'){
 		ln_tot_nodes = m3l_ln_cleanemptylinks(TList,  Popts);
		return ln_tot_nodes;
	}
		
	if(*SList == NULL){
		Warning("Cp: NULL source list");
		return -1;
	}
	
	
/* 
 * check location of sources
 */
	if ( (SFounds = m3l_locator_caller( *SList, s_path, s_path_loc, Popts)) == NULL){
		Warning("Cp: NULL SFounds");
		return 0;
	}
/*
 * set default values of options for TList
 */
	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_k = '\0'; opts.opt_b = '\0'; opts.opt_l = '\0';
	opts.opt_linkscleanemptrefs = '\0'; opts.opt_linkscleanemptlinks = '\0'; opts.opt_nomalloc = '\0';
	Popts_Tlist = &opts;
/*
 * check only one node is to be copied to the same directory  (ie. path is onlu ./ (dotslash)
 */
	len = strlen(t_path_loc);
	if(strncmp(t_path_loc, "./", 2) == 0 && len == 2){
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
			
				if( (ln_nodes = m3l_ln_list(init_call, &Tmpnode, &TmpnodePar,  (lmchar_t *)t_path, Popts ) ) < 0){
					Warning("problem in ln_list");
				}
				else{
				ln_tot_nodes += ln_nodes;
				}
			}
		}
		i = SFounds->founds;
		m3l_DestroyFound(&SFounds);
		return i;
	}
	else{
/*
 * locate target; if target == NULL, just rename the node(s) or --add option specified 
 */
		if ( Popts->opt_add == 'a' || (TFounds = m3l_locator_caller( *TList, t_path, t_path_loc, Popts_Tlist)) == NULL){
/*
 * check it the direcotry exist, if it does, the name is a new name
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
	
				if(  ( path = (lmchar_t *)malloc( (k+1)*sizeof(lmchar_t))) == NULL){
					Perror("malloc");
					return -1;
				}
				if(  (newname = (lmchar_t *)malloc( (k+1)*sizeof(lmchar_t))) == NULL){
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
				if(  ( path_loc= (lmchar_t *)malloc( (k+1)*sizeof(lmchar_t))) == NULL){
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
				if ( (TFounds = m3l_locator_caller( *TList, path, path_loc, Popts_Tlist)) == NULL){
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
	
				ln_tot_nodes = 0;
				for(i=0; i< SFounds->founds; i++){
/*
 * copy and change the name of the list
 */
					if( (ln_nodes = m3l_ln_list(init_call, &SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  newname, Popts ) ) < 0){
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
				m3l_DestroyFound(&SFounds);
				m3l_DestroyFound(&TFounds);
				return ln_tot_nodes;
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
			
			ln_tot_nodes = 0;
				
			for(i=0; i< SFounds->founds; i++){
				
				if( (ln_nodes = m3l_ln_list(init_call, &SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List, (lmchar_t *) NULL, Popts )) < 0){
					Warning("problem in ln_list");
				}
				else{
					ln_tot_nodes += ln_nodes;
				}
			}
					
			m3l_DestroyFound(&TFounds);
			m3l_DestroyFound(&SFounds);
			return ln_tot_nodes;
		}
	}
}

size_t m3l_ln_list(lmint_t call, node_t **SList, node_t **TList, lmchar_t* NewName, opts_t *Popts){
/*
 * function copies list SList to a target list TList
 * 	if the SList is FILE, TLIST has to be file too. The data sunion and ->fdim of the TList target nodes is furst freed, then reallocated
 *	and then SList data union + ->fdim and ->ndim are copied.
 * 	if the TList is DIR the Slist is re-created and added to the TList. In case SList is a DIR, the copying is done by 
 *	traversing the list and copying it item-by-item
 */
	node_t *NewList;
	lmchar_t *name;
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
 * link list to the source list
 */
		if (  m3l_ln_recrt_list(TList, SList, NewName) < 1){
			Error("Copying list");
			return -1;
		}

		return 1;
	}
	else{
/*
 * link list to the directory, first make a new lits
 */
		if ( (NewList = m3l_ln_crt_list(SList, NewName, Popts)) == NULL){
			Error("Copying list");
			return -1;
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
	return 1;
}


node_t *m3l_ln_crt_list(node_t **Slist, lmchar_t *NewName, opts_t *Popts){
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

	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"LINK") < 0){
		Perror("snprintf");
		return (node_t *) NULL;
	}
	TMPSTR.ndim = 1;
	TMPSTR.dim = NULL;	
/*
 * create new node
 */
	if( (Pnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
		Error("Allocate");
		return (node_t *)NULL;
	}

	Pnode->child  = (*Slist);
	
	if( m3l_AllocateLinkInfo(Slist, Pnode) < 0){
		Error("AllocateLinkInfo");
		return (node_t *) NULL;
	}
		
	return Pnode;
}



lmint_t m3l_ln_recrt_list(node_t ** Tlist, node_t **Slist, lmchar_t *NewName){
/*
 *      function frees the existing list. Used if both source and target lists are FILE types
 *	first the target list data union and ->fdim is freed, then reallocated and Slist data union, ->fdim and ->ndim is sopied to 
 *	Tlist
 */
	node_t *Pnode;
	size_t i;
/* 
 * copy name, type, number iof dimensions, dimensions
 * if list has a different name then original list, rename the list
 */
// 	if( (*Tlist) == NULL){
// 		Error("Null Tlist ");	
// 		return -1;
// 	}
// 	
// 	if( (*Slist) == NULL){
// 		Error("Null Slist ");	
// 		return -1;
// 	}
	bzero( (*Tlist)->name, MAX_NAME_LENGTH);
	bzero( (*Tlist)->type, MAX_TYPE_LENGTH);

	if(NewName != NULL){
		if( snprintf((*Tlist)->name, MAX_NAME_LENGTH,"%s",NewName) < 0){
			Perror("snprintf");
			return -1;
		}
	}
	else{
		if( snprintf((*Tlist)->name, MAX_NAME_LENGTH,"%s", (*Slist)->name) < 0){
			Perror("snprintf");
			return -1;
		}
	}

	if( snprintf( (*Tlist)->type, MAX_TYPE_LENGTH,"%s", "LINK") < 0){
		Perror("snprintf");
		return -1;
	}
/*
 * re-create Tlist node
 * first - free existing data set, keep the List itself
 */
	if ( m3l_Free_data_str(Tlist) != 0){
		Error("Free_data_str");
		return -1;
	}
/*
 * NOTE - what if Free_data_str is applied on LINK?
 */
/*
 * set child of the node to the link source
 */
	(*Tlist)->child = (*Slist);
	(*Tlist)->ndim = 1;
	
	if( m3l_AllocateLinkInfo(Slist, (*Tlist)) < 0){
		Error("AllocateLinkInfo");
		return -1;
	}
/*
 * NOTE - here you have to take care of link information in SList
 */

	return 1;
}


lmint_t m3l_AllocateLinkInfo(node_t **Slist, node_t *Tlist){
/*
 * function allocates the linknode structure in the source node (Slist) and fills it with 
 * address if link target (Tlist)
 */
	size_t lcounter; 
	
	if( (*Slist)->linknode == NULL){
/*
 * first link
 * structure is not yet allocated
 */
		if( ( (*Slist)->linknode = (find_str_t **)malloc(sizeof(find_str_t *))) == NULL)
			Perror("linknode malloc");
		
		if( ( (*Slist)->linknode[0] = (find_str_t *)malloc(sizeof(find_str_t))) == NULL)
			Perror("linknode malloc");
		
		(*Slist)->linknode[0]->List = Tlist;
		(*Slist)->lcounter = 1;
		return 1;
	}
	else{
/*
 * structure already exist
 */
		lcounter = (*Slist)->lcounter;
		
		if( ( (*Slist)->linknode = (find_str_t **)realloc( (*Slist)->linknode, (lcounter+1) * sizeof(find_str_t *))) == NULL)
			Perror("linknode malloc");
		
		if( ( (*Slist)->linknode[lcounter] = (find_str_t *)malloc(sizeof(find_str_t))) == NULL)
			Perror("linknode malloc");
		(*Slist)->linknode[lcounter]->List = Tlist;
		(*Slist)->lcounter++;
		
		return 1;
	}
	return -1;
}


size_t m3l_ln_cleanemptylinks(node_t **List,  opts_t *Popt){
/*
 * function traverses entire list and look for empty links. 
 * Once it finds them it removes them
 *
 * if called with recursive option, it traveres entire tree, otherwise it lists the current node children only
 */
	node_t *Tmpnode, *Tmpnode1;
	size_t rm_nodes, cleaned_nodes;
		
	if( (*List) == NULL){
/*
 * Node is FILE type, loop over next nodes
 */
		Warning("WriteData: NULL list");
		return -1;
	}
/*
 * List entire tree
 */ 
	if((*List)->child == NULL){
/*
 * check if list is LINK
 * if empty link and Popt->opt_linkscleanemptlinks == 'e' remove empty link
 * if not link and Popt->opt_linkscleanemptrefs == 'c' check if list is linked to another LIST and 
 * remove NULL references 
 */
		rm_nodes = 0;
		if( strncmp( (*List)->type, "LINK", 4) == 0){
			if(Popt != NULL && (*List)->ndim == 0 && Popt->opt_linkscleanemptlinks == 'e') rm_nodes = m3l_rm_list(2, List, Popt);
		}
		else if	(Popt != NULL && Popt->opt_linkscleanemptrefs == 'c'){
			rm_nodes = rm_nodes + m3l_ln_cleanemptylinksref(List, Popt);
		}

		return rm_nodes;
		
	}
	else
	{
/*
 * recursive calling, go to the first child node and loop over list in the same level as child
 */
		Tmpnode = (*List)->child;
		cleaned_nodes = 0;
		
		while(Tmpnode != NULL){
			Tmpnode1 = Tmpnode->next;
			cleaned_nodes = cleaned_nodes + m3l_ln_cleanemptylinks(&Tmpnode,  Popt);
			if(Popt != NULL && Popt->opt_linkscleanemptrefs == 'c') cleaned_nodes = cleaned_nodes + m3l_ln_cleanemptylinksref(&Tmpnode, Popt);
			Tmpnode = Tmpnode1;
		}
		return cleaned_nodes;
	}
	return -1;
}



lmint_t m3l_ln_cleanemptylinksref(node_t **List,  opts_t *Popt){
/*
 * function cleans empty references in  
 * (*Slist)->linknode structure
 */
	size_t i, counter, old_counter;
	counter = 0;
	
	find_str_t **TMP;
	
	return 0;
	
	if( (*List) == NULL){
		Warning("ln_cleanemptylinksref: Empty node");
		return -1;
	}
/*
 * find how many nodes uses this node as a link
 * discard NULL links
 */
	if((*List)->lcounter > 0 ){
		old_counter = (*List)->lcounter;
		for(i=0; i< (*List)->lcounter; i++)
			if( (*List)->linknode[i]->List != NULL)counter++;
	}
	else{
/*
 * no link info exist
 */
		return 0;
	}

	if(counter > 0){
		
		if( (TMP = (find_str_t **)malloc( (*List)->lcounter * sizeof(find_str_t *))) == NULL)
			Perror("ln_cleanemptylinksref malloc");
/*
 * find number of non-NULL links
 */
		
		for(i=0; i< (*List)->lcounter; i++){
			TMP[i] = NULL;
			
			if( (*List)->linknode[i]->List != NULL){
/* 
 * link is not empty info
 * increase counter
 * allocate TMP field and save the content of (*List)->linknode[i]->List in it
 */
				if( (TMP[i] = (find_str_t *)malloc( sizeof(find_str_t))) == NULL)
					Perror("ln_cleanemptylinksref malloc");
				TMP[i]->List = (*List)->linknode[i]->List;
			}
		}
		
		if(counter < (*List)->lcounter){
/*
 * there were NULL nodes saved in the structure
 * but node is still linked to some of the original nodes
 */
	
			for(i=0; i< (*List)->lcounter; i++)
				free((*List)->linknode[i]);
			free((*List)->linknode);
			
			if( ((*List)->linknode = (find_str_t **)malloc( counter * sizeof(find_str_t *))) == NULL)
				Perror("ln_cleanemptylinksref malloc");
			
			for(i=0; i< counter; i++){
				if( ((*List)->linknode[i] = (find_str_t *)malloc( sizeof(find_str_t))) == NULL)
					Perror("ln_cleanemptylinksref malloc");
				
				(*List)->linknode[i]->List = 	TMP[i]->List ;
			}
			(*List)->lcounter = counter;
/*
 * free TMP
 */
			for(i=0; i< (*List)->lcounter; i++){
				if(TMP[i] != NULL)free(TMP[i]);
			}
			free(TMP);
			
			return 1;
			
		}
		else if(counter > (*List)->lcounter){
/*
 * counter is larger then (*List)->lcounter which is impossible
 * return error
 */
			return -2;
		}
		else{
/*
 * unspecified error, return -3
 */
			for(i=0; i< (*List)->lcounter; i++){
				if(TMP[i] != NULL)free(TMP[i]);
			}
			free(TMP);
			return -3;
		}
	}
	else if(counter == 0){
/*
 * link is not linked to any other node
 */
		for(i=0; i< (*List)->lcounter; i++){
			free((*List)->linknode[i]);
			if(TMP[i] != NULL)free(TMP[i]);
		}
		free((*List)->linknode);
		free(TMP);
		(*List)->lcounter = 0;
		return 1;
	}	
	
	return -1;
}
