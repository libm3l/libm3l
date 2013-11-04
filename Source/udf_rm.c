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
 *     Function udf_rm.c
 *
 *     Date: 2012-07-01
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

#include "FunctionsPrt.h"
#include "udf_rm.h"


lmint_t m3l_Free(node_t **Lnode)
{
/*
 * NOTE: This routine would not need to be necessarily written 
 *       having pointer-to-pointer argument if it ONLY freed the pointer!
 *       as freeing pointer does not modify its value
  *      http://cboard.cprogramming.com/c-programming/66209-struct-pointer-memory-allocation.html
 */
	lmsize_t i;
	
	if( m3l_Free_data_str(Lnode) != 0)
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
/*
 * notify links about this node being freed (set their ->child to NULL
 */				
// 			(*Lnode)->linknode[i]->List->child = NULL;
/*
 * free linknode memory
 */	
			free((*Lnode)->linknode[i]);
		
		free((*Lnode)->linknode);
	}

	free(*Lnode);
	(*Lnode) = NULL;
		
	return 0;
}



lmint_t m3l_Free_data_str(node_t **Lnode)
{
	if(strncmp((*Lnode)->type,"DIR",3) != 0 && strncmp((*Lnode)->type,"LINK",4) != 0 ){
 /*
  * get the total size of field if multidimensional 
  */
		free((*Lnode)->fdim);
		(*Lnode)->fdim = NULL;
		(*Lnode)->ndim = 0;
/*
 * if data field was not malloced and used only as a pointer, 
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


node_t *m3l_AllocateNode(tmpstruct_t TMPSTR, opts_t *Popt){
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
	if ( ( Lnode->type  = (lmchar_t *)malloc(MAX_TYPE_LENGTH* sizeof(lmchar_t))) == NULL)
		Perror("malloc");

	if ( ( Lnode->name  = (lmchar_t *)malloc(MAX_NAME_LENGTH* sizeof(lmchar_t))) == NULL)
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
		if( m3l_AllocateNodeData(&Lnode, TMPSTR, Popt) != 0)
			Error("AllocateNodeData");
	}
	return Lnode;
}


lmint_t m3l_AllocateNodeData(node_t **Lnode, tmpstruct_t TMPSTR, opts_t *Popt)
{
 /*
  * get the total size of field if multidimensional 
  * NOTE - when allocating sc,uc,c fields, allocate 1 element more and put it explicitely '\0' during reading
  */
	lmsize_t i, tot_dim;
	
	if(strncmp(TMPSTR.Type,"DIR",3) != 0 && strncmp(TMPSTR.Type,"LINK",4) != 0 ){
		if ( ( (*Lnode)->fdim  = (lmsize_t *)malloc(TMPSTR.ndim* sizeof(lmsize_t))) == NULL)
			Perror("malloc");
	}
/*
 * filling Lnode->ndim, if List is not DIR, this has already been filled with in AllocateNode
 */
	if(   ( (*Lnode)->ndim = TMPSTR.ndim) < 1 )
		Error("udf_rm: too low value of number of dimensions");
	
	tot_dim = 1;
	for(i=0; i<TMPSTR.ndim; i++){
		if ( (   (*Lnode)->fdim[i] = TMPSTR.dim[i]  ) < 1 )
			Error("udf_rm: too low value of dimensions");
		tot_dim = tot_dim * TMPSTR.dim[i];
	}
/*
 * if not required to malloc field, return now
 */	
	if(Popt != NULL && Popt->opt_nomalloc == 'm'){
		(*Lnode)->no_malloc = 'n';
		return 0;
	}
		
	if (strncmp((*Lnode)->type,"LD",2) == 0){  /* long double */
		if ( ( (*Lnode)->data.ldf  = (lmlongdouble_t *)malloc(tot_dim*sizeof(lmlongdouble_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"D",1) == 0){  /* double */
		if ( ( (*Lnode)->data.df  = (lmdouble_t *)malloc(tot_dim*sizeof(lmdouble_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"F",1) == 0){  /* float */
		if ( ( (*Lnode)->data.f  = (lmfloat_t *)malloc(tot_dim*sizeof(lmfloat_t))) == NULL)
			Perror("malloc");
	}
/*
 * chars
 */
	else if (strncmp((*Lnode)->type,"SC",2) == 0){  /* signed char */
		if ( ( (*Lnode)->data.sc = (lmsignchar_t *)malloc((tot_dim+1)*sizeof(lmsignchar_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"UC",2) == 0){  /* unsigned char */
		if ( ( (*Lnode)->data.uc = (lmusignchar_t *)malloc((tot_dim+1)*sizeof(lmusignchar_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"C",1) == 0){  /* char */
		if ( ( (*Lnode)->data.c = (lmchar_t *)malloc((tot_dim+1)*sizeof(lmchar_t))) == NULL)
			Perror("malloc");
	}
/*
 * integers
 */
	else if(strncmp((*Lnode)->type,"ULLI",4) == 0){  /* unsigned long long  int */
		if ( ( (*Lnode)->data.ulli = (lmullint_t *)malloc(tot_dim*sizeof(lmullint_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"SLLI",4) == 0){  /* signed long long  int */
		if ( ( (*Lnode)->data.slli = (lmsllint_t *)malloc(tot_dim*sizeof(lmsllint_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"LLI",3) == 0){  /* long long int */
		if ( ( (*Lnode)->data.lli = (lmllint_t *)malloc(tot_dim*sizeof(lmllint_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"ULI",3) == 0){  /* unsigned long int */
		if ( ( (*Lnode)->data.uli = (lmulint_t *)malloc(tot_dim*sizeof(lmulint_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"USI",3) == 0){  /* unsigned short int */
		if ( ( (*Lnode)->data.usi = (lmushint_t *)malloc(tot_dim*sizeof(lmushint_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"SI",2) == 0){  /* short int */
		if ( ( (*Lnode)->data.si = (lmshint_t *)malloc(tot_dim*sizeof(lmshint_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"UI",2) == 0){  /* unsigned int */
		if ( ( (*Lnode)->data.ui = (lmuint_t *)malloc(tot_dim*sizeof(lmuint_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"LI",2) == 0){  /* long  int */
		if ( ( (*Lnode)->data.li = (lmlint_t *)malloc(tot_dim*sizeof(lmlint_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"I",1) == 0){  /* int */
		if ( ( (*Lnode)->data.i = (lmint_t *)malloc(tot_dim *sizeof(lmint_t))) == NULL)
			Perror("malloc");
	}
/*
 * counters
 */
	else if(strncmp((*Lnode)->type,"ST",2) == 0){  /* size_t */
		if ( ( (*Lnode)->data.st = (lmsize_t *)malloc(tot_dim*sizeof(lmsize_t))) == NULL)
			Perror("malloc");
	}
	else if(strncmp((*Lnode)->type,"PTRDF",1) == 0){  /* ptrdf_t */
		if ( ( (*Lnode)->data.ptrdf = (lmptrdiff_t *)malloc(tot_dim*sizeof(lmptrdiff_t))) == NULL)
			Perror("malloc");
	}
	
	return 0;

}
