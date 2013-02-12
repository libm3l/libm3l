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
 *     Function ReadDescriptor.c
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




 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"
#include "format_conversion_spec.h"

#include "FunctionsPrt.h"
#include "udf_rm.h"
#include "ReadSocket.h"

#define EXPR       (*pc != ' ' && *pc != '\t' && *pc != '\n' && *pc != '\0')
#define IFEXPR     (*pc == ' ' || *pc == '\t' || *pc == '\n' && *pc != '\0')
#define LASTEXPR   (lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0')


static lmint_t m3l_read_file_data_line(node_t **, tmpstruct_t, FILE *f, opts_t *);
static lmint_t m3l_read_file_data_charline(node_t **, tmpstruct_t, FILE *f);
static node_t *m3l_read_file_dir_data(tmpstruct_t , FILE *f, opts_t *);
static node_t *m3l_read_file_data(FILE *f, opts_t *);
static lmssize_t Fread(FILE * ,lmint_t);
static lmint_t CheckEOFile(FILE *);

lmchar_t *pc, buff[MAXLINE];
lmsize_t ngotten;

/*
 * Function read just one line from a socket, disregarding comments line
 * It identifies if the line is a header of DATA or DIR list
 * If DATA type of list it allocates a node_t and calls ReadDataLine which reads the
 * actual conent of the list
 * If the list if of DIR type, it calls ReadDir routine which reads DIR types of list (recursive calling)
 */ 
node_t *m3l_read_file(FILE *fp, opts_t *Popts)
{
	lmchar_t type[MAX_WORD_LENGTH], lastchar;
	lmsize_t   wc, i, hi, tmpi;
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
	ngotten = 0;
	
	if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
		Perror("fread");
	}

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
				ngotten = 0;
				if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
					Perror("fread");
				}

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

					if( (Dnode = m3l_read_file_dir_data(TMPSTR, fp, Popts)) == NULL)
						Perror("ReadDirData - ReadDir");
					
					if(CheckEOFile(fp) == 0){
						Error(" ReadDescriptor: End of file not reached\n");
					}
/*
 * check if now additional data in the file
 * disregard possible spaces, tabs, newlines at the end of file
 */
// 						while(IFEXPR) pc++;
						
/*
 * check if at the end of file was reached, if not give warning
 */
// 						if( !feof(fp) || *pc != '\0' ){
// 						
// 							tmpi = 0;
// 							printf("\n  WARNING - end of file not reached \n  Remaining part of the file starts at\n");
// 							while(*pc != '\0' && tmpi++ < 100)
// 								printf("%c", *pc++);
// 							printf("\n");
// // 							exit(0);
// 						}
/*
 * reading of the main node succesfully finished, return
 */
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


node_t *m3l_read_file_dir_data(tmpstruct_t TMPSTR, FILE *fp, opts_t *Popts)
{
	lmsize_t i;
	node_t *Dnode, *Tmpnode, *Pnode;
 /*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
	if( (Dnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
		Error("Allocate");
	}

	for(i=1;i<=TMPSTR.ndim; i++){
 		
		Tmpnode=NULL;	
				
		if ( (Tmpnode = m3l_read_file_data(fp, Popts)) == NULL)
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

node_t *m3l_read_file_data(FILE *fp, opts_t *Popts)
{
	lmchar_t type[MAX_WORD_LENGTH], lastchar;
	lmsize_t   wc, i, hi;
	tmpstruct_t TMPSTR;
	node_t *Pnode;
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
				ngotten = 0;
				if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
					Perror("fread");
				}

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
						if( (TMPSTR.dim=(lmsize_t *)malloc(TMPSTR.ndim * sizeof(lmsize_t))) == NULL)
							Perror("malloc");
					}
					else
					{
/*
 * if type is DIR, read it
 */
						if( (Pnode = m3l_read_file_dir_data(TMPSTR, fp, Popts)) == NULL)
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
					
		if( (Pnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
			Error("Allocate");}
		
		if( strncmp(TMPSTR.Type,"UC",2) == 0 || strncmp(TMPSTR.Type,"SC",2) == 0 || TMPSTR.Type[0] == 'C' || strncmp(TMPSTR.Type,"DISKFILE",8) == 0){
/*
 * data is Char type or DISK file
 * if DISKFILE, read name of the file and work with it only when sending through ReadSocket and Write2Socket
 */ 
			if( m3l_read_file_data_charline(&Pnode, TMPSTR, fp) != 0){
				Error("Error reading data");
				return NULL;
			}
		}
		else
		{
/*
 * data are numbers
 */
			if( m3l_read_file_data_line(&Pnode, TMPSTR, fp, Popts) != 0){
				Error("Error reading data");
				return NULL;
			}
		}

		if(TMPSTR.dim!=NULL)
		{
			free(TMPSTR.dim);
			TMPSTR.dim = NULL;
		}
/*
 * Return main list
 */
		return Pnode;
	}
/*
 * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
 */
	bzero(buff,sizeof(buff));
	if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
		Perror("fread");
		return NULL;
	}
	buff[ngotten] = '\0';
	pc = &buff[0];
	if ( (Pnode = m3l_read_file_data(fp, Popts)) == NULL)
		Error("ReadDirData: ReadData");
	
	return Pnode;
 /*
  * end of reading the lists in DIR list   - while(ngotten)
  */
	
/*
 * If you get here something is wrong
 */
	return NULL;
}



lmint_t m3l_read_file_data_line(node_t **Lnode, tmpstruct_t TMPSTR, FILE *fp, opts_t *Popts)
{
/* 
 * function reads data from FILE
 */
	lmchar_t type[MAX_WORD_LENGTH], lastchar;
	lmsize_t i, tot_dim, wc, hi, j;
	
	lmfloat_t         *pf;
	lmdouble_t        *pdf;
	lmlongdouble_t  *pldf;
/*
 * chars
 */
	lmchar_t           *err;
/*
 * integers
 */
// 	short  int         	*psi;
// 	unsigned short int 	*pusi;
// 	int           		*pi;
// 	unsigned int  		*pui;
// 	long  int     		*pli;
// 	unsigned long int       *puli;	
// 	long long int           *plli;
// 	signed long long int    *pslli;
// 	unsigned long long int  *pulli;

	lmshint_t		*psi;
	lmushint_t 	*pusi;
	lmint_t		*pi;
	lmuint_t	*pui;
	lmlint_t	*pli;
	lmulint_t	*puli;
	lmllint_t	*plli;
	lmsllint_t	*pslli;
	lmullint_t 	*pulli;

	lmsize_t *pst;
	lmptrdiff_t *pptrdf;

	tot_dim = 1;
	
	for(i=0; i<TMPSTR.ndim; i++)
		tot_dim = tot_dim * TMPSTR.dim[i];
/*
 * decide what type 
 */	
 	if (strncmp(TMPSTR.Type,"LD",2) == 0){  /* long double */
 		pldf = (*Lnode)->data.ldf;
#include "ReadDescriptor_Part1"
		*pldf++ = FCS_C2LD(type, &err);
#include "ReadDescriptor_Part2"
 	}
 	else if(strncmp(TMPSTR.Type,"D",1) == 0){  /* double */
 		pdf = (*Lnode)->data.df;
#include "ReadDescriptor_Part1"
		*pdf++ = FCS_C2D(type, &err);
#include "ReadDescriptor_Part2"
 	}
 	else if(strncmp(TMPSTR.Type,"F",1) == 0){  /* float */
 		pf = (*Lnode)->data.f;
#include "ReadDescriptor_Part1"
		*pf++ = FCS_C2F(type, &err);
#include "ReadDescriptor_Part2"
  	}
/*
 * integers
 */
	else if(strncmp(TMPSTR.Type,"ULLI",4) == 0){  /* unsigned long long  int */
		pulli = (*Lnode)->data.ulli;
#include "ReadDescriptor_Part1"
// 		*pulli++ = (unsigned long long int)FCS_C2LLI(type, &err);
		*pulli++ = (lmullint_t)FCS_C2LLI(type, &err);
#include "ReadDescriptor_Part2"
	}
	else if(strncmp(TMPSTR.Type,"SLLI",4) == 0){  /* signed long long  int */
		pslli = (*Lnode)->data.slli;
#include "ReadDescriptor_Part1"
// 		*pslli++ = (signed long long int)FCS_C2LLI(type, &err);
		*pslli++ = (lmsllint_t)FCS_C2LLI(type, &err);
#include "ReadDescriptor_Part2"
	}
	else if(strncmp(TMPSTR.Type,"ULI",3) == 0){  /* unsigned long int */
		puli = (*Lnode)->data.uli;
#include "ReadDescriptor_Part1"
// 		*puli++ = (unsigned long int)FCS_C2LI(type, &err);
		*puli++ = (lmulint_t)FCS_C2LI(type, &err);
#include "ReadDescriptor_Part2"
	}
	else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* unsigned long int */
		plli = (*Lnode)->data.lli;
#include "ReadDescriptor_Part1"
		*plli++ = (lmllint_t)FCS_C2LLI(type, &err);
#include "ReadDescriptor_Part2"
	}
	else if(strncmp(TMPSTR.Type,"USI",3) == 0){  /* unsigned short int */
		pusi = (*Lnode)->data.usi;
#include "ReadDescriptor_Part1"
// 		*pusi++ = (unsigned short int)FCS_C2I(type);
		*pusi++ = (lmushint_t)FCS_C2I(type);
#include "ReadDescriptor_Part2"
	}
	else if(strncmp(TMPSTR.Type,"SI",2) == 0){  /* short int */
		psi = (*Lnode)->data.si;
#include "ReadDescriptor_Part1"
// 		*psi++ = (signed int)FCS_C2I(type);
		*psi++ = (lmshint_t)FCS_C2I(type);
#include "ReadDescriptor_Part2"
	}
	else if(strncmp(TMPSTR.Type,"UI",2) == 0){  /* unsigned int */
		pui = (*Lnode)->data.ui;
#include "ReadDescriptor_Part1"
// 		*pui++ = (unsigned int)FCS_C2I(type);
		*pui++ = (lmuint_t)FCS_C2I(type);
#include "ReadDescriptor_Part2"
	}
	else if(strncmp(TMPSTR.Type,"LI",2) == 0){  /* long  int */
		pli = (*Lnode)->data.li;
#include "ReadDescriptor_Part1"
// 		*pli++ = FCS_C2LI(type, &err);
		*pli++ = (lmlint_t)FCS_C2LI(type, &err);
#include "ReadDescriptor_Part2"
	}
	else if(strncmp(TMPSTR.Type,"I",1) == 0){  /* int */
		pi = (*Lnode)->data.i;
#include "ReadDescriptor_Part1"
// 		*pi++ = FCS_C2I(type);
		*pi++ = (lmint_t)FCS_C2I(type);
#include "ReadDescriptor_Part2"
	}
/*
 * counters
 */
	else if(strncmp(TMPSTR.Type,"ST",2) == 0){  /* lmsize_t */
		pst = (*Lnode)->data.st;
#include "ReadDescriptor_Part1"
		*pst++ = FCS_C2LLI(type, &err);
#include "ReadDescriptor_Part2"
	}
	else if(strncmp(TMPSTR.Type,"PTRDF",1) == 0){  /* ptrdf_t */
		pptrdf = (*Lnode)->data.ptrdf;
#include "ReadDescriptor_Part1"
		*pptrdf++ = FCS_C2LLI(type, &err);
#include "ReadDescriptor_Part2"
	}
	
	return 0;
/*
  * if you get here something went wrong
  */	
	return -1;
}


lmint_t m3l_read_file_data_charline(node_t **Lnode, tmpstruct_t TMPSTR, FILE *fp)
{
/* 
 * function reads data from FILE
 */
	lmchar_t 		type[MAX_WORD_LENGTH], lastchar;
	lmsize_t 		i, tot_dim;
	lmchar_t		*pdat;
	lmusignchar_t 	*pdatu;
	lmsignchar_t   	*pdats;
	lmint_t 		init;

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
		init = 0;
		i = 0;
		while(*pc != '\0') /*  while(ngotten) */
		{
/* 
 * if reading the very begining of the text, disregard everything before \`\ symbol
 */
			if(init == 0){
againUC:
				while(*pc != '\0' && *pc != TEXT_SEPAR_SIGN && i++ < tot_dim)pc++;
/*
 * once at `, disregard it and put init =1
*  of cycle terminated bedcause of \0 do nothing
 */
				if(*pc != '\0'){
					pc++;
					init = 1;
					i=0;
				}
/*
 * did not find beginning of test ie. TEXT_SEPAR_SIGN, try reading file again
 */
				if(init == 0){
					bzero(buff,sizeof(buff));
					ngotten = 0;
					if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
						Perror("fread");
					}
/*
 * if End of File, give error message
 */
					if(feof(fp) && ngotten == 0){
						printf("Buffer '%s'\n", buff);
						printf("Data set %s (%s): string: '%s'\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.c);
						Error("m3l_read_file_data_charline Error: beginning of text (TEXT_SEPAR_SIGN) not found"); 
						return -1;
					}
/*
 * if no more data available, give error message
 */										
					if(ngotten == 0)return -1; /* no more data in buffer */
					buff[ngotten] = '\0';
					pc = &buff[0];
/*
 * the initial TEXT_SEPAR_SIGN not found, init == 0, attemt another reading
 */
					goto againUC;
				}
			}
/*
 * read until end of buffer or ` symbol
 */			
			while(*pc != '\0' && *pc != TEXT_SEPAR_SIGN)
				*pdatu++ = (lmusignchar_t)*pc++;
/*
 * find why while was left
 */
			if(*pc == '\0'){	
				bzero(buff,sizeof(buff));
				ngotten = 0;
				if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
						Perror("fread");
				}

				if(ngotten == 0)return 0; /* no more data in buffer */
				buff[ngotten] = '\0';
				pc = &buff[0];
			}
			else if (*pc == TEXT_SEPAR_SIGN){
/*
 * check that string dimentions are correct
 */
				if( i != tot_dim ){
					printf("Data set %s (%s): string: %s\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.uc);
					Error("Mismatch in string length");
					return -1;
				}
				pc++;  /* do not count ` in the next buff analysis */
				*pdatu = '\0';
				return 0;
			}
		}
/*
 * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
 */
		bzero(buff,sizeof(buff));
		if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
			Perror("fread");
		}
		
		buff[ngotten] = '\0';
		pc = &buff[0];
		if( m3l_read_file_data_charline(Lnode, TMPSTR, fp) != 0){
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
		init = 0;
		i = 0;
		while(*pc != '\0') /*  while(ngotten) */
		{
/* 
 * if reading the very begining of the text, disregard everything before \`\ symbol
 */
			if(init == 0){
againSC:
				while(*pc != '\0' && *pc != TEXT_SEPAR_SIGN && i++ < tot_dim)pc++;
/*
 * once at `, disregard it and put init =1
*  of cycle terminated bedcause of \0 do nothing
 */
				if(*pc != '\0'){
					pc++;
					init = 1;
					i=0;
				}
/*
 * did not find beginning of test ie. TEXT_SEPAR_SIGN, try reading file again
 */
				if(init == 0){
					bzero(buff,sizeof(buff));
					ngotten = 0;
					if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
						Perror("fread");
					}
/*
 * if End of File, give error message
 */
					if(feof(fp) && ngotten == 0){
						printf("Buffer '%s'\n", buff);
						printf("Data set %s (%s): string: '%s'\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.c);
						Error("m3l_read_file_data_charline Error: beginning of text (TEXT_SEPAR_SIGN) not found"); 
						return -1;
					}
/*
 * if no more data available, give error message
 */										
					if(ngotten == 0)return -1; /* no more data in buffer */
					buff[ngotten] = '\0';
					pc = &buff[0];
/*
 * the initial TEXT_SEPAR_SIGN not found, init == 0, attemt another reading
 */
					goto againSC;
				}				
			}
/*
 * read until end of buffer or ` symbol
 */			
			while(*pc != '\0' && *pc != TEXT_SEPAR_SIGN)
				*pdats++ = (lmsignchar_t)*pc++;
/*
 * find why while was left
 */
			if(*pc == '\0'){	
				bzero(buff,sizeof(buff));
				ngotten = 0;
				if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
					Perror("fread");
				}

				if(ngotten == 0)return 0; /* no more data in buffer */
				buff[ngotten] = '\0';
				pc = &buff[0];
			}
			else if (*pc == TEXT_SEPAR_SIGN){
/*
 * check that string dimentions are correct
 */
				if( i != tot_dim ){
					printf("Data set %s (%s): string: %s\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.sc);
					Error("Mismatch in string length");
					return -1;
				}
				pc++;  /* do not count ` in the next buff analysis */
				*pdats = '\0';
				return 0;
			}
		}
/*
 * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
 */
		bzero(buff,sizeof(buff));
		if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
			Perror("fread");
		}
		
		buff[ngotten] = '\0';
		pc = &buff[0];
		if( m3l_read_file_data_charline(Lnode, TMPSTR, fp) != 0){
			Error("Error reading data");
			return -1;
		}	
		return 0;
		
		
	}
	else if ( TMPSTR.Type[0] == 'C'){
		pdat = (*Lnode)->data.c;
/*
 * process buffer, set last char to
 * set init = 0 maning going to read a word, need to find initial ` symbol
 */
		init = 0;
		i = 0;

		while(*pc != '\0') /*  while(ngotten) */
		{
/* 
 * if reading the very begining of the text, disregard everything before \`\ symbol
 */
			if(init == 0){
againC:				
				while(*pc != '\0' && *pc != TEXT_SEPAR_SIGN)pc++;
/*
 * once at `, disregard it and put init =1
*  of cycle terminated bedcause of \0 do nothing
 */
				if(*pc != '\0'){
					pc++;
					init = 1;
					i=0;
				}
/*
 * did not find beginning of test ie. TEXT_SEPAR_SIGN, try reading file again
 */
				if(init == 0){
					bzero(buff,sizeof(buff));
					ngotten = 0;
					if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
						Perror("fread");
					}
/*
 * if End of File, give error message
 */
					if(feof(fp) && ngotten == 0){
						printf("Buffer '%s'\n", buff);
						printf("Data set %s (%s): string: '%s'\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.c);
						Error("m3l_read_file_data_charline Error: beginning of text (TEXT_SEPAR_SIGN) not found"); 
						return -1;
					}
/*
 * if no more data available, give error message
 */										
					if(ngotten == 0)return -1; /* no more data in buffer */
					buff[ngotten] = '\0';
					pc = &buff[0];
/*
 * the initial TEXT_SEPAR_SIGN not found, init == 0, attemt another reading
 */
					goto againC;
				}
			}
/*
 * read until end of buffer or ` symbol
 */			
			while(*pc != '\0' && *pc != TEXT_SEPAR_SIGN){				
				*pdat++ = *pc++;
				i++;
			}
/*
 * find why while was left
 */
			if(*pc == '\0'){
				bzero(buff,sizeof(buff));
				ngotten = 0;
				if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
					Perror("fread");
				}

				if(ngotten == 0)return 0; /* no more data in buffer */
				buff[ngotten] = '\0';
				pc = &buff[0];
/*
 * NOTE - need to add here that if the word is ending i == tot_dim and next char is separ sign, leave - see ReadSocket.c
 */
			}
			else if (*pc == TEXT_SEPAR_SIGN){
/*
 * check that string dimentions are correct
 */
				if( i != tot_dim ){
					printf("Data set %s (%s): string: '%s'  DIAG: %ld   %ld\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.c, i, tot_dim);
					Error("Mismatch in string length");
					return -1;
				}
				pc++;  /* do not count ` in the next buff analysis */
				*pdat = '\0';
				return 0;
			}
		}
/*
 * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
 */
		bzero(buff,sizeof(buff));	
		if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
			Perror("fread");
		}
		
		buff[ngotten] = '\0';
		pc = &buff[0];
		if( m3l_read_file_data_charline(Lnode, TMPSTR, fp) != 0){
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

lmssize_t Fread(FILE *fp ,lmint_t n)
{
		if(   (ngotten = fread(buff, sizeof(buff[0]), n, fp))   < MAXLINE-1){
			if(ferror(fp))
				Perror("fread - ferror");
				return -1;
		}
		buff[ngotten] = '\0';
	
	return ngotten;

}

/*
 * function check the end of filed
 */
lmint_t CheckEOFile(FILE *fp){
	lmsize_t tmpi;
/*
 * get rid of all null spaces, tabs, newlines etc.
 */
	while(IFEXPR) pc++;
/*
 * check if at the end of file was reached, if not give warning
 */
	if( feof(fp) && *pc == '\0' ) 
		return 1;
	else if( feof(fp)){
/*
 * buffer still contains additional info, and end of file was reached, give back error message
 */
		tmpi = 0;
		printf("\n  WARNING - end of file not reached, remaining part of the file starts with\n");
		while(*pc != '\0' && tmpi++ < 100)
		printf("%c", *pc++);
		printf("\n");
		return 0;
	}
	else if( *pc == '\0'){
/*
 * EOF file not reached, end of buffer reached, read again
 */
		bzero(buff, strlen(buff));
		ngotten = 0;
		if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
			Perror("fread");
		}
		buff[ngotten] = '\0';		
		pc = &buff[0];
	}
/*
 * process the string, in case it returned anything
 */
	while(*pc != '\0'){
		
		while(IFEXPR) pc++;
/*
 * check if at the end of file was reached, if not give warning
 */
		if( feof(fp) && *pc == '\0' ) return 1;
	
		if( feof(fp)){
/*
 * buffer still contains additional info, and end of file was reached, give back error message
 */
			tmpi = 0;
			printf("\n  WARNING - end of file not reached, remaining part of the file starts with\n");
			while(*pc != '\0' && tmpi++ < 100)
			printf("%c", *pc++);
			printf("\n");
			return 0;
		}
		else if(*pc == '\0' ) {
			bzero(buff, strlen(buff));
			ngotten = 0;
			if(   (ngotten = Fread(fp, MAXLINE-1))   < 0){
				Perror("fread");
			}
			buff[ngotten] = '\0';		
			pc = &buff[0];
		}
	}
	
	if( feof(fp) && *pc == '\0' ) 
		return 1;
	else if( feof(fp)){
/*
 * buffer still contains additional info, and end of file was reached, give back error message
 */
		tmpi = 0;
		printf("\n  WARNING - end of file not reached, remaining part of the file starts with\n");
		while(*pc != '\0' && tmpi++ < 100)
		printf("%c", *pc++);
		printf("\n");
		return 0;
	}
	return 0;
}
