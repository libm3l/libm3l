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
 *     Function ReadSocket.c
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
#include "format_conversion_spec.h"

#include "FunctionsPrt.h"
#include "udf_rm.h"
#include "ReadSocket.h"
#include "NumberConversion.h"
#include "Check_EOFbuff.h"

/*
 * note EXPR = EXPR_SNPRNTF was used when MEMCP was #define(d) in libm3l_header.h
 * for valule of MEMCP == SNPRINTF
 * for value fo MEMCP == MEMCPY the value of EXPR = EXPR_MEMCP
 * 
 * now, only EXPR = EXPR_MEMCP is used
 */

//#define EXPR_SNPRNTF (*pc != ' ' && *pc != '\t' && *pc != '\n' && *pc != SEPAR_SIGN && *pc != '\0')
//#define EXPR_MEMCP (*pc != SEPAR_SIGN && *pc != '\0')

#define EXPR (*pc != SEPAR_SIGN && *pc != '\0')

#define IFEXPR     ( (*pc == SEPAR_SIGN || *pc == ' ' || *pc == '\t' || *pc == '\n' ) && *pc != '\0')
#define LASTEXPR   (lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0' && lastchar != SEPAR_SIGN)


/* 
 * NOTE: these can be:  (Wite2Socket filters spaces, tabs and newlines)
#define EXPR      *pc != SEPAR_SIGN && *pc != '\0'
#define IFEXPR   *pc != '\0' || *pc == SEPAR_SIGN 
#define LASTEXPR   lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0' && lastchar != SEPAR_SIGN 
*/

static lmint_t m3l_read_socket_data_line(node_t **, tmpstruct_t, lmint_t, opts_t *);
static lmint_t m3l_read_socket_data_charline(node_t **, tmpstruct_t, lmint_t);
static node_t *m3l_read_socket_dir_data(tmpstruct_t , lmint_t, opts_t *);
static node_t *m3l_read_socket_data(lmint_t, opts_t *);
static lmssize_t Read(lmint_t ,lmint_t );

static lmchar_t *pc, buff[MAXLINE];
static lmssize_t ngotten;

/*
 * Function read just one line from a socket, disregarding comments line
 * It identifies if the line is a header of DATA or DIR list
 * If DATA type of list it allocates a node_t and calls ReadDataLine which reads the
 * actual conent of the list
 * If the list if of DIR type, it calls ReadDir routine which reads DIR types of list (recursive calling)
 * 
  * This function is a copy of ReadDescriptor, the only change is replacing the 
 * FILE * by lmint_t and fread by read and having SEPAR_SIGN as a separation sign between the 
 * words, although the algorithm should be able to take spaces, tabs and new lines too.
 * For C, UC and SC fields, spaces, \t and \n are taken as valid characters
 */ 
node_t *m3l_read_socket(lmint_t descrpt, opts_t *Popts)
{
	lmchar_t type[MAX_WORD_LENGTH], lastchar;
	lmsize_t   wc, i, hi;
	tmpstruct_t TMPSTR;
	node_t *Dnode;
	lmchar_t prevbuff[EOBlen+1];
/*
 * set EXPR for type of writing to buffer
 */
	bzero(prevbuff,EOBlen+1);
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
 * read MAXLINE-1, MAXLINE will be '\0', put pointer at the beginning of the array
 */
	bzero(buff, strlen(buff));
	if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1)
		Perror("read");

	buff[ngotten] = '\0';		
	pc = &buff[0];
	
	if(ngotten == 0)return (node_t *)NULL;
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
				if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1)
					Perror("read");
				buff[ngotten] = '\0';
				pc = &buff[0];
				
				if(ngotten == 0)return (node_t *)NULL;
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

					if( (Dnode = m3l_read_socket_dir_data(TMPSTR, descrpt, Popts)) == NULL)
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
 * check that the end of entire message ends with EOFbuff
 * length of EOFbuff is EOBlen (defined in libm3l_header.h)
 * loop over EOBlen times to make sure that if EOFbuff is sent over in single bytes 
 * entire word is received. If possitive test ( == 1), return Gnode, otherwise give warning and exit
 */
					for(i=0; i<EOBlen; i++){
						if(Check_EOFbuff(buff,prevbuff, strlen(buff),EOBlen, EOFbuff) == 1){
							bzero(buff,sizeof(buff));
							return Dnode;
						}
							
						bzero(buff,sizeof(buff));
						if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1)
 							Perror("read");
					}
					printf("\n  WARNING - end of buffer not reached, remaining data is %s\n", buff);
						exit(0);
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
node_t *m3l_read_socket_dir_data(tmpstruct_t TMPSTR, lmint_t descrpt, opts_t *Popts)
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
		if ( (Tmpnode = m3l_read_socket_data(descrpt, Popts)) == NULL)
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





node_t *m3l_read_socket_data(lmint_t descrpt, opts_t *Popts)
{
	lmchar_t type[MAX_WORD_LENGTH], lastchar;
	lmsize_t   wc, i, hi;
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
				if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1)
					Perror("read");

				buff[ngotten] = '\0';
				pc = &buff[0];
				if(ngotten == 0)return (node_t *)NULL;

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
						if( (Pnode = m3l_read_socket_dir_data(TMPSTR, descrpt, Popts)) == NULL)
							Perror("ReadSocketData - ReadDir");
						return Pnode;
					}
				}
				else if ( wc > 3 && strncmp(TMPSTR.Type,"DIR",3) != 0){

					TMPSTR.dim[wc - 4] = Strol(type);
					if( (wc - 3) == TMPSTR.ndim) break; /* stop reading when all dimensions are red */
				}
			}

			if(IFEXPR) pc++;
			bzero(type,sizeof(type));
			i = 0;
			lastchar = '\0';
		}		
			
		if( (Pnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
			Error("Allocate");}
	
		if( strncmp(TMPSTR.Type,"UC",2) == 0 || strncmp(TMPSTR.Type,"SC",2) == 0 || TMPSTR.Type[0] == 'C'){
/*
 * data is Char type
 */ 
			if( m3l_read_socket_data_charline(&Pnode, TMPSTR, descrpt) != 0)
				Error("Error reading data");
		}
/*
 * data is DISKFILE from a disk without any specific format type
 */ 		
		else if( strncmp(TMPSTR.Type,"DISKFILE",8) == 0){
			Error("ReadSocket.c: not implemented yet");
		}
		else
		{
/*
 * data are numbers
 */
			if( m3l_read_socket_data_line(&Pnode, TMPSTR, descrpt, Popts) != 0)
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
	if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1){
		Perror("read");
		return NULL;
	}
	
	buff[ngotten] = '\0';
	pc = &buff[0];
	if(ngotten == 0)return (node_t *)NULL;

	if ( (Pnode = m3l_read_socket_data(descrpt, Popts)) == NULL)
		Error("ReadDirData: ReadData");
		
	return Pnode;
/*
 * If you get here something is wrong
 */
	return NULL;
}



lmint_t m3l_read_socket_data_line(node_t **Lnode, tmpstruct_t TMPSTR, lmint_t descrpt, opts_t *Popts)
{
/* 
 * function reads data from FILE
 */
	lmchar_t type[MAX_WORD_LENGTH], lastchar;
	lmsize_t i, tot_dim, wc, hi, len;
	
	lmfloat_t         *pf, f2;
	lmdouble_t        *pdf, d2;
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
// 	long long int          	*plli;
// 	signed long long int   	*pslli;
// 	unsigned long long int 	*pulli;
	
	lmshint_t	*psi;
	lmushint_t 	*pusi;
	lmint_t		*pi;
	lmuint_t	*pui;
	lmlint_t	*pli;
	lmulint_t	*puli;
	lmllint_t	*plli;
	lmsllint_t	*pslli;
	lmullint_t 	*pulli;

	Uint32_t fi;
	Uint64_t di;
	
	void *(*GetFromBuffD)(Uint64_t *, lmchar_t *, lmsize_t);
	void *(*GetFromBuffF)(Uint64_t *, lmchar_t *, lmsize_t);
	
	lmsize_t *pst;
	lmptrdiff_t *pptrdf;
	
	if(Popts == NULL)
		Error("ReadSocket: NULL Popts");
/*
 * determine buffer copying
 */
	if(Popts->opt_MEMCP == 'M'){
		GetFromBuffD = RD_MemcpyD;
		len = 8;
	}
	else{
		GetFromBuffD = RD_StrtoullD;
		len = 16;
	}
	
	tot_dim = 1;
	
	for(i=0; i<TMPSTR.ndim; i++)
		tot_dim = tot_dim * TMPSTR.dim[i];
/*
 * decide what type 
 */	
 	if (strncmp(TMPSTR.Type,"LD",2) == 0){  /* long double */
 		pldf = (*Lnode)->data.ldf;

		if(Popts->opt_tcpencoding == 'I'){   /* IEEE-754 encoding */
			#include "ReadSocket_Part1"
			di = Strtoull(type, 16);
			d2 = unpack754_64(di);
			*pldf++ = d2;
			#include "ReadSocket_Part2"
 		}
		else if(Popts->opt_tcpencoding == 'r'){   /* raw data */
			Error("Raw coding not implemented");
			exit(0);
		}
 		else if(Popts->opt_tcpencoding == 't'){   /* text enconding */
			#include "ReadSocket_Part1"
			*pldf++ = FCS_C2LD(type, &err);
			#include "ReadSocket_Part2"
 		}
 	}
 	else if(strncmp(TMPSTR.Type,"D",1) == 0){  /* double */
 		pdf = (*Lnode)->data.df;

		if(Popts->opt_tcpencoding == 'I'){   /* IEEE-754 encoding */
			#include "ReadSocket_Part1"
			GetFromBuffD(&di, &type[0], len);
			d2 = unpack754_64(di);
			*pdf++ = d2;
			#include "ReadSocket_Part2"
 		}
		else if(Popts->opt_tcpencoding == 'r'){   /* raw data */
			Error("Raw coding not implemented");
			exit(0);
		}
 		else if(Popts->opt_tcpencoding == 't'){   /* text enconding */
			#include "ReadSocket_Part1"
			*pdf++ = FCS_C2D(type, &err);
			#include "ReadSocket_Part2"
 		}
 	}
 	else if(strncmp(TMPSTR.Type,"F",1) == 0){  /* float */
 		pf = (*Lnode)->data.f;
		
		if(Popts->opt_tcpencoding == 'I'){   /* IEEE-754 encoding */
			#include "ReadSocket_Part1"
 			fi = Strtoul(type, 8);
			f2 = unpack754_32(fi);
			*pf++ = f2;
			#include "ReadSocket_Part2"
 		}
		else if(Popts->opt_tcpencoding == 'r'){   /* raw data */
			Error("Raw coding not implemented");
			exit(0);
		}
 		else if(Popts->opt_tcpencoding == 't'){   /* text enconding */
			#include "ReadSocket_Part1"
			*pf++ = FCS_C2F(type, &err);
			#include "ReadSocket_Part2"
		}
  	}
/*
 * integers
 */
	else if(strncmp(TMPSTR.Type,"ULLI",4) == 0){  /* unsigned long long  int */
		pulli = (*Lnode)->data.ulli;
#include "ReadSocket_Part1"
// 			*pulli++ = (unsigned long long int)FCS_C2LLI(type, &err);
			*pulli++ = (lmullint_t)FCS_C2LLI(type, &err);
#include "ReadSocket_Part2"
	}
	else if(strncmp(TMPSTR.Type,"SLLI",4) == 0){  /* signed long long  int */
		pslli = (*Lnode)->data.slli;
#include "ReadSocket_Part1"
// 		*pslli++ = (signed long long int)FCS_C2LLI(type, &err);
		*pslli++ = (lmsllint_t)FCS_C2LLI(type, &err);
#include "ReadSocket_Part2"
	}
	else if(strncmp(TMPSTR.Type,"ULI",3) == 0){  /* unsigned long int */
		puli = (*Lnode)->data.uli;
#include "ReadSocket_Part1"
// 		*puli++ = (unsigned long int)FCS_C2LI(type, &err);
		*puli++ = (lmulint_t)FCS_C2LI(type, &err);
#include "ReadSocket_Part2"
	}
	else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* unsigned long int */
		plli = (*Lnode)->data.lli;
#include "ReadSocket_Part1"
// 		*plli++ = FCS_C2LLI(type, &err);
		*plli++ = (lmllint_t)FCS_C2LLI(type, &err);
#include "ReadSocket_Part2"
	}
	else if(strncmp(TMPSTR.Type,"USI",3) == 0){  /* unsigned short int */
		pusi = (*Lnode)->data.usi;
#include "ReadSocket_Part1"
// 		*pusi++ = (unsigned short int)FCS_C2I(type);
		*pusi++ = (lmushint_t)FCS_C2I(type);
#include "ReadSocket_Part2"
	}
	else if(strncmp(TMPSTR.Type,"SI",2) == 0){  /* short int */
		psi = (*Lnode)->data.si;
#include "ReadSocket_Part1"
// 		*psi++ = (signed int)FCS_C2I(type);
		*psi++ = (lmshint_t)FCS_C2I(type);
#include "ReadSocket_Part2"
	}
	else if(strncmp(TMPSTR.Type,"UI",2) == 0){  /* unsigned int */
		pui = (*Lnode)->data.ui;
#include "ReadSocket_Part1"
// 		*pui++ = (unsigned int)FCS_C2I(type);
		*pui++ = (lmuint_t)FCS_C2I(type);
#include "ReadSocket_Part2"
	}
	else if(strncmp(TMPSTR.Type,"LI",2) == 0){  /* long  int */
		pli = (*Lnode)->data.li;
#include "ReadSocket_Part1"
// 		*pli++ = FCS_C2LI(type, &err);
		*pli++ = (lmint_t)FCS_C2LI(type, &err);
#include "ReadSocket_Part2"
	}
	else if(strncmp(TMPSTR.Type,"I",1) == 0){  /* int */
		pi = (*Lnode)->data.i;
#include "ReadSocket_Part1"
// 		*pi++ = FCS_C2I(type);
		*pi++ = (lmint_t)FCS_C2I(type);
#include "ReadSocket_Part2"
	}
/*
 * counters
 */
	else if(strncmp(TMPSTR.Type,"ST",2) == 0){  /* size_t */
		pst = (*Lnode)->data.st;
#include "ReadSocket_Part1"
		*pst++ = FCS_C2LLI(type, &err);
#include "ReadSocket_Part2"
	}
	else if(strncmp(TMPSTR.Type,"PTRDF",1) == 0){  /* ptrdf_t */
		pptrdf = (*Lnode)->data.ptrdf;
#include "ReadSocket_Part1"
		*pptrdf++ = FCS_C2LLI(type, &err);
#include "ReadSocket_Part2"
	}
		
	return 0;
/*
  * if you get here something went wrong
  */	
	return -1;
}


// int m3l_read_socket_data_line1(node_t **Lnode, tmpstruct_t TMPSTR, int descrpt, opts_t *Popts)
// {
// /* 
//  * function reads data from FILE
//  */
// 	char type[MAX_WORD_LENGTH], lastchar;
// 	size_t i, tot_dim, wc, hi, j;
// 	
// 	float         *pf;
// 	double        *pdf;
// 	long  double  *pldf;
// /*
//  * chars
//  */
// 	char           *err;
// /*
//  * integers
//  */
// // 	short  int         	*psi;
// // 	unsigned short int 	*pusi;
// // 	int           		*pi;
// // 	unsigned int  		*pui;
// // 	long  int     		*pli;
// // 	unsigned long int       *puli;
// // 	long long int           *plli;
// // 	signed long long int    *pslli;
// // 	unsigned long long int  *pulli;
// 	
// 	lmshint_t		*psi;
// 	lmushint_t 	*pusi;
// 	lmint_t		*pi;
// 	lmuint_t	*pui;
// 	LI_T		*pli;
// 	lmulint_t	*puli;
// 	lmllint_t		*plli;
// 	lmllint_t		*pslli;
// 	lmullint_t 	*pulli;
// 
// 	size_t *pst;
// 	ptrdiff_t *pptrdf;
// 	
// 	tot_dim = 1;
// 	
// 	for(i=0; i<TMPSTR.ndim; i++)
// 		tot_dim = tot_dim * TMPSTR.dim[i];
// /*
//  * decide what type 
//  */	
// 	if (strncmp(TMPSTR.Type,"LD",2) == 0){  /* long double */
// 		pldf = (*Lnode)->data.ldf;
// 	}
// 	else if(strncmp(TMPSTR.Type,"D",1) == 0){  /* double */
// 		pdf = (*Lnode)->data.df;
// 	}
// 	else if(strncmp(TMPSTR.Type,"F",1) == 0){  /* float */
// 		pf = (*Lnode)->data.f;
// 	}
// /*
//  * integers
//  */
// 	else if(strncmp(TMPSTR.Type,"ULLI",4) == 0){  /* unsigned long long  int */
// 		pulli = (*Lnode)->data.ulli;
// 	}
// 	else if(strncmp(TMPSTR.Type,"SLLI",4) == 0){  /* signed long long  int */
// 		pslli = (*Lnode)->data.slli;
// 	}
// 	else if(strncmp(TMPSTR.Type,"ULI",3) == 0){  /* unsigned long int */
// 		puli = (*Lnode)->data.uli;
// 	}
// 	else if(strncmp(TMPSTR.Type,"USI",3) == 0){  /* unsigned short int */
// 		pusi = (*Lnode)->data.usi;
// 	}
// 	else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* unsigned long int */
// 		plli = (*Lnode)->data.lli;
// 	}
// 	else if(strncmp(TMPSTR.Type,"SI",2) == 0){  /* short int */
// 		psi = (*Lnode)->data.si;
// 	}
// 	else if(strncmp(TMPSTR.Type,"UI",2) == 0){  /* unsigned int */
// 		pui = (*Lnode)->data.ui;
// 	}
// 	else if(strncmp(TMPSTR.Type,"LI",2) == 0){  /* long  int */
// 		pli = (*Lnode)->data.li;
// 	}
// 	else if(strncmp(TMPSTR.Type,"I",1) == 0){  /* int */
// 		pi = (*Lnode)->data.i;
// 	}
// /*
//  * counters
//  */
// 	else if(strncmp(TMPSTR.Type,"ST",2) == 0){  /* size_t */
// 		pst = (*Lnode)->data.st;
// 	}
// 	else if(strncmp(TMPSTR.Type,"PTRDF",1) == 0){  /* ptrdf_t */
// 		pptrdf = (*Lnode)->data.ptrdf;
// 	}
// /*
//  * process buffer
//  */
// 	while(*pc != '\0') /*  while(ngotten) */
// 	{
// 		bzero(type,sizeof(type));
// 		i = 0;
// 		wc = 0;
// 		lastchar = '\0';
// /*
//  * read until the end of string
//  */
// 		while(*pc != '\0'){
// 			while(EXPR){ /*avoid having empty spaces, tabs, newlines or end of buffer */
// 				type[i++] = *pc++;				
// /*
//  * if number of chars in one word exceeds limit, print warning
//  */
// 				if(i == (MAX_WORD_LENGTH-1))
// 					Perror("read_socket - word too long");
// 			}
// 			type[i] = '\0';
// 
// /*
//  * save last character, if it is space, tab or \0 it means the buffer ended at end of the word
//  * otherwise the buffer ended in the middle of word
//  */
// 			if(*pc == '\0'){
// 				hi =0;
// 			}
// 			else
// 			{
// 				hi = 1;
// 			}
// 
// 			if(i > 0 && *(pc+hi) == '\0') lastchar = *(pc+hi-1); 
// 				
// 			
// 			if ( *(pc+hi) == '\0'){
// /*
//  * read next chunk of text file, complete the word by the rest from next chunk
//  */
// 				bzero(buff,sizeof(buff));
// 				if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1)
// 					Perror("read");
// 
// 				buff[ngotten] = '\0';
// 				pc = &buff[0];
// 				if(ngotten == 0)return -1;
// 
// 				
// 				if(LASTEXPR) continue;
// 			}
// /*
//  * if word is longer then 0
//  */			
// 			
// 			if(strlen(type) >0){
//  				wc++;
// /*
//  * get the value
//  */
// 		if (strncmp(TMPSTR.Type,"LD",2) == 0){  /* long double */
// 			*pldf++ = FCS_C2LD(type, &err);
// 		}
// 		else if(strncmp(TMPSTR.Type,"D",1) == 0){  /* double */
// 			*pdf++ = FCS_C2D(type, &err);
// 		}
// 		else if(strncmp(TMPSTR.Type,"F",1) == 0){  /* float */
// 			*pf++ = FCS_C2F(type, &err);
// 		}
// /*
//  * integers
//  */
// 		else if(strncmp(TMPSTR.Type,"ULLI",4) == 0){  /* unsigned long long  int */
// // 			*pulli++ = (unsigned long long int)FCS_C2LLI(type, &err);
// 			*pulli++ = (lmullint_t)FCS_C2LLI(type, &err);
// 		}
// 		else if(strncmp(TMPSTR.Type,"SLLI",4) == 0){  /* signed long long int */
// // 			*pslli++ = (signed long long int)FCS_C2LLI(type, &err);
// 			*pslli++ = (lmllint_t)FCS_C2LLI(type, &err);
// 		}
// 		else if(strncmp(TMPSTR.Type,"LLI",3) == 0){  /* unsigned long int */
// // 			*plli++ = FCS_C2LLI(type, &err);
// 			*plli++ = (lmllint_t)FCS_C2LLI(type, &err);
// 		}
// 		else if(strncmp(TMPSTR.Type,"ULI",3) == 0){  /* unsigned long int */
// // 			*puli++ = (unsigned long int)FCS_C2LI(type, &err);
// 			*puli++ = (lmulint_t)FCS_C2LI(type, &err);
// 		}
// 		else if(strncmp(TMPSTR.Type,"USI",3) == 0){  /* unsigned short int */
// // 			*pusi++ = (unsigned short int)FCS_C2I(type);
// 			*pusi++ = (lmushint_t)FCS_C2I(type);
// 		}
// 		else if(strncmp(TMPSTR.Type,"SI",2) == 0){  /* short int */
// // 			*psi++ = (signed int)FCS_C2I(type);
// 			*psi++ = (lmshint_t)FCS_C2I(type);
// 		}
// 		else if(strncmp(TMPSTR.Type,"UI",2) == 0){  /* unsigned int */
// // 			*pui++ = (unsigned int)FCS_C2I(type);
// 			*pui++ = (lmuint_t)FCS_C2I(type);
// 		}
// 		else if(strncmp(TMPSTR.Type,"LI",2) == 0){  /* long  int */
// // 			*pli++ = FCS_C2LI(type, &err);
// 			*pli++ = (LI_T)FCS_C2LI(type, &err);
// 		}
// 		else if(strncmp(TMPSTR.Type,"I",1) == 0){  /* int */
// // 			*pi++ = FCS_C2I(type);
// 			*pi++ = (lmint_t)FCS_C2I(type);
// 		}
// /*
//  * counters
//  */
// 		else if(strncmp(TMPSTR.Type,"ST",2) == 0){  /* size_t */
// 			*pst++ = FCS_C2LLI(type, &err);
// 		}
// 		else if(strncmp(TMPSTR.Type,"PTRDF",1) == 0){  /* ptrdf_t */
// 			*pptrdf++ = FCS_C2LLI(type, &err);
// 		}
// /*
//   * end of reading the line, number of words is the same as required, return succes
//   */
// 				if( wc == tot_dim ) return 0;
// 			}
// 
// 			if(IFEXPR) pc++;
// 			bzero(type,sizeof(type));
// 			i = 0;
// 			lastchar = '\0';
// 		}
// 	}
// /*
//  * if upon entering function *pc == '\0' attempt to read buffer and call routine recurively
//  */
// 	bzero(buff,sizeof(buff));
// 	if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1){
// 		Perror("read");
// 		return -1;
// 	}
// 	
// 	buff[ngotten] = '\0';
// 	pc = &buff[0];
// 	
// 	if(ngotten == 0)return -1;
// 
// 	if( m3l_read_socket_data_line(Lnode, TMPSTR, descrpt, Popts) != 0){
// 		Error("Error reading data");
// 		return -1;
// 	}
// 		
// 	return 0;
// /*
//   * if you get here something went wrong
//   */	
// 	return -1;
// }

lmint_t m3l_read_socket_data_charline(node_t **Lnode, tmpstruct_t TMPSTR, lmint_t descrpt)
{
/* 
 * function reads data from FILE
 */
	lmsize_t i, tot_dim;
	lmchar_t 		*pdat;
	lmusignchar_t 	*pdatu;
	lmsignchar_t   	*pdats;

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
			if(i == 0) while(IFEXPR) pc++;	/* if in middle of reading buffer, consider all characters */
			while(*pc != '\0' && i < tot_dim){
				*pdatu++ = (lmusignchar_t)*pc++;
				i++;
			}
/*
 * find why while was left
 */
			if(*pc == '\0'){	
				bzero(buff,sizeof(buff));
				if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1)
					Perror("read");

				if(ngotten == 0)return 0; /* no more data in buffer */
				buff[ngotten] = '\0';
				pc = &buff[0];
				if(ngotten == 0)return -1;

/*
 * if this is at the same time end of reading the text (i == tot_dim) and the first character of the next buffer is IFEXPR, return
 */
				if(i == tot_dim && IFEXPR) {
					(*Lnode)->data.c[tot_dim] = '\0';
					return 0;
				}

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
		if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1){
			Perror("read");
			return -1;
		}
		
		buff[ngotten] = '\0';
		pc = &buff[0];
		if(ngotten == 0)return -1;

		if( m3l_read_socket_data_charline(Lnode, TMPSTR, descrpt) != 0){
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
			if(i == 0) while(IFEXPR) pc++;	/* if in middle of reading buffer, consider all characters */
			while(*pc != '\0' && i < tot_dim){
				*pdats++ = (lmsignchar_t)*pc++;
				i++;
			}
/*
 * find why while was left
 */
			if(*pc == '\0'){	
				bzero(buff,sizeof(buff));
				if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1)
					Perror("read");

				if(ngotten == 0)return 0; /* no more data in buffer */
				buff[ngotten] = '\0';
				pc = &buff[0];
				if(ngotten == 0)return -1;
/*
 * if this is at the same time end of reading the text (i == tot_dim) and the first character of the next buffer is IFEXPR, return
 */
				if(i == tot_dim && IFEXPR) {
					(*Lnode)->data.c[tot_dim] = '\0';
					return 0;
				}
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
		if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1){
			Perror("read");
			return -1;
		}
		
		buff[ngotten] = '\0';
		pc = &buff[0];
		if(ngotten == 0) return -1;

		if( m3l_read_socket_data_charline(Lnode, TMPSTR, descrpt) != 0){
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
 * if in middle of reading buffer, consider all characters
 */
			if(i == 0) while(IFEXPR) pc++;
/*
 * read until end of buffer or end of array dimension reached
 */
			while(*pc != '\0' && i < tot_dim){
				*pdat++ = *pc++;
				i++;
			}
/*
 * find why while was left
 */
			if(*pc == '\0'){	
				bzero(buff,sizeof(buff));
				if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1)
					Perror("read");

				if(ngotten == 0){
					return 0; /* no more data in buffer */
				}
				buff[ngotten] = '\0';
				pc = &buff[0];
				if(ngotten == 0)return -1;

/*
 * if this is at the same time end of reading the text (i == tot_dim) and the first character of the next buffer is IFEXPR, return
 */
				if(i == tot_dim && IFEXPR) {
					(*Lnode)->data.c[tot_dim] = '\0';
					return 0;
				}

			}
			else if (i == tot_dim){
				*pdat = '\0';
				if( EXPR ){
					printf("buffer is '%s'\n", buff);
					printf("pc is '%c'\n", *pc);
					printf("Data set %s (%s): string: '%s'\n", (*Lnode)->name,  (*Lnode)->type, (*Lnode)->data.c);
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
		if (  (ngotten = Read(descrpt, MAXLINE-1)) == -1){
			Perror("read");
			return -1;
		}
//  				printf("3- buffer is '%s'\n", buff);

		buff[ngotten] = '\0';
		pc = &buff[0];
		if(ngotten == 0)return -1;

		if( m3l_read_socket_data_charline(Lnode, TMPSTR, descrpt) != 0){
				Error("Error reading data");
			return -1;
		}
// 		printf(" returning E4\n");
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


lmssize_t Read(lmint_t descrpt ,lmint_t n)
{

		if (  (ngotten = read(descrpt,buff,n)) == -1){
			Perror("read");
			return -1;
		}
		buff[ngotten] = '\0';
	return ngotten;
}

