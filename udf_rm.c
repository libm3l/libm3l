/*
 * function frees memory of the node 
 * copyright � 2012 Adam Jirasek
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
	if(strncmp((*Lnode)->type,"DIR",3) != 0){
 /*
  * get the total size of field if multidimensional 
  */
		free((*Lnode)->fdim);
		(*Lnode)->fdim = NULL;

		if (strncmp((*Lnode)->type,"IL",2) == 0){
			free( (*Lnode)->data.df);
			(*Lnode)->data.df = NULL;
		}
  
		if (strncmp((*Lnode)->type,"I",1) == 0){
			free( (*Lnode)->data.i);
			(*Lnode)->data.i = NULL;
		}
			
		if (strncmp((*Lnode)->type,"F",1) == 0){
			free ( (*Lnode)->data.f );
			(*Lnode)->data.f = NULL;
		}
	
		if (strncmp((*Lnode)->type,"D",1) == 0){
			free( (*Lnode)->data.df);
			(*Lnode)->data.df = NULL;
		}
	}

	if((*Lnode)->type != NULL) {
		free((*Lnode)->type);
		(*Lnode)->type == NULL;
	}  

	if((*Lnode)->name != NULL) {
		free((*Lnode)->name);
		(*Lnode)->name == NULL;
	}

	(*Lnode)->next=NULL; (*Lnode)->prev=NULL; (*Lnode)->parent=NULL; (*Lnode)->child=NULL;  (*Lnode)->fdim=NULL;

	free(*Lnode);
	(*Lnode) = NULL;
		
	return 0;
}



int Allocate(node_t **Lnode, tmpstruct_t TMPSTR)
/*
 * function allocates node 
 */
{
  
/*
 * allocate the main node
 */
	size_t i, tot_dim;
/*
 * allocate Lnode and 
 */
	if ( (*Lnode  = (node_t *)malloc(sizeof(node_t))) == NULL)
		Perror("malloc");
  /*
  * nullify all node_t pointes in the node_t list
  */
 	(*Lnode)->next=NULL; (*Lnode)->prev=NULL; (*Lnode)->parent=NULL; (*Lnode)->child=NULL;  (*Lnode)->fdim=NULL;
	(*Lnode)->type=NULL; (*Lnode)->name=NULL;
 /*
  * Allocate pointers
  */
	if ( ( (*Lnode)->type  = (char *)malloc(MAX_TYPE_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");

	if ( ( (*Lnode)->name  = (char *)malloc(MAX_NAME_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");

	if ( ( (*Lnode)->fdim  = (size_t *)malloc(TMPSTR.ndim* sizeof(size_t))) == NULL)
		Perror("malloc");
/*
 * allocating data in data union (if list is of DATA type)
 */   
	if( snprintf((*Lnode)->type, MAX_TYPE_LENGTH,"%s",TMPSTR.Type) < 0)
		Perror("snprintf");
	if( snprintf((*Lnode)->name, MAX_TYPE_LENGTH,"%s",TMPSTR.Name_Of_List) < 0)
		Perror("snprintf");
	(*Lnode)->ndim = TMPSTR.ndim;

	printf("%s -> %s -> %ld\n", (*Lnode)->type,(*Lnode)->name,(*Lnode)->ndim);

/*
 * if not DIR type, allocate field
 */
	if(strncmp(TMPSTR.Type,"DIR",3) != 0){
		printf("ALLOCATE - allocating data for data.union\n");
 /*
  * get the total size of field if multidimensional 
  */
		if(TMPSTR.ndim > 1){
			tot_dim = 1;
			for(i=0; i<TMPSTR.ndim; i++){
				(*Lnode)->fdim[i] = TMPSTR.dim[i];
				tot_dim = tot_dim * TMPSTR.dim[i];
			}
		}
		else{
			(*Lnode)->fdim[0] = TMPSTR.dim[0];
			tot_dim = TMPSTR.dim[0];
		}
 
		printf("Total dimension is %ld\n", tot_dim);

		if (strncmp(TMPSTR.Type,"IL",2) == 0){
			printf("Long I type \n");
			if ( ( (*Lnode)->data.df  = (double *)malloc(tot_dim*sizeof(double))) == NULL)
			Perror("malloc");
		}
		else if(*TMPSTR.Type == 'I'){
			printf("Intereg  %s\n", TMPSTR.Type);
			if ( ( (*Lnode)->data.i  = (int *)malloc(tot_dim*sizeof(int))) == NULL)
			Perror("malloc");
		}
		else if (*TMPSTR.Type == 'F'){
			printf("Float %s\n", TMPSTR.Type);
			if ( ( (*Lnode)->data.f  = (float *)malloc(tot_dim*sizeof(float))) == NULL)
				Perror("malloc");
		}
		else if (*TMPSTR.Type == 'D'){
			printf("Double %s\n", TMPSTR.Type);
			if ( ( (*Lnode)->data.df  = (double *)malloc(tot_dim*sizeof(double))) == NULL)
          			Perror("malloc");
		}
	}
	return 0;
}

node_t *AllocateNode(tmpstruct_t TMPSTR)
/*
 * function allocates node 
 */
{
/*
 * allocate the main node
 */
	node_t *Lnode=NULL;
	size_t i, tot_dim;
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
 /*
  * Allocate pointers
  */
	if ( ( Lnode->type  = (char *)malloc(MAX_TYPE_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");

	if ( ( Lnode->name  = (char *)malloc(MAX_NAME_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");
 
	if ( ( Lnode->fdim  = (size_t *)malloc(TMPSTR.ndim* sizeof(size_t))) == NULL)
		Perror("malloc");
/*
 * allocating data in data union (if list is of DATA type)
 */   
	if( snprintf(Lnode->type, MAX_TYPE_LENGTH,"%s",TMPSTR.Type) < 0)
		Perror("snprintf");

	if( snprintf(Lnode->name, MAX_TYPE_LENGTH,"%s",TMPSTR.Name_Of_List) < 0)
		Perror("snprintf");
	Lnode->ndim = TMPSTR.ndim;
/*
 * if not DIR type, allocate field
 */
	if(strncmp(TMPSTR.Type,"DIR",3) != 0){
 /*
  * get the total size of field if multidimensional 
  */
		if(TMPSTR.ndim > 1){
			tot_dim = 1;
			for(i=0; i<TMPSTR.ndim; i++){
				Lnode->fdim[i] = TMPSTR.dim[i];
				tot_dim = tot_dim * TMPSTR.dim[i];
			}
		}
		else
		{
			Lnode->fdim[0] = TMPSTR.dim[0];
			tot_dim = TMPSTR.dim[0];
		}

		printf("Total dimension is %ld\n", tot_dim);

		if (strncmp(TMPSTR.Type,"IL",2) == 0){
			printf("Long I type \n");
			if ( ( Lnode->data.df  = (double *)malloc(tot_dim*sizeof(double))) == NULL)
				Perror("malloc");
		}
		else if(*TMPSTR.Type == 'I')
		{
			printf("Intereg  %s\n", TMPSTR.Type);
			if ( ( Lnode->data.i  = (int *)malloc(tot_dim*sizeof(int))) == NULL)
				Perror("malloc");
		}
		else if (*TMPSTR.Type == 'F')
		{
			printf("Float %s\n", TMPSTR.Type);
			if ( ( Lnode->data.f  = (float *)malloc(tot_dim*sizeof(float))) == NULL)
				Perror("malloc");
		}
		else if (*TMPSTR.Type == 'D')
		{
			printf("Double %s\n", TMPSTR.Type);
			if ( ( Lnode->data.df  = (double *)malloc(tot_dim*sizeof(double))) == NULL)
				Perror("malloc");
		}
	}
	return Lnode;
}