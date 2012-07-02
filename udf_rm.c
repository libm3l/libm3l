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
 *     contact: clibm3l@gmail.com
 * 
 */



/*
 *     Function udf_rm.c
 *
 *     Author: Adam Jirasek
 *     Date: 2012-07-01
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

#include "FunctionsPrt.h"
#include "udf_rm.h"


int Free(node_t **Lnode)
{
/*
 * NOTE: This routine would not need to be necessarily written 
 *       having pointer-to-pointer argument if it ONLY freed the pointer!
 *       as freeing pointer does not modify its value
  *      http://cboard.cprogramming.com/c-programming/66209-struct-pointer-memory-allocation.html
 */
	size_t i;
	
	if( Free_data_str(Lnode) != 0)
		Perror("Free_data_str");

	if((*Lnode)->type != NULL) {
		free((*Lnode)->type);
		(*Lnode)->type == NULL;
	}  

	if((*Lnode)->name != NULL) {
		free((*Lnode)->name);
		(*Lnode)->name == NULL;
	}
/*
 * nullify node_t
 */
	(*Lnode)->next=NULL; (*Lnode)->prev=NULL; (*Lnode)->parent=NULL; (*Lnode)->child=NULL;  (*Lnode)->fdim=NULL; 
/*
 * free filed of lists pointing to links
 */
	if((*Lnode)->lcounter > 0){
		for (i=0; i<(*Lnode)->lcounter; i++)
			free((*Lnode)->linknode[i]);
		
		free((*Lnode)->linknode);
	}

	free(*Lnode);
	(*Lnode) = NULL;
		
	return 0;
}



int Free_data_str(node_t **Lnode)
{
	if(strncmp((*Lnode)->type,"DIR",3) != 0 && strncmp((*Lnode)->type,"LINK",4) != 0 ){
 /*
  * get the total size of field if multidimensional 
  */
		free((*Lnode)->fdim);
		(*Lnode)->fdim = NULL;
		(*Lnode)->ndim = 0;
/*
 * if data field was nota malloced and used only as a pointer, 
 * do not free
 */
		if((*Lnode)->no_malloc == 'n') return 0;
		
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
	return 0;
}


node_t *AllocateNode(tmpstruct_t TMPSTR, opts_t *Popt){
/*
 * function allocates node 
 *
 * allocate the main node
 */
	node_t *Lnode=NULL;
/*
 * allocate Lnode and 
 */
	if ( (Lnode  = (node_t *)malloc(sizeof(node_t))) == NULL)
		Perror("malloc");
  /*
  * nullify all node_t pointes in the node_t list
  */
	Lnode->next=NULL; Lnode->prev=NULL; Lnode->parent=NULL; Lnode->child=NULL;  Lnode->fdim=NULL;
	Lnode->type=NULL; Lnode->name=NULL;
	Lnode->linknode = NULL;
	Lnode->lcounter = 0;
	
	Lnode->no_malloc = '\0';
 /*
  * Allocate pointers
  */
	if ( ( Lnode->type  = (char *)malloc(MAX_TYPE_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");

	if ( ( Lnode->name  = (char *)malloc(MAX_NAME_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");
/*
 * allocating data in data union (if list is of DATA type)
 */   
	if( snprintf(Lnode->type, MAX_TYPE_LENGTH,"%s",TMPSTR.Type) < 0)
		Perror("snprintf");

	if( snprintf(Lnode->name, MAX_TYPE_LENGTH,"%s",TMPSTR.Name_Of_List) < 0)
		Perror("snprintf");
/*
 * filling Lnode->ndim, if List is not DIR, this will be re-filled again with the same value of AllocateNodeData
 */
	Lnode->ndim = TMPSTR.ndim;
/*
 * if not DIR type, allocate field
 */
	if(strncmp(TMPSTR.Type,"DIR",3) != 0 && strncmp(TMPSTR.Type,"LINK",4) != 0 ){
		if( AllocateNodeData(&Lnode, TMPSTR, Popt) != 0)
			Error("AllocateNodeData");
	}
	return Lnode;
}


int AllocateNodeData(node_t **Lnode, tmpstruct_t TMPSTR, opts_t *Popt)
{
 /*
  * get the total size of field if multidimensional 
  * NOTE - when allocating sc,uc,c fields, allocate 1 element more and put it explicitely '\0' during reading
  */
	size_t i, tot_dim;
	
	if(strncmp(TMPSTR.Type,"DIR",3) != 0 && strncmp(TMPSTR.Type,"LINK",4) != 0 ){
		if ( ( (*Lnode)->fdim  = (size_t *)malloc(TMPSTR.ndim* sizeof(size_t))) == NULL)
			Perror("malloc");
	}
/*
 * filling Lnode->ndim, if List is not DIR, this has already been filled with in AllocateNode
 */
	(*Lnode)->ndim = TMPSTR.ndim;
	
	tot_dim = 1;
	for(i=0; i<TMPSTR.ndim; i++){
		(*Lnode)->fdim[i] = TMPSTR.dim[i];
		tot_dim = tot_dim * TMPSTR.dim[i];
	}
/*
 * if not required to malloc field, return now
 */	
	if(Popt->opt_m == 'm'){
		(*Lnode)->no_malloc = 'n';
		return 0;
	}
		
	if (strncmp((*Lnode)->type,"LD",2) == 0){  /* long double */
		if ( ( (*Lnode)->data.ldf  = (long double *)malloc(tot_dim*sizeof(long double))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"D",1) == 0){  /* double */
		if ( ( (*Lnode)->data.df  = (double *)malloc(tot_dim*sizeof(double))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"F",1) == 0){  /* float */
		if ( ( (*Lnode)->data.f  = (float *)malloc(tot_dim*sizeof(float))) == NULL)
			Perror("malloc");
	}
/*
 * chars
 */
	else if (strncmp((*Lnode)->type,"SC",2) == 0){  /* signed char */
		if ( ( (*Lnode)->data.sc = (signed char *)malloc((tot_dim+1)*sizeof(signed char))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"UC",2) == 0){  /* unsigned char */
		if ( ( (*Lnode)->data.uc = (unsigned char *)malloc((tot_dim+1)*sizeof(unsigned char))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"C",1) == 0){  /* char */
		if ( ( (*Lnode)->data.c = (char *)malloc((tot_dim+1)*sizeof(char))) == NULL)
			Perror("malloc");
	}
/*
 * integers
 */
	else if(strncmp((*Lnode)->type,"ULLI",4) == 0){  /* unsigned long long  int */
		if ( ( (*Lnode)->data.ulli = (unsigned long long int *)malloc(tot_dim*sizeof(unsigned long long int))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"SLLI",4) == 0){  /* signed long long  int */
		if ( ( (*Lnode)->data.slli = (signed long long int *)malloc(tot_dim*sizeof(signed long long int))) == NULL)
			Perror("malloc");
	}
	else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* long long int */
		if ( ( (*Lnode)->data.lli = (long long int *)malloc(tot_dim*sizeof(long long int))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"ULI",3) == 0){  /* unsigned long int */
		if ( ( (*Lnode)->data.uli = (unsigned long int *)malloc(tot_dim*sizeof(unsigned long int))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"USI",3) == 0){  /* unsigned short int */
		if ( ( (*Lnode)->data.usi = (unsigned short int *)malloc(tot_dim*sizeof(unsigned short int))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"SI",2) == 0){  /* short int */
		if ( ( (*Lnode)->data.si = (short int *)malloc(tot_dim*sizeof(short int))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"UI",2) == 0){  /* unsigned int */
		if ( ( (*Lnode)->data.ui = (unsigned int *)malloc(tot_dim*sizeof(unsigned int))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"LI",2) == 0){  /* long  int */
		if ( ( (*Lnode)->data.li = (long int *)malloc(tot_dim*sizeof(long int))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"I",1) == 0){  /* int */
		if ( ( (*Lnode)->data.i = (int *)malloc(tot_dim *sizeof(int))) == NULL)
			Perror("malloc");
	}
/*
 * counters
 */
	else if(strncmp((*Lnode)->type,"ST",2) == 0){  /* size_t */
		if ( ( (*Lnode)->data.st = (size_t *)malloc(tot_dim*sizeof(size_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"PTRDF",1) == 0){  /* ptrdf_t */
		if ( ( (*Lnode)->data.ptrdf = (ptrdiff_t *)malloc(tot_dim*sizeof(ptrdiff_t))) == NULL)
			Perror("malloc");
	}
	
	return 0;

}
