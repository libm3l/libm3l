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
	size_t i;

	if(strncmp((*Lnode)->type,"DIR",3) != 0 && strncmp((*Lnode)->type,"LINK",4) != 0 ){
 /*
  * get the total size of field if multidimensional 
  */
		free((*Lnode)->fdim);
		(*Lnode)->fdim = NULL;

		
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
	
	if((*Lnode)->lcounter > 0){
		for (i=0; i<(*Lnode)->lcounter; i++)
			free((*Lnode)->linknode[i]);
		
		free((*Lnode)->linknode);
	}
		

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
	(*Lnode)->linknode = NULL;
	(*Lnode)->lcounter = 0;
 /*
  * Allocate pointers
  */
	if ( ( (*Lnode)->type  = (char *)malloc(MAX_TYPE_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");

	if ( ( (*Lnode)->name  = (char *)malloc(MAX_NAME_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");

	if(strncmp(TMPSTR.Type,"DIR",3) != 0){
		if ( ( (*Lnode)->fdim  = (size_t *)malloc(TMPSTR.ndim* sizeof(size_t))) == NULL)
			Perror("malloc");
	}
/*
 * allocating data in data union (if list is of DATA type)
 */   
	if( snprintf((*Lnode)->type, MAX_TYPE_LENGTH,"%s",TMPSTR.Type) < 0)
		Perror("snprintf");
	if( snprintf((*Lnode)->name, MAX_TYPE_LENGTH,"%s",TMPSTR.Name_Of_List) < 0)
		Perror("snprintf");
	(*Lnode)->ndim = TMPSTR.ndim;
/*
 * if not DIR type, allocate field
 */
	if(strncmp(TMPSTR.Type,"DIR",3) != 0){
 /*
  * get the total size of field if multidimensional 
  */
		tot_dim = 1;
		for(i=0; i<TMPSTR.ndim; i++){
			(*Lnode)->fdim[i] = TMPSTR.dim[i];
			tot_dim = tot_dim * TMPSTR.dim[i];
		}
		
		
		if (strncmp(TMPSTR.Type,"LD",2) == 0){  /* long double */
			if ( ( (*Lnode)->data.ldf  = (long double *)malloc(tot_dim*sizeof(long double))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"D",1) == 0){  /* double */
			if ( ( (*Lnode)->data.df  = (double *)malloc(tot_dim*sizeof(double))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"F",1) == 0){  /* float */
			if ( ( (*Lnode)->data.f  = (float *)malloc(tot_dim*sizeof(float))) == NULL)
				Perror("malloc");
		}
/*
 * chars
 */
		else if (strncmp(TMPSTR.Type,"SC",2) == 0){  /* signed char */
			if ( ( (*Lnode)->data.sc = (signed char *)malloc((tot_dim+1)*sizeof(signed char))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"UC",2) == 0){  /* unsigned char */
			if ( ( (*Lnode)->data.uc = (unsigned char *)malloc((tot_dim+1)*sizeof(unsigned char))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"C",1) == 0){  /* char */
			if ( ( (*Lnode)->data.c = (char *)malloc((tot_dim+1)*sizeof(char))) == NULL)
				Perror("malloc");
		}
/*
 * integers
 */
		else if(strncmp(TMPSTR.Type,"ULLI",4) == 0){  /* unsigned long long  int */
			if ( ( (*Lnode)->data.ulli = (unsigned long long int *)malloc(tot_dim*sizeof(unsigned long long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"SLLI",4) == 0){  /* signed long long  int */
			if ( ( (*Lnode)->data.slli = (signed long long int *)malloc(tot_dim*sizeof(signed long long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* long long int */
			if ( ( (*Lnode)->data.lli = (long long int *)malloc(tot_dim*sizeof(long long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"ULI",3) == 0){  /* unsigned long int */
			if ( ( (*Lnode)->data.uli = (unsigned long int *)malloc(tot_dim*sizeof(unsigned long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"USI",3) == 0){  /* unsigned short int */
			if ( ( (*Lnode)->data.usi = (unsigned short int *)malloc(tot_dim*sizeof(unsigned short int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"SI",2) == 0){  /* short int */
			if ( ( (*Lnode)->data.si = (short int *)malloc(tot_dim*sizeof(short int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"UI",2) == 0){  /* unsigned int */
			if ( ( (*Lnode)->data.ui = (unsigned int *)malloc(tot_dim*sizeof(unsigned int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"LI",2) == 0){  /* long  int */
			if ( ( (*Lnode)->data.li = (long int *)malloc(tot_dim*sizeof(long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"I",1) == 0){  /* int */
			if ( ( (*Lnode)->data.i = (int *)malloc(tot_dim*sizeof(int))) == NULL)
				Perror("malloc");
		}
/*
 * counters
 */
		else if(strncmp(TMPSTR.Type,"ST",2) == 0){  /* size_t */
			if ( ( (*Lnode)->data.st = (size_t *)malloc(tot_dim*sizeof(size_t))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"PTRDF",1) == 0){  /* ptrdf_t */
			if ( ( (*Lnode)->data.ptrdf = (ptrdiff_t *)malloc(tot_dim*sizeof(ptrdiff_t))) == NULL)
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
	Lnode->linknode = NULL;
	Lnode->lcounter = 0;
 /*
  * Allocate pointers
  */
	if ( ( Lnode->type  = (char *)malloc(MAX_TYPE_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");

	if ( ( Lnode->name  = (char *)malloc(MAX_NAME_LENGTH* sizeof(char))) == NULL)
		Perror("malloc");
 
	if(strncmp(TMPSTR.Type,"DIR",3) != 0){
		if ( ( Lnode->fdim  = (size_t *)malloc(TMPSTR.ndim* sizeof(size_t))) == NULL)
			Perror("malloc");
	}
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
  * NOTE - when allocating sc,uc,c fields, allocate 1 element more and put it explicitely '\0' during reading
  */
		tot_dim = 1;
		for(i=0; i<TMPSTR.ndim; i++){
			Lnode->fdim[i] = TMPSTR.dim[i];
			tot_dim = tot_dim * TMPSTR.dim[i];
		}

		
		if (strncmp(Lnode->type,"LD",2) == 0){  /* long double */
			if ( ( Lnode->data.ldf  = (long double *)malloc(tot_dim*sizeof(long double))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"D",1) == 0){  /* double */
			if ( ( Lnode->data.df  = (double *)malloc(tot_dim*sizeof(double))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"F",1) == 0){  /* float */
			if ( ( Lnode->data.f  = (float *)malloc(tot_dim*sizeof(float))) == NULL)
				Perror("malloc");
		}
/*
 * chars
 */
		else if (strncmp(Lnode->type,"SC",2) == 0){  /* signed char */
			if ( ( Lnode->data.sc = (signed char *)malloc((tot_dim+1)*sizeof(signed char))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"UC",2) == 0){  /* unsigned char */
			if ( ( Lnode->data.uc = (unsigned char *)malloc((tot_dim+1)*sizeof(unsigned char))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"C",1) == 0){  /* char */
			if ( ( Lnode->data.c = (char *)malloc((tot_dim+1)*sizeof(char))) == NULL)
				Perror("malloc");
		}
/*
 * integers
 */
		else if(strncmp(Lnode->type,"ULLI",4) == 0){  /* unsigned long long  int */
			if ( ( Lnode->data.ulli = (unsigned long long int *)malloc(tot_dim*sizeof(unsigned long long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"SLLI",4) == 0){  /* signed long long  int */
			if ( ( Lnode->data.slli = (signed long long int *)malloc(tot_dim*sizeof(signed long long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* long long int */
			if ( ( Lnode->data.lli = (long long int *)malloc(tot_dim*sizeof(long long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"ULI",3) == 0){  /* unsigned long int */
			if ( ( Lnode->data.uli = (unsigned long int *)malloc(tot_dim*sizeof(unsigned long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"USI",3) == 0){  /* unsigned short int */
			if ( ( Lnode->data.usi = (unsigned short int *)malloc(tot_dim*sizeof(unsigned short int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"SI",2) == 0){  /* short int */
			if ( ( Lnode->data.si = (short int *)malloc(tot_dim*sizeof(short int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"UI",2) == 0){  /* unsigned int */
			if ( ( Lnode->data.ui = (unsigned int *)malloc(tot_dim*sizeof(unsigned int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"LI",2) == 0){  /* long  int */
			if ( ( Lnode->data.li = (long int *)malloc(tot_dim*sizeof(long int))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"I",1) == 0){  /* int */
			if ( ( Lnode->data.i = (int *)malloc(tot_dim *sizeof(int))) == NULL)
				Perror("malloc");
		}
/*
 * counters
 */
		else if(strncmp(Lnode->type,"ST",2) == 0){  /* size_t */
			if ( ( Lnode->data.st = (size_t *)malloc(tot_dim*sizeof(size_t))) == NULL)
				Perror("malloc");
		}
		else if(strncmp(Lnode->type,"PTRDF",1) == 0){  /* ptrdf_t */
			if ( ( Lnode->data.ptrdf = (ptrdiff_t *)malloc(tot_dim*sizeof(ptrdiff_t))) == NULL)
				Perror("malloc");
		}
	}
	return Lnode;
}
