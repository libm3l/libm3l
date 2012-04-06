/*
 * function frees memory of the node 
 * copyright � 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "FunctionsPrt.h"
#include "udf_rm.h"
#include "ReadDescriptor.h"

#define EXPR       *pc != ' ' && *pc != '\t' && *pc != '\n' && *pc != '\0'
#define IFEXPR     *pc == ' ' || *pc == '\t' || *pc == '\n' && *pc != '\0'

static int read_data_line(node_t **, tmpstruct_t, FILE *);
static node_t *read_dir_data(tmpstruct_t , FILE *);
static node_t *read_data(FILE *);

static char *pc, buff[MAXLINE];
static size_t ngotten;

/*
 * Function read just one line from the file, disregarding comments line
 * It identifies if the line is a header of DATA or DIR list
 * If DATA type of list it allocates a node_t and calls ReadDataLine which reads the
 * actual conent of the list
 * If the list if of DIR type, it calls ReadDir routine which reads DIR types of list (recursive calling)
 */ 
node_t *read_file(FILE *fp)
{
	char type[MAX_WORD_LENGTH], lastchar;
	size_t   wc, i, hi;
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
	ngotten = fread(buff, sizeof(buff[0]), MAXLINE-1, fp);	
	buff[ngotten] = '\0';
	pc = &buff[0];
/*
 * process the string, in case it returned anything
 */
	while(ngotten)
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
					Perror("read_file - word too long");
			}
/*
 * set the last member of the string to \0
 */
			type[i] = '\0';
/*
 * save last character, if it is space, tab or \0 it means the buffer ended at end of the word
 * otherwise the buffer ended in the middle of word
 */	
			printf("ReadDir1 - i is %d',   char is '%c'   type is %s  wc is %d\n", i, *pc, type, wc);
/*
 * if the last string in previuous while loop was the end of buff - \0 set indicator of it
 */
			if(*pc == '\0'){
				hi =0;
			}
			else
			{
				hi = 1;
			}
/*
 * save the last character - if the last character was due to \0 in buffer, save the one before
 */
			if(i > 0 && *(pc+hi) == '\0') lastchar = *(pc+hi-1); /* NOTE */ 

			printf("ReadDir1 - Last Char is '%c'\n", lastchar);
/*
 * if reached the end of buff
 */
			if ( *(pc+hi) == '\0'){
/*
 * read next chunk of text file, complete the word by the rest from next chunk and put pointer at it's beggining
 */
				bzero(buff,sizeof(buff));
				ngotten = fread(buff, sizeof(buff[0]), MAXLINE-1, fp);
				buff[ngotten] = '\0';
				pc = &buff[0];
/*
 * if last character was not space, tab, new line or \0 the buffer did not contain entire word, some of it's part is in the next buffer
 * NOTE - for TCP/IP this is going to be replaced by separator 
 */
				if(lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0') continue;
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
	   				printf("ReadDir1 - Name of list is %s\n", TMPSTR.Name_Of_List);
				}
/*
 * get the type of the list
 */
				else if (wc == 2 ){
					bzero(TMPSTR.Type,sizeof(TMPSTR.Type));
					if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", type) < 0)
		   				Perror("snprintf");
					printf("ReadDir1 -Type of list is %s\n", TMPSTR.Type);
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

					printf("ReadDir1 - Number of dimensions is %ld\n", TMPSTR.ndim);
					if( (Dnode = read_dir_data(TMPSTR, fp)) == NULL)
						Perror("ReadDirData - ReadDir");
					printf("ReadDir1 - SUCCESS reading entire dir %s\n", Dnode->name);
/*
 * Return main list
 */
					return Dnode;
				}
			}
	
/*
 * if emtpy spaces and new lines after the word, move pointer
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


node_t *read_dir_data(tmpstruct_t TMPSTR, FILE *fp)
{
	size_t i;
	node_t *Dnode, *Tmpnode, *Pnode;
 
	printf("ReadDirData1 - To Allocate  type is %s\n\n", TMPSTR.Type);
/*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
	if( (Dnode = AllocateNode(TMPSTR)) == NULL){
		Error("Allocate");
	}

	printf("ReadDirData1 -  Successfully allocated Dnode %s with ndim  %ld\n\a", Dnode->name, Dnode->ndim);

	for(i=1;i<=TMPSTR.ndim; i++){
 
		printf("ReadDirData1 - Reading data set in dir %s number %ld of %ld\n",Dnode->name, i,Dnode->ndim);
		Tmpnode=NULL;	
		if ( (Tmpnode = read_data(fp)) == NULL)
			Error("ReadDirData: ReadData");

		printf("ReadDirData1 - After reading data set %s in PAR: %s \n",Tmpnode->name, Dnode->name);
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





node_t *read_data(FILE *fp)
{
	char type[MAX_WORD_LENGTH], lastchar;
	size_t   wc, i, hi;
	tmpstruct_t TMPSTR;
	node_t *Pnode;
/*
 * process the string - the algorithm is the same as in ReadDir1, comments reduced
 */
	while(ngotten)
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
					Perror("read_file - word too long");
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
				ngotten = fread(buff, sizeof(buff[0]), MAXLINE-1, fp);
				buff[ngotten] = '\0';
				pc = &buff[0];

				if(lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0')
					continue;
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
	   				printf("ReadData1 - Name of list is %s\n", TMPSTR.Name_Of_List);
				}
/*
 * get the type of the list
 */
				else if (wc == 2 ){
					bzero(TMPSTR.Type,sizeof(TMPSTR.Type));
					if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", type) < 0)
		   				Perror("snprintf");
					printf("ReadData1 - Type of list is %s\n", TMPSTR.Type);
				}
/*
 * get the number, in case of DIR number is a number of items in DIR, in case of DATA, number is a number of dimensions
 */
				else if (wc == 3){
					TMPSTR.ndim = Strol(type);
					TMPSTR.dim=NULL;
					printf("ReadData1 - Number of dimensions is %ld\n", TMPSTR.ndim);

/*
 * if type is FILE, allocate field for its dimensions
 */
					if ( strncmp(TMPSTR.Type,"DIR",3) != 0){

						if( (TMPSTR.dim=malloc(TMPSTR.ndim * sizeof(int))) == NULL)
	  						Perror("malloc");
					}
					else
					{
/*
 * if type is DIR, read it
 */
						if( (Pnode = read_dir_data(TMPSTR, fp)) == NULL)
							Perror("ReadData1 - ReadDir");
						return Pnode;
					}
				}
				else if ( wc > 3 && strncmp(TMPSTR.Type,"DIR",3) != 0){
					TMPSTR.dim[wc - 4] = Strol(type);
					printf("ReadData1 - Dimension #  %d is  %d\n", wc - 3, TMPSTR.dim[wc-4]);
					if( (wc - 3) == TMPSTR.ndim) break;
				}
			}

			if(IFEXPR) pc++;
			bzero(type,sizeof(type));
			i = 0;
			lastchar = '\0';
		}		

		printf("ReadData1 -  reading data line\n");
		
		if( (Pnode = AllocateNode(TMPSTR)) == NULL){
			Error("Allocate");}
	
		if( read_data_line(&Pnode, TMPSTR, fp) != 0)
			Error("Error reading data");

		if(TMPSTR.dim!=NULL)
		{
			free(TMPSTR.dim);
			TMPSTR.dim = NULL;
		}

		printf("ReadData1 -  success reading data of node %s  -- RETURNIGN\n", Pnode->name);
/*
 * Return main list
 */
		return Pnode;
 /*
  * end of reading the lists in DIR list   - while(ngotten)
  */
	}
/*
 * If you get here something is wrong
 */
	return NULL;
}



int read_data_line(node_t **Lnode, tmpstruct_t TMPSTR, FILE *fp)
{
	char type[MAX_WORD_LENGTH], lastchar;
	size_t i, tot_dim, wc, hi;
	int *pdat;
	
	tot_dim = 1;
	
	for(i=0; i<TMPSTR.ndim; i++)
		tot_dim = tot_dim * TMPSTR.dim[i];
	
	pdat = (*Lnode)->data.i;	
/*
 * process buffer
 */
	while(ngotten)
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
					Perror("read_file - word too long");
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

			if(i > 0 && *(pc+hi) == '\0') lastchar = *(pc+hi-1); /* NOTE */ 
			
			if ( *(pc+hi) == '\0'){
/*
 * read next chunk of text file, complete the word by the rest from next chunk
 */
				bzero(buff,sizeof(buff));
				ngotten = fread(buff, sizeof(buff[0]), MAXLINE-1, fp);
				buff[ngotten] = '\0';
				pc = &buff[0];

				if(lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0') continue;
			}
/*
 * if word is longer then 0
 */			
			
			if(strlen(type) >0){
 				wc++;
/*
 * get the value
 */
				printf("ReadDataLine1 -  number is %d   %s      -    %d\n", wc, type, atoi(type));
				*pdat++ = atoi(type);
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
  * if you get here something went wrong
  */	
	return -1;
}
