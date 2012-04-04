/*
 * function frees memory of the node 
 * copyright � 2012 Adam Jirasek  C
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "FunctionsPrt.h"
#include "ReadData.h"
#include "udf_rm.h"
#include "ReadDir.h"

static int ReadDataLine(node_t **, tmpstruct_t, FILE *);

/*
 * Function read just one line from the file, disregarding comments line
 * It identifies if the line is a header of DATA or DIR list
 * If DATA type of list it allocates a node_t and calls ReadDataLine which reads the
 * actual conent of the list
 * If the list if of DIR type, it calls ReadDir routine which reads DIR types of list (recursive calling)
 */ 
node_t *ReadData(FILE *fp)
{
	char *pc,buff[MAXLINE], type[MAX_TYPE_LENGTH];
	size_t   wc, i;
	int COMMLINE;
	tmpstruct_t TMPSTR;
	node_t *Pnode;
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
			while(*pc != ' ' && *pc != '\t'){  /*avoid having empty spces and tabs */
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
	
//	printf("Type is: '%s'  length is %ld\n", type,strlen(type) );

	  
			if(strlen(type) >0){
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
					TMPSTR.dim=NULL;
	 
					if ( strncmp(TMPSTR.Type,"DIR",3) != 0){
						if( (TMPSTR.dim=malloc(TMPSTR.ndim * sizeof(int))) == NULL)
	  						Perror("malloc");
					}
				}
				else if ( wc > 3 && strncmp(TMPSTR.Type,"DIR",3) != 0){
					TMPSTR.dim[wc -4] = Strol(type);
				}
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
	
		if(strncmp(TMPSTR.Type,"DIR",3) != 0){
			for(i=0; i<TMPSTR.ndim; i++){
				printf("Dimension number %ld has value %ld\n", i, TMPSTR.dim[i]);
			}
		}
/*
 * allocate structure and add it to the main node
 */

		printf("To Allocate  type is %s\n\n", TMPSTR.Type);
		if(strncmp(TMPSTR.Type,"DIR",3) != 0){
/*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
		if( (Pnode = AllocateNode(TMPSTR)) == NULL){
			Error("Allocate");}
	
			printf("Read DATA:   Successfully allocated Pnode %ld\n\a", Pnode->ndim);
/*
 * if not DIR, read data from file
 */
			if(strncmp(TMPSTR.Type,"DIR",3) != 0){
				 if( ReadDataLine(&Pnode, TMPSTR, fp) != 0)
	  				 Error("Error reading data");
			}
		}       /*if(strncmp(TMPSTR.Type,"DIR",3) != 0)*/
     		else
		{
/*
 * DIR type of data
 */
			printf("READING DIR FROM READING DATA\n");
			if( (Pnode = ReadDirData(TMPSTR, fp)) == NULL)
				Perror("ReadData - ReadDir");
			printf("AFTER READING DIR FROM READING DATA\n");
	
		}
		if(TMPSTR.dim!=NULL) free(TMPSTR.dim);

		return Pnode;
 /*
  * end of reading the lists in DIR list   - while (   ( fgets(buff, MAXLINE, fp) != NULL) ) {  -- loop
  */
	}
}



int ReadDataLine(node_t **Lnode, tmpstruct_t TMPSTR, FILE *fp)
/*
 * function reads data of the FILE node type of list
 */
{
	size_t i, tot_dim;
	int *pc;
	char buff[MAXLINE];
/*
 * get the number of elements
 */
	if(TMPSTR.ndim > 1){
		tot_dim = 1;
		for(i=0; i<TMPSTR.ndim; i++){
			tot_dim = tot_dim * TMPSTR.dim[i];
		}
	}
      else
	{
		tot_dim = TMPSTR.dim[0];
	}
/*
 * read data
 */
	pc = (*Lnode)->data.i;

	for (i=0; i< tot_dim; i++){
		printf("Reading %ld of %ld\n", i,tot_dim);
//		if (fscanf(fp, "%d", &(*Lnode)->data.i[i]) == EOF)
		if (fscanf(fp, "%d", pc++ ) == EOF)
			Perror("fscanf");
  	}
/*
 * read \n - newline symbol at the end of the line
 */
	while ( fgetc(fp) != '\n');
	return 0;
}
