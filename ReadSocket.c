/*
 * function frees memory of the node 
 * copyright � 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"
#include "format_conversion_spec.h"

#include "FunctionsPrt.h"
#include "udf_rm.h"
#include "ReadSocket.h"

#define EXPR      *pc != ' ' && *pc != '\t' && *pc != '\n' && *pc != SEPAR_SIGN && *pc != '\0'
#define IFEXPR     *pc == ' ' || *pc == '\t' || *pc == '\n' && *pc != '\0' || *pc == SEPAR_SIGN 
#define LASTEXPR   lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0' && lastchar != SEPAR_SIGN 


/* 
 * NOTE: these can be:  (Wite2Socket filters spaces, tabs and newlines)
#define EXPR      *pc != SEPAR_SIGN && *pc != '\0'
#define IFEXPR   *pc != '\0' || *pc == SEPAR_SIGN 
#define LASTEXPR   lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0' && lastchar != SEPAR_SIGN 
*/

static int read_socket_data_line(node_t **, tmpstruct_t, int);
static int read_socket_data_charline(node_t **, tmpstruct_t, int);
static node_t *read_socket_dir_data(tmpstruct_t , int);
static node_t *read_socket_data(int);

char *pc, buff[MAXLINE];
ssize_t ngotten;

/*
 * Function read just one line from a socket, disregarding comments line
 * It identifies if the line is a header of DATA or DIR list
 * If DATA type of list it allocates a node_t and calls ReadDataLine which reads the
 * actual conent of the list
 * If the list if of DIR type, it calls ReadDir routine which reads DIR types of list (recursive calling)
 * 
  * This function is a copy of ReadDescriptor, the only change is replacing the 
 * FILE * by int and fread by read and having SEPAR_SIGN as a separation sign between the 
 * words, although the algorithm should be able to take spaces, tabs and new lines too.
 * For C, UC and SC fields, spaces, \t and \n are taken as valid characters
 */ 
node_t *read_socket(int descrpt)
{
	char type[MAX_WORD_LENGTH], lastchar;
	size_t   wc, i, hi, tmpi;
	tmpstruct_t TMPSTR;
	node_t *Dnode;
/*
 * 1. -----------   read info about list (on one line)
 * 			Parameters are as follows:
 *                      DIRECTORY:
 * 				Name of list
 * 				DIR keyword
 * 				number of items in directory
 *                      DATA:
 * 				Name_of_list
 * 				Type of list
 * 				Number of dimensions if filed
 * 				Size of each dimension
 *
 * Example:   
 * 			Main_data_strucure DIR  5
 * 			Pressure   D    3     100 200 300
 */
/*
 * 1. -----------   read info about list (on one line)
 * 			Parameters are as follows:
 *                      DIRECTORY:
 * 				Name of list
 * 				DIR keyword
 * 				number of items in directory
 *                      DATA:
 * 				Name_of_list
 * 				Type of list
 * 				Number of dimensions if filed
 * 				Size of each dimension
 *
 * Example:   
 * 			Main_data_strucure DIR  5
 * 			Pressure   D    3     100 200 300
 */
/*
 * read MAXLINE-1, MAXLINE will be '\0', put pointer at the beginning of the fiield
 */
	bzero(buff, strlen(buff));
	if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1)
		Perror("read");

	buff[ngotten] = '\0';		
	pc = &buff[0];
/*
 * process the string, in case it returned anything
 */
	while(*pc != '\0') /*  while(ngotten) */
	{
		bzero(type,sizeof(type));
		i = 0;
		wc = 0;
		lastchar = '\0';
/*
 * read until the end of string
 */
		while(*pc != '\0'){
/*
 *avoid having empty spaces, tabs, newlines or end of buffer
 */
			while(EXPR){
				type[i++] = *pc++;
/*
 * if number of chars in one word exceeds limit, print warning
 */
				if(i == (MAX_WORD_LENGTH-1))
					Perror("read_socket - word too long");
			}
/*
 * set the last member of the string to \0
 */
			type[i] = '\0';
/*
 * save last character, if it is space, tab or \0 it means the buffer ended at end of the word
 * otherwise the buffer ended in the middle of word
  * if the last string in previuous while loop was the end of buff - \0 set indicator of it
 */
			if(*pc == '\0'){
				hi = 0;
			}
			else
			{
				hi = 1;
			}
/*
 * save the last character - if the last character was due to \0 in buffer, save the one before
 */
			if(i > 0 && *(pc+hi) == '\0') lastchar = *(pc+hi-1);

/*
 * if reached the end of buff
 */
			if ( *(pc+hi) == '\0'){
/*
 * read next chunk of text file, complete the word by the rest from next chunk and put pointer at it's beggining
 */
				bzero(buff,sizeof(buff));
				if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1)
					Perror("read");
				buff[ngotten] = '\0';

				pc = &buff[0];
/*
 * if last character was not space, tab, new line or \0 the buffer did not contain entire word, some of it's part is in the next buffer
 */
				if(LASTEXPR) continue;
			}
/*
 * if word is longer then 0
 */
			if(strlen(type) >0){
 				wc++;
/*
 * get the name of the list
 */
				if(wc == 1){
					bzero(TMPSTR.Name_Of_List,sizeof(TMPSTR.Name_Of_List));
					if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", type) < 0)
		   				Perror("snprintf");
				}
/*
 * get the type of the list
 */
				else if (wc == 2 ){
					bzero(TMPSTR.Type,sizeof(TMPSTR.Type));
					if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", type) < 0)
		   				Perror("snprintf");
				}
/*
 * get the number, in case of DIR number is a number of items in DIR, in case of DATA, number is a number of dimensions
 */
				else if (wc == 3){
					TMPSTR.ndim = Strol(type);  
					TMPSTR.dim=NULL;
/*
 * read data in DIR
 */

					if( (Dnode = read_socket_dir_data(TMPSTR, descrpt)) == NULL)
						Perror("ReadDirData - ReadDir");
/*
 * Return main list
 */
					while(IFEXPR) pc++;
					i = 0;
					while(EXPR){
						type[i++] = *pc++;
/*
 * if number of chars in one word exceeds limit, print warning
 */
						if(i == (MAX_WORD_LENGTH-1))
							Perror("read_socket - word too long");
					}
					type[i] = '\0';
/*
 * make sure that if the EOFbuff word is split and part of it is still in socket
 * read it and append to the beginning of word
 * 
 * compare the last word with EOFbuff, if not equal attempt to read rest if socket
 */
					if(strncmp(type,EOFbuff,strlen(EOFbuff))  != 0){
/*
 * read the last part of buffer and add it to the previuous word
 */
							bzero(buff,sizeof(buff));
							if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1)
								Perror("read");
							buff[ngotten] = '\0';
							if(ngotten > 0) strcat(type, buff);
/*
 * compare with EOFbuff, if not equal, give warning
 * NOTE: if from whatever reason it happens there are data after EOFbuff, maybe read them and print on screen, just to make
 * sure the socket is empty
 */
						if(strncmp(type,EOFbuff,strlen(EOFbuff))  != 0){
								tmpi = 0;
								printf("\n  WARNING - end of buffer not reached \n  Remaining part of the buffer starts at\n");
								while(*pc != '\0' && tmpi++ < 100)
									printf("%c", *pc++);
								printf("\n");
/*
 * if from whatever reason it happens there are data after EOFbuff, maybe read them and print on screen, just to make
 * sure the socket is empty
 */								
								while(*pc != '\0') /*  while(ngotten) */{
									bzero(buff,sizeof(buff));
									if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1)
										Perror("read");
								}
								exit(0); 
						}
/*
 * reading socket ended sucesfully, give back Gnode
 */
					}
					return Dnode;
				}
			}
	
/*
 * if emtpy spaces and new lines and seprar sight (SEPAR_SIGN) and not '\0' after the word, move pointer
 */
			if(IFEXPR) pc++;
/*
 * nullify type char, set counter of chars in word to 0 and set lastchar to \0
 */
			bzero(type,sizeof(type));
			i = 0;
			lastchar = '\0';
		}
 /*
  * end of reading the lists in DIR list   - while(ngotten)
  */
	}
/*
 * if you get here, something went wrong
 */
	return NULL;
}



/*
 * reads data after line identifying DIR
 */


node_t *read_socket_dir_data(tmpstruct_t TMPSTR, int descrpt)
{
	size_t i;
	node_t *Dnode, *Tmpnode, *Pnode;
 /*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
	if( (Dnode = AllocateNode(TMPSTR)) == NULL){
		Error("Allocate");
	}

	for(i=1;i<=TMPSTR.ndim; i++){ 
		Tmpnode=NULL;
		if ( (Tmpnode = read_socket_data(descrpt)) == NULL)
			Error("ReadDirData: ReadData");
/*
 * add to node
 */
		if(i ==1){
			Pnode         = Tmpnode;
			Dnode->child  = Pnode;
			Pnode->parent = Dnode;
		}
		else
		{
			Pnode->next      = Tmpnode;
			Tmpnode->prev    = Pnode;
			Tmpnode->parent  = Dnode;
			 Pnode            = Tmpnode;
		}
	}
	return Dnode;
}





node_t *read_socket_data(int descrpt)
{
	char type[MAX_WORD_LENGTH], lastchar;
	size_t   wc, i, hi;
	tmpstruct_t TMPSTR;
	node_t *Pnode;
	
	Pnode = NULL;
/*
 * process the string - the algorithm is the same as in ReadDir1, comments reduced
 */
	while(*pc != '\0') /*  while(ngotten) */
	{
		bzero(type,sizeof(type));
		i = 0;
		wc = 0;
		lastchar = '\0';
/*
 * read until the end of string
 */
		while(*pc != '\0'){
/*
 *avoid having empty spaces, tabs, newlines or end of buffer 
 */
			while(EXPR){
			
				type[i++] = *pc++;
/*
 * if number of chars in one word exceeds limit, print warning
 */
				if(i == (MAX_WORD_LENGTH-1))
					Perror("read_socket - word too long");
			}
			type[i] = '\0';
/*
 * save last character, if it is space, tab or \0 it means the buffer ended at end of the word
 * otherwise the buffer ended in the middle of word
 */
			if(*pc == '\0'){
				hi =0;
			}
			else
			{
				hi = 1;
			}

			if(i > 0 && *(pc+hi) == '\0') lastchar = *(pc+hi-1);
			
			if ( *(pc+hi) == '\0'){
/*
 * read next chunk of text file, complete the word by the rest from next chunk
 */
				bzero(buff,sizeof(buff));
				if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1)
					Perror("read");

				buff[ngotten] = '\0';

				pc = &buff[0];

				if(LASTEXPR)continue;
			}
/*
 * if word is longer then 0
 */			
			if(strlen(type) >0){
				wc++;
/*
 * get the name of the list
 */
				if(wc == 1){
					bzero(TMPSTR.Name_Of_List,sizeof(TMPSTR.Name_Of_List));
					if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", type) < 0)
						Perror("snprintf");
				}
/*
 * get the type of the list
 */
				else if (wc == 2 ){
					bzero(TMPSTR.Type,sizeof(TMPSTR.Type));
					if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", type) < 0)
						Perror("snprintf");
				}
/*
 * get the number, in case of DIR number is a number of items in DIR, in case of DATA, number is a number of dimensions
 */
				else if (wc == 3){
					TMPSTR.ndim = Strol(type);
					TMPSTR.dim=NULL;
/*
 * if type is FILE, allocate field for its dimensions
 * if the type is LINK, the dimensions will always be 0, if the dimensions is 1, IO operation dereference link
 */
					if ( strncmp(TMPSTR.Type,"DIR",3) != 0 &&  strncmp(TMPSTR.Type,"LINK",4) != 0 ){

						if( (TMPSTR.dim=(size_t *)malloc(TMPSTR.ndim * sizeof(size_t))) == NULL)
							Perror("malloc");
					}
					else
					{
/*
 * if type is DIR, read it
 */
						if( (Pnode = read_socket_dir_data(TMPSTR, descrpt)) == NULL)
							Perror("ReadSocketData - ReadDir");
						return Pnode;
					}
				}
				else if ( wc > 3 && strncmp(TMPSTR.Type,"DIR",3) != 0){

					TMPSTR.dim[wc - 4] = Strol(type);
					if( (wc - 3) == TMPSTR.ndim) break;
				}
			}

			if(IFEXPR) pc++;
			bzero(type,sizeof(type));
			i = 0;
			lastchar = '\0';
		}		
			
		if( (Pnode = AllocateNode(TMPSTR)) == NULL){
			Error("Allocate");}
	
		if( strncmp(TMPSTR.Type,"UC",2) == 0 || strncmp(TMPSTR.Type,"SC",2) == 0 || TMPSTR.Type[0] == 'C'){
/*
 * data is Char type
 */ 
			if( read_socket_data_charline(&Pnode, TMPSTR, descrpt) != 0)
				Error("Error reading data");
		}
		else
		{
/*
 * data are numbers
 */
			if( read_socket_data_line(&Pnode, TMPSTR, descrpt) != 0)
				Error("Error reading data");
		}

		if(TMPSTR.dim!=NULL){
			free(TMPSTR.dim);
			TMPSTR.dim = NULL;
		}
/*
 * Return main list
 */
		return Pnode;
 /*
  * end of reading the lists in DIR list   - while(ngotten)
  */
	}
/*
 * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
 */
	bzero(buff,sizeof(buff));
	if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1){
		Perror("read");
		return NULL;
	}
	
	buff[ngotten] = '\0';
	pc = &buff[0];
	if ( (Pnode = read_socket_data(descrpt)) == NULL)
		Error("ReadDirData: ReadData");
		
	return Pnode;
/*
 * If you get here something is wrong
 */
	return NULL;
}



int read_socket_data_line(node_t **Lnode, tmpstruct_t TMPSTR, int descrpt)
{
/* 
 * function reads data from FILE
 */
	char type[MAX_WORD_LENGTH], lastchar;
	size_t i, tot_dim, wc, hi, j;
	
	float         *pf;
	double        *pdf;
	long  double  *pldf;
/*
 * chars
 */
	char           *err;
/*
 * integers
 */
	short  int         	*psi;
	unsigned short int 	*pusi;
	int           		*pi;
	unsigned int  		*pui;
	long  int     		*pli;
	unsigned long int       *puli;
	long long int           *plli;
	signed long long int    *pslli;
	unsigned long long int  *pulli;

	size_t *pst;
	ptrdiff_t *pptrdf;
	
	tot_dim = 1;
	
	for(i=0; i<TMPSTR.ndim; i++)
		tot_dim = tot_dim * TMPSTR.dim[i];
/*
 * decide what type 
 */	
	if (strncmp(TMPSTR.Type,"LD",2) == 0){  /* long double */
		pldf = (*Lnode)->data.ldf;
	}
	else if(strncmp(TMPSTR.Type,"D",1) == 0){  /* double */
		pdf = (*Lnode)->data.df;
	}
	else if(strncmp(TMPSTR.Type,"F",1) == 0){  /* float */
		pf = (*Lnode)->data.f;
	}
/*
 * integers
 */
	else if(strncmp(TMPSTR.Type,"ULLI",4) == 0){  /* unsigned long long  int */
		pulli = (*Lnode)->data.ulli;
	}
	else if(strncmp(TMPSTR.Type,"SLLI",4) == 0){  /* signed long long  int */
		pslli = (*Lnode)->data.slli;
	}
	else if(strncmp(TMPSTR.Type,"ULI",3) == 0){  /* unsigned long int */
		puli = (*Lnode)->data.uli;
	}
	else if(strncmp(TMPSTR.Type,"USI",3) == 0){  /* unsigned short int */
		pusi = (*Lnode)->data.usi;
	}
	else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* unsigned long int */
		plli = (*Lnode)->data.lli;
	}
	else if(strncmp(TMPSTR.Type,"SI",2) == 0){  /* short int */
		psi = (*Lnode)->data.si;
	}
	else if(strncmp(TMPSTR.Type,"UI",2) == 0){  /* unsigned int */
		pui = (*Lnode)->data.ui;
	}
	else if(strncmp(TMPSTR.Type,"LI",2) == 0){  /* long  int */
		pli = (*Lnode)->data.li;
	}
	else if(strncmp(TMPSTR.Type,"I",1) == 0){  /* int */
		pi = (*Lnode)->data.i;
	}
/*
 * counters
 */
	else if(strncmp(TMPSTR.Type,"ST",2) == 0){  /* size_t */
		pst = (*Lnode)->data.st;
	}
	else if(strncmp(TMPSTR.Type,"PTRDF",1) == 0){  /* ptrdf_t */
		pptrdf = (*Lnode)->data.ptrdf;
	}
/*
 * process buffer
 */
	while(*pc != '\0') /*  while(ngotten) */
	{
		bzero(type,sizeof(type));
		i = 0;
		wc = 0;
		lastchar = '\0';
/*
 * read until the end of string
 */
		while(*pc != '\0'){
			while(EXPR){ /*avoid having empty spaces, tabs, newlines or end of buffer */
				type[i++] = *pc++;				
/*
 * if number of chars in one word exceeds limit, print warning
 */
				if(i == (MAX_WORD_LENGTH-1))
					Perror("read_socket - word too long");
			}
			type[i] = '\0';

/*
 * save last character, if it is space, tab or \0 it means the buffer ended at end of the word
 * otherwise the buffer ended in the middle of word
 */
			if(*pc == '\0'){
				hi =0;
			}
			else
			{
				hi = 1;
			}

			if(i > 0 && *(pc+hi) == '\0') lastchar = *(pc+hi-1); 
				
			
			if ( *(pc+hi) == '\0'){
/*
 * read next chunk of text file, complete the word by the rest from next chunk
 */
				bzero(buff,sizeof(buff));
				if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1)
					Perror("read");

				buff[ngotten] = '\0';
				pc = &buff[0];
				
				if(LASTEXPR) continue;
			}
/*
 * if word is longer then 0
 */			
			
			if(strlen(type) >0){
 				wc++;
/*
 * get the value
 */
		if (strncmp(TMPSTR.Type,"LD",2) == 0){  /* long double */
			*pldf++ = FCS_C2LD(type, &err);
		}
		else if(strncmp(TMPSTR.Type,"D",1) == 0){  /* double */
			*pdf++ = FCS_C2D(type, &err);
		}
		else if(strncmp(TMPSTR.Type,"F",1) == 0){  /* float */
			*pf++ = FCS_C2F(type, &err);
		}
/*
 * integers
 */
		else if(strncmp(TMPSTR.Type,"ULLI",4) == 0){  /* unsigned long long  int */
			*pslli++ = (unsigned long long int)FCS_C2LLI(type, &err);
		}
		else if(strncmp(TMPSTR.Type,"SLLI",4) == 0){  /* signed long long int */
			*pslli++ = (signed long long int)FCS_C2LLI(type, &err);
		}
		else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* unsigned long int */
			*plli++ = FCS_C2LLI(type, &err);
		}
		else if(strncmp(TMPSTR.Type,"ULI",3) == 0){  /* unsigned long int */
			*puli++ = (unsigned long int)FCS_C2LI(type, &err);
		}
		else if(strncmp(TMPSTR.Type,"USI",3) == 0){  /* unsigned short int */
			*pusi++ = (unsigned short int)FCS_C2I(type);
		}
		else if(strncmp(TMPSTR.Type,"SI",2) == 0){  /* short int */
			*psi++ = (signed int)FCS_C2I(type);
		}
		else if(strncmp(TMPSTR.Type,"UI",2) == 0){  /* unsigned int */
			*pui++ = (unsigned int)FCS_C2I(type);
		}
		else if(strncmp(TMPSTR.Type,"LI",2) == 0){  /* long  int */
			*pli++ = FCS_C2LI(type, &err);
		}
		else if(strncmp(TMPSTR.Type,"I",1) == 0){  /* int */
			*pi++ = FCS_C2I(type);
		}
/*
 * counters
 */
		else if(strncmp(TMPSTR.Type,"ST",2) == 0){  /* size_t */
			*pst++ = FCS_C2LLI(type, &err);
		}
		else if(strncmp(TMPSTR.Type,"PTRDF",1) == 0){  /* ptrdf_t */
			*pptrdf++ = FCS_C2LLI(type, &err);
		}
/*
  * end of reading the line, number of words is the same as required, return succes
  */
				if( wc == tot_dim ) return 0;
			}

			if(IFEXPR) pc++;
			bzero(type,sizeof(type));
			i = 0;
			lastchar = '\0';
		}
	}
/*
 * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
 */
	bzero(buff,sizeof(buff));
	if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1){
		Perror("read");
		return -1;
	}
	
	buff[ngotten] = '\0';
	pc = &buff[0];
	if( read_socket_data_line(Lnode, TMPSTR, descrpt) != 0){
		Error("Error reading data");
		return -1;
	}
		
	return 0;
/*
  * if you get here something went wrong
  */	
	return -1;
}

int read_socket_data_charline(node_t **Lnode, tmpstruct_t TMPSTR, int descrpt)
{
/* 
 * function reads data from FILE
 */
	size_t i, tot_dim;
	char 		*pdat;
	unsigned char 	*pdatu;
	signed char   	*pdats;

	tot_dim = 1;
	
	for(i=0; i<TMPSTR.ndim; i++)
		tot_dim = tot_dim * TMPSTR.dim[i];
/*
 * array was allocated with +1 to store '\0' symbol
 */
	tot_dim--;
/*
 * what type of data
 */
	if( strncmp(TMPSTR.Type,"UC",2) == 0){
		pdatu = (*Lnode)->data.uc;
/*
 * process buffer, set last char to \0
 */
		i = 0;
		while(*pc != '\0') /*  while(ngotten) */
		{
/*
 * read until end of buffer or until end of array dimension reached
 */
			while(IFEXPR) pc++;			
			while(*pc != '\0' && i < tot_dim){
				*pdatu++ = (unsigned char)*pc++;
				i++;
			}
/*
 * find why while was left
 */
			if(*pc == '\0'){	
				bzero(buff,sizeof(buff));
				if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1)
					Perror("read");

				if(ngotten == 0)return 0; /* no more data in buffer */
				buff[ngotten] = '\0';
				pc = &buff[0];
/*
 * if this is at the same time end of reading the text (i == tot_dim) and the first character of the next buffer is IFEXPR, return
 */
				if(i == tot_dim && IFEXPR) return 0;

			}
			else if (i == tot_dim){
				*pdatu = '\0';
				if( EXPR ){
					printf("Data set %s (%s): string: %s\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.uc);
					Error("Mismatch in string length");
					return -1;
				}
				return 0;
			}
		}
/*
 * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
 */
		bzero(buff,sizeof(buff));
		if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1){
			Perror("read");
			return -1;
		}
		
		buff[ngotten] = '\0';
		pc = &buff[0];
		if( read_socket_data_charline(Lnode, TMPSTR, descrpt) != 0){
				Error("Error reading data");
			return -1;
		}
			
		return 0;
	}
	else if ( strncmp(TMPSTR.Type,"SC",2) == 0 ){
		pdats = (*Lnode)->data.sc;
/*
 * process buffer, set last char to \0
 */
		i = 0;
		while(*pc != '\0') /*  while(ngotten) */
		{
/*
 * read until end of buffer or end of array dimension reached
 */
			while(IFEXPR) pc++;			
			while(*pc != '\0' && i < tot_dim){
				*pdats++ = (signed char)*pc++;
				i++;
			}
/*
 * find why while was left
 */
			if(*pc == '\0'){	
				bzero(buff,sizeof(buff));
				if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1)
					Perror("read");

				if(ngotten == 0)return 0; /* no more data in buffer */
				buff[ngotten] = '\0';
				pc = &buff[0];
/*
 * if this is at the same time end of reading the text (i == tot_dim) and the first character of the next buffer is IFEXPR, return
 */
				if(i == tot_dim && IFEXPR) return 0;

			}
			else if (i == tot_dim){
				*pdats = '\0';
				if( EXPR ){
					printf("Data set %s (%s): string: %s\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.sc);
					Error("Mismatch in string length");
					return -1;
				}
				return 0;
			}
		}
/*
 * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
 */
		bzero(buff,sizeof(buff));
		if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1){
			Perror("read");
			return -1;
		}
		
		buff[ngotten] = '\0';
		pc = &buff[0];
		if( read_socket_data_charline(Lnode, TMPSTR, descrpt) != 0){
				Error("Error reading data");
			return -1;
		}
			
		return 0;
	}
	else if ( TMPSTR.Type[0] == 'C'){

		pdat = (*Lnode)->data.c;
/*
 * process buffer, set last char to \0
 */
		i = 0;
		while(*pc != '\0') /*  while(ngotten) */
		{
/*
 * read until end of buffer or end of array dimension reached
 */
			while(IFEXPR) pc++;			
			while(*pc != '\0' && i < tot_dim){
				*pdat++ = *pc++;
				i++;
			}
/*
 * find why while was left
 */
			if(*pc == '\0'){	
				bzero(buff,sizeof(buff));
				if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1)
					Perror("read");

				if(ngotten == 0)return 0; /* no more data in buffer */
				buff[ngotten] = '\0';
				pc = &buff[0];
/*
 * if this is at the same time end of reading the text (i == tot_dim) and the first character of the next buffer is IFEXPR, return
 */
				if(i == tot_dim && IFEXPR) return 0;

			}
			else if (i == tot_dim){
				*pdat = '\0';
				if( EXPR ){
					printf("Data set %s (%s): string: %s\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.c);
					Error("Mismatch in string length");
					return -1;
				}
				return 0;
			}
		}
/*
 * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
 */
		bzero(buff,sizeof(buff));
		if (  (ngotten = read(descrpt,buff,MAXLINE-1)) == -1){
			Perror("read");
			return -1;
		}
		
		buff[ngotten] = '\0';
		pc = &buff[0];
		if( read_socket_data_charline(Lnode, TMPSTR, descrpt) != 0){
				Error("Error reading data");
			return -1;
		}
			
		return 0;
	}
	else{
		Error("char data type wrong");
	}
/*
  * if you get here something went wrong
  */	
	return -1;
}