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
 *     Function mv_list.c
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

#include "mv_list.h"
#include "add_list.h"
#include "locate_list.h"
#include "udf_rm.h"
#include "FunctionsPrt.h"
#include "find_list.h"
#include "rm_list.h"

static lmint_t m3l_mv_list(lmint_t , node_t **, node_t **, opts_t *);
/*
 * function moves list.
 */

lmsize_t m3l_mv_(node_t **SList, const lmchar_t *s_path, const lmchar_t *s_path_loc, node_t **TList, const lmchar_t *t_path, const lmchar_t *t_path_loc, opts_t *Popts)
{
/*
 * function is a  of the mv function
 */

/* NOTE - check that Tfounds and SFounds are identical */
 
	lmsize_t i, j,k,l,mv_tot_nodes, mv_nodes,len;
	find_t *SFounds, *TFounds;
	lmint_t init_call;
 	lmchar_t *name, *path, *path_loc, *newname;
	const lmchar_t *pc;
	opts_t *Popts_Tlist, opts;
	
	init_call = 1;
/*
 * check if data set exists
 */
	if(*SList == NULL){
		Warning("mv: NULL source list");
		return -1;
	}

	if(*TList == NULL){
		Warning("mv: NULL target list");
		return -1;
	}
/*
 * call locator to locate the target node
 */
	if ( (SFounds = m3l_locate( *SList, s_path, s_path_loc, Popts)) == NULL){
		Warning("mv: NULL SFounds");
		return 0;
	}
/*
 * set default values of options for TList
 */
	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_k = '\0'; opts.opt_b = '\0';opts.opt_l = '\0';
	Popts_Tlist = &opts;
/*
 * check only one node is to be moved to the same directory  (ie. path is only ./ (dotslash)
 */
	len = strlen(t_path_loc);
	if(strncmp(t_path_loc, "./", 2) == 0 && len == 2){
		for(i=0; i< SFounds->founds; i++){
			name = SFounds->Found_Nodes[i]->List->name;
			bzero(name, sizeof(name));
			if( snprintf(name,MAX_NAME_LENGTH,"%s",t_path) < 0)
				Perror("snprintf");
		}
		i = SFounds->founds;
		m3l_DestroyFound(&SFounds);
		return i;
	}
	else{
/*
 * locate target; if target == NULL, just rename the node(s) or --add option specified 
 */
		if ( Popts->opt_add == 'a' || (TFounds = m3l_locate( *TList, t_path, t_path_loc, Popts_Tlist)) == NULL){
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
				if ( (TFounds = m3l_locate( *TList, path, path_loc, Popts_Tlist)) == NULL){
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
	
				mv_tot_nodes = 0;
				for(i=0; i< SFounds->founds; i++){
/*
 * change the name of the listPopts_TList
 */
					name = SFounds->Found_Nodes[i]->List->name;
					bzero(name, sizeof(name));
					if( snprintf(name,MAX_NAME_LENGTH,"%s",newname) < 0)
						Perror("snprintf");
/*
 * check if source and target dirs are different, if different, move list to a new location
 */
					if( SFounds->Found_Nodes[i]->List->parent != TFounds->Found_Nodes[0]->List){
	
						if(SFounds->Found_Nodes[i]->List == TFounds->Found_Nodes[0]->List){
							Warning("mv_list: can not move node to itself");
						}
						else{
							if( (mv_nodes = (lmsize_t) m3l_mv_list(init_call, &SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  Popts )) < 0){
								Warning("problem in mv_list");
							}
							else{
								mv_tot_nodes += mv_nodes;
							}
						}
					}
					else{
						mv_tot_nodes++;
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
				return mv_tot_nodes;
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
				Warning("mv: multiple target nodes");
				m3l_DestroyFound(&TFounds);
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
	
			mv_tot_nodes = 0;
				
			for(i=0; i< SFounds->founds; i++){
					
				if(SFounds->Found_Nodes[i]->List == TFounds->Found_Nodes[0]->List){
					Warning("mv_list: can not move node to itself");
				}

				else{
					if( (mv_nodes = (lmsize_t) m3l_mv_list(init_call, &SFounds->Found_Nodes[i]->List, &TFounds->Found_Nodes[0]->List,  Popts )) < 0){
						Warning("problem in mv_list");
					}
					else{
						mv_tot_nodes += mv_nodes;
					}
				}
			}
					
			m3l_DestroyFound(&TFounds);
			m3l_DestroyFound(&SFounds);
			return mv_tot_nodes;
		}
	}
}



lmint_t m3l_mv_list(lmint_t call, node_t **SList, node_t **TList, opts_t *Popts)
{
/*
 * function moves list SList to the target list TList
 * 	if the SList is FILE, TLIST has to be file too. The data union and ->fdim of the TList target nodes is furst freed, then reallocated
 *	and then SList data union + ->fdim and ->ndim are copied.
 * 	if the TList is DIR the Slist is re-created and added to the TList. In case SList is a DIR, the copying is done by 
 *	traversing the list and copying it item-by-item
 */
	node_t *Prev, *Par, *Next;
	node_t *TPrev, *TPar, *TNext;
/*
 * save neighbours of the Slist
 */
		Next = (*SList)->next;
		Prev = (*SList)->prev;
		Par  = (*SList)->parent;
/*
 * copy source (Slist) to target (Tlist)
 */
	if(strncmp( (*TList)->type, "DIR", 3) != 0){  
/*
 * FILE to FILE -> keep original (SList) pointer, update name using TList name and remove TList name
 */
/*
 * *Tlist is FILE, check that SList is not DIR
 */
		if(strncmp( (*SList)->type, "DIR", 3) == 0){
			Warning(" cp_list: cannot overwrite non-DIR  with DIR ");
			return -1;
		}
/*
 * Save neighbours of the Tlist
 */
		TNext = (*TList)->next;
		TPrev = (*TList)->prev;
		TPar  = (*TList)->parent;
		
		bzero((*SList)->name, sizeof((*SList)->name));
		if( snprintf((*SList)->name, sizeof((*SList)->name),"%s", (*TList)->name) < 0){
			Perror("snprintf");
			return -1;
		}
/*
 * remove TList
 */		
		if ( m3l_rm_list(1, TList, Popts) < 1 ){
			Error("mv_list: rm_list");
			return -1;
		}
/*
 * Place Slist where TList was before
 */
		(*SList)->next    = TNext;
		(*SList)->prev    = TPrev;
		(*SList)->parent  = TPar;
		
		if((*SList)->next != NULL) TNext->prev = (*SList);
		if((*SList)->prev != NULL) TPrev->next = (*SList);
/*
 * if moved node is in the same directory as target node
 * dicrease number of items in the directory
 */
 		if(Par != TPar)
			TPar->ndim++;
	}
	else{
/*
 * add a new node to the DIR list
 */
		if ( m3l_add_list(SList, TList, Popts) < 0){
			Warning("Error mv_list copy");
			return -1;
		}
	}
/*
 * If the SList was removed, connect the list chain (after SList was removed)
 * if Slist has a parent, dicrease number of nodes in it
 * This will not happen if Slist was a head node, ie. its Par == NULL
 */
	if(Par != NULL){
		Par->ndim--;
		if(Par->ndim > 0){
/*
 * still nodes in directory
 */		
			if(Next != NULL && Prev != NULL){
/*
 * List is in the middle
 */
				Prev->next = Next;
				Next->prev = Prev;
			}
			else if(Next == NULL){
/*
 * List is the last in line
 */
				Prev->next = NULL;
// 			Par->child = NULL;
// 			Par->ndim = 0;
			}
			else if(Prev == NULL){
/*
 * List is the fisrt in line
 */
				Next->prev = NULL;
				Par->child = Next;
			}
		}
		else{
/*
 * after move, DIR is empty
 */
			Par->child = NULL;
		}
	}
	return 1;
}


