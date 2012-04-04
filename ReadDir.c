/*
 * function frees memory of the node 
 * copyright � 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "FunctionsPrt.h"
#include "udf_rm.h"
#include "ReadData.h"
#include "ReadDir.h"

/*
 * Function rads line from the file which MUST BE DIR type of List (the function is called
*  at the very beginning of the file reading and each file MUST start with DIR list or
*  upon reaching enbedded List type of DIR (called recursivelly from ReadData))
*
 * Function allocates list for DIR type of data
 * and then reads everything in the DIR. If
 * DIR contains DIR, function is called recursivelly
 */ 

node_t *ReadDir(FILE *fp)
{
	char *pc,buff[MAXLINE], type[MAX_TYPE_LENGTH];
	size_t   wc, i;
	int COMMLINE;
	tmpstruct_t TMPSTR;
	node_t *Dnode;

	int tot_dim, *tmppoint; 
/*
 * 1. -----------   read info about list (on one line)
 * 			Parameters are as follows:
 *                      DIRECTORY:
 * 				Name of list
 * 				DIR keyword
 * 				number of items in directory
 *
 * Example:   
 * 			Main_data_strucure DIR  5
 */
	while (   ( fgets(buff, MAXLINE, fp) != NULL) ) { 
		bzero(type,sizeof(type));
		pc = &buff[0];
		i = 0;
		wc = 0;
		COMMLINE= 1;
/*
 * read until the end of line
 */
		while(*pc != '\n'){
			while(*pc != ' ' && *pc != '\t'){
/*
 * if comment line, ignore
 */
	 		 	if( *pc == '!'){
	 				COMMLINE = 0;
					break;
				}
	    			type[i++] = *pc++;
				if(*pc == '\n') break;
			}
/*
 * comment line, do not interpret what is in it
 */	   
			if(COMMLINE == 0)break;
	
			if(strlen(type) >0){
/*
 * increase word count
 */
 				wc++;
/*
 * get the name of the list
 */
				if(wc == 1){
					bzero(TMPSTR.Name_Of_List,sizeof(TMPSTR.Name_Of_List));
					if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", type) < 0)
		   				Perror("snprintf");
	   				printf("Name of list is %s\n", TMPSTR.Name_Of_List);
				}
/*
 * get the type of the list
 */
				else if (wc == 2 ){
					bzero(TMPSTR.Type,sizeof(TMPSTR.Type));
					if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", type) < 0)
		  	 		Perror("snprintf");
					printf("Type of list is %s\n", TMPSTR.Type);
				}
/*
 * get the number, in case of DIR number is a number of items in DIR, in case of DATA, number is a number of dimensions
 */
				else if (wc == 3){
					TMPSTR.ndim = Strol(type);  
					TMPSTR.dim=NULL;}
				}
		
			if(*pc++ == '\n') break;
			bzero(type,sizeof(type));
			i = 0;
/*
 * end-of-reading the line   -- while(*pc != '\n'){ loop
 */
 		}
/*
 * comment line, do not take any action
 */
		if(COMMLINE == 0)continue;
		printf("Number of dimensions is %ld\n", TMPSTR.ndim);
/*
 * allocate note_t pointer and read all data in DIR strucutre
 */
		if( (Dnode = ReadDirData(TMPSTR, fp)) == NULL)
			Perror("ReadDirData - ReadDir");
 /*
  * end of reading the lists in DIR list   - while (   ( fgets(buff, MAXLINE, fp) != NULL) ) {  -- loop
  */
	}
/*
 * Return main list
 */
	return Dnode;
}


node_t *ReadDirData(tmpstruct_t TMPSTR, FILE *fp)
{
	size_t i;
	char buff[MAXLINE];
	node_t *Dnode, *Tmpnode, *Pnode;
 
	printf("DIR - To Allocate  type is %s\n\n", TMPSTR.Type);
/*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
	if( (Dnode = AllocateNode(TMPSTR)) == NULL){
		Error("Allocate");
	}

	printf("Successfully allocated Dnode %ld\n\a", Dnode->ndim);

	for(i=1;i<=TMPSTR.ndim; i++){
 
		printf("Reading data set number %ld of %ld\n",i,Dnode->ndim);
		Tmpnode=NULL;	
		if ( (Tmpnode = ReadData(fp)) == NULL)
			Error("ReadDirData: ReadData");

		printf("After reading ReadData\n");
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