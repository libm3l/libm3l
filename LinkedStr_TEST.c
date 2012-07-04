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
 *     Function LinkedStr_TEST.c
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




/*
 * Array examples
 */
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "IO.h"
#include "Find.h"
//#include "Find_Source.h"
#include "Rm.h"
#include "Mount.h"
#include "Umount.h"
#include "Cat.h"
#include "tcpip_socket_IOop.h"
#include "WriteData.h"
#include "FunctionsPrt.h"
#include "Locate.h"
#include "Cp.h"
#include "Mklist.h"
#include "Add.h"


int main(void) 
{	
    node_t *Gnode=NULL, *RecNode, *Anode, *Tmpnode, *NewList=NULL;
    find_t *Founds;
    
    int i, count,countgrp, socketnr, j;

	size_t *dim;
    
    char name[255], type[30];
    char *pc;
    
    path_t *parsed_path;

    int  num, wc;
//     int *ada;
    int ada[5];
    get_arg_t argsstr;
    
    char dummy[1];
    double value = 42.000042; /* or anything else */
    int siz;


    FILE *fp;
    
    char *node_path;

    printf("Going to Fread1\n");
   
//     printf("%p\n ",  __builtin_return_address(0));  /" gives pointer to return address */

//     siz = snprintf(dummy, sizeof(dummy), "%lf", value);
//     printf("exact length needed to represent 'value' "
//            "(without the '\\0' terminator) is %d.\n", siz);
//         printf("%f\n\n\n", -DBL_MAX);
	printf("%d\n", snprintf(NULL, 0, "%.10f", value));
//     exit(0);

	for (j=0; j<1; j++){
		
		printf(" CYCLE %d\n\n", j);

 		if( (Gnode = Fread("ADA1" , "--clean_empy_links", (char *)NULL))  == NULL)
 			Perror("Linked_test: Fread");
		
// 		Fwrite(Gnode, "ADA", (char *)NULL);
// 		printf("\n\n\n\n");
// 		exit(0);

		
		if(Cat(Gnode, "--all", "-P", "-L","--links",  "*",   (char *)NULL) != 0)
 	                   Error("CatData");
					
		printf("\n\n\n\n");
		
 		if( (Anode = Fread("TEST.dat", (char *)NULL))  == NULL)
 			Perror("Linked_test: Fread");	
//  		if( (Anode = Fread("ADA_EMPTYLINK"))  == NULL)
//  			Perror("Linked_test: Fread");	

 		if(Cat(Anode, "--all", "-P", "-L", "--links", "*", (char *)NULL) != 0)
 	                   Error("CatData");
// exit(0);
	
// 

		
		printf("\n\n\n\n");
		
// 		Send_to_tcpipsocket(Gnode, "localhost", 4096, (char *)NULL);

// 		
// 		socketnr =  cli_open_socket("localhost", 4096);
// 		write_to_socket(1, Gnode,  socketnr);
// // 		 RecNode = Send_receive_tcpipsocket(Gnode, "localhost", 4096, (char *)NULL);
// 		close(socketnr);
		
// 		printf("printing received node RECNODE \n\n ");
		
// 		 if(Cat(RecNode, "--recursive", "--all", "-P", "-L", "*", (char *)NULL) != 0)
// 	    	Error("CatData");
		
// // 		
// // 		usleep(10000);
// // 		
// // 		exit(0);
// 		
// 		
// 		
// 		
// 		printf(" Looking for data set\n");
// 		
// 		if( ( Founds = Find(Gnode, "--recursive" ,"BBB_DATA_DADA", (char *)NULL)) == NULL){
// 
// 			printf("No subset found\n"); exit(0);
// 			}
// 		else
// 		{
// 			for(i=0; i < Founds->founds; i++){
// 			printf(" Found name is %s  %p   %s\n", Founds->Found_Nodes[i]->List->name, Founds->Found_Nodes[i]->List, Founds->Found_Nodes[i]->List->type);
// 			}
// 
// 		DestroyFound(&Founds);
// 
// 		}
// 			
// 		printf("Number of removed nodes is %ld\n", Rm(&Gnode , "--recursive" ,  "BBB_DATA_DADA", (char *)NULL) );
// // 
// // 
// 		if(Cat(Gnode, "--all", "-P", "-L", "*", (char *)NULL) != 0)
// 	                   Error("CatData");
// 						
// 		
// 		if(Umount(&Gnode) != 1)
//                   Perror("Umount");
// 		
// 		exit(0);
// // 		if(Umount(&RecNode) != 1)
// //                   Perror("Umount");
// 				
// 	}
// 		
// 
// 		exit(0);

				
// 		printf("Going to locate %p\n", Gnode);
// 		if( (Founds = Locate(Gnode, "/main/grid1/boundary", "/*/SV_name_of_grid=CFD_grid/*", "--ignore", (char *)NULL)) != NULL){
// 		if( (Founds = Locate(Gnode, "./grid1/boundary", "/*/SV_name_of_grid=CFD_grid/*", "--ignore", (char *)NULL)) != NULL){

//  		if( (Founds = Locate(Gnode, "/main/grid1/*", "/*/*/*", "--ignore", (char *)NULL)) != NULL){
//  		if( (Founds = Locate(Gnode, "/main/grid1/*", "/*/SV_name_of_grid=CFD_grid/*", "--ignore", (char *)NULL)) != NULL){
// //  		if( (Founds = Locate(Gnode, "/main/*/boundary", "/*/*/*", "--ignore", (char *)NULL)) != NULL){
//  		if( (Founds = Locate(Gnode, "/*/*", "/*/*", "--ignore", (char *)NULL)) != NULL){
			
// 			for(i=0; i < Founds->founds; i++){
// 				printf(" Found name is %s  %p   %s\n", Founds->Found_Nodes[i]->List->name, Founds->Found_Nodes[i]->List, Founds->Found_Nodes[i]->List->type);
// 
// 				if( (node_path = Path(Founds->Found_Nodes[i]->List, NULL)) != NULL){
// 					printf(" Path is %s \n", node_path);
// 					free(node_path);
// 				}
// 				
// 				 if(Cat(Founds->Found_Nodes[i]->List, "--all", "-P", "-L", "*", (char *)NULL) != 0)
// 					Error("CatData");
// 			}
// 		DestroyFound(&Founds);
// 		}
// 		else
// 		{
// 			printf(" No founds\n");
// 		}
		
		
// 		printf("Number of removed nodes is %ld\n", Rm(&Gnode , "/main/grid1/boundary", "/*/SV_name_of_grid=CFD_grid/*",  "--keepheadnode", (char *)NULL) );
// 		printf("Number of removed nodes is %ld\n", Rm(&Gnode , "/main/grid1/boundary", "/*/SV_name_of_grid=CFD_grid/n=2",  "--keepheadnode", (char *)NULL) );

			   
//  		Cp(Gnode, "/main/Additional_directory/Belonging_to_ADDDATA/BBB_DATA_DADA", "/*/*/*/*", &Gnode, "/main/grid3", "/*/*", (char *)NULL);   
//  		Cp(Gnode, "/main/grid1/boundary/name", "/*/SV_name_of_grid=CFD_grid/n=1/*", &Gnode, "/main/grid3", "/*/*", (char *)NULL);   
//  		Cp(Gnode, "/main/Additional_directory/Belonging_to_ADDDATA/BBB_DATA_DADA", "/*/*/*/n=2", &Gnode, "/main/ADA", "/*/*", (char *)NULL);   
//     		Cp(Gnode, "/main/Additional_directory/Belonging_to_ADDDATA/BBB_DATA_DADA", "/*/*/*/*", &Gnode, "/main/grid3/BBDD_NEWName", "/*/*/*", (char *)NULL);
// 		Cp(Gnode, "/main/grid1", "/*/SV_name_of_grid=CSM_grid", &Gnode, "/main/grid1/GRID_grid1", "/*/SV_name_of_grid=CFD_grid/*", (char *)NULL);   /* rename node */
// 		Cp(Gnode, "/main/grid1/name_of_grid", "/*/SV_name_of_grid=CSM_grid/N=name_of_grid", &Gnode, "/main/grid1/name_GRID_grid1", "/*/SV_name_of_grid=CSM_grid/*", (char *)NULL); 
// 		Cp(Gnode, "/main/grid1", "/*/SV_name_of_grid=CSM_grid", &Gnode, "/main/ADD_grid1", "/*/*", (char *)NULL);   /* rename node */
// 		Cp(Gnode, "/main/grid1/boundary", "/*/SV_name_of_grid=CSM_grid/SV_type=Wall", &Gnode, "BouNdaRy", "./*", (char *)NULL); 
// 		Cp(Gnode, "/main/grid1/boundary/type", "/*/SV_name_of_grid=CSM_grid/n=1/n=1", &Gnode, "/main/grid1/boundary/BouNdaRy", "/*/n=1/n=1/*", (char *)NULL); 
		Cp(&Gnode, "/main/grid1/boundary", "/*/*/SV_type=Wall", &Gnode, "BouNdaRy", "./", (char *)NULL); 
//   		Cp(Anode, "/Main_DATA_Structure/Additional_directory", "/*/*", &Gnode, "/main/grid1/boundary/typeNew", "/*/SV_name_of_grid=CSM_grid/n=2/*", (char *)NULL);   /* rename node */

				   
//   		Cp(Gnode, "/main/grid1/boundary", "/*/SV_name_of_grid=CSM_grid/SV_type=Wall", &Gnode, "/main", "/*", (char *)NULL);   


//  		Mv(&Gnode, "/main/grid1/boundary", "/*/SV_name_of_grid=CSM_grid/SV_type=Wall", &Gnode, "BouNdaRy", "./*", (char *)NULL);   /* rename node */
// 		Mv(&Gnode, "/main/grid1", "/*/SV_name_of_grid=CSM_grid", &Gnode, "/main/grid1", "/*/SV_name_of_grid=CFD_grid", (char *)NULL);   /* rename node */
// 		Mv(&Gnode, "/main/grid1/boundary/name", "/*/SV_name_of_grid=CSM_grid/n=1/n=1", &Gnode, "/main/grid1/boundary/type", "/*/SV_name_of_grid=CSM_grid/n=2/n=1", (char *)NULL);   /* rename node */
// 		Mv(&Gnode, "/main/grid1/boundary/name", "/*/SV_name_of_grid=CSM_grid/n=1/n=1", &Gnode, "/main/grid1/boundary/name_new", "/*/SV_name_of_grid=CSM_grid/n=2/*", (char *)NULL);   /* rename node */


//  		Ln(&Gnode, "/main/grid1/boundary/name", "/*/SV_name_of_grid=CSM_grid/n=1/n=1", &Gnode, "/main/grid1/boundary/type", "/*/SV_name_of_grid=CSM_grid/n=2/n=1", (char *)NULL);   /* rename node */
//   		Ln(&Gnode, "/main/grid1/boundary/name", "/*/SV_name_of_grid=CSM_grid/n=1/n=1", &Gnode, "/main/grid1/boundary/typeNew", "/*/SV_name_of_grid=CSM_grid/n=2/*", (char *)NULL);   /* rename node */
//  		Ln(&Gnode, "/main/grid1/boundary", "/*/SV_name_of_grid=CSM_grid/n=1", &Gnode, "/main/grid1/boundary/typeNew", "/*/SV_name_of_grid=CSM_grid/n=2/*", (char *)NULL);   /* rename node */
  		Ln(&Anode, "/Main_DATA_Structure/Additional_directory", "/*/*", &Gnode, "/main/grid1/boundary/typeNewLINK", "/*/SV_name_of_grid=CSM_grid/n=2/*", (char *)NULL);   /* rename node */

// 		if(Cat(Gnode,  "--all", "--links", "-P", "-L", "*", (char *)NULL) != 0)
// 			Error("CatData");
		
		printf(" \n\n\n Copying link \n\n\n");
		Cp(&Gnode, "/main/grid1/boundary/typeNewLINK", "/*/*/*/*", &Gnode, "/main/grid3", "/*/*", (char *)NULL); 
// 		Cp(Gnode, "/main/grid1/boundary/typeNewLINK", "/*/SV_name_of_grid=CSM_grid/n=1/*", &Gnode, "/main/grid3", "/*/*", (char *)NULL); 

		if(Cat(Gnode,  "--all", "--links", "-P", "-L", "*", (char *)NULL) != 0)
			Error("CatData");
		
		
		if( (Founds = Locate(Anode, "/Main_DATA_Structure/Additional_directory", "/*/*", "--ignore", (char *)NULL)) != NULL){
			
			for(i=0; i < Founds->founds; i++){
				printf(" Found name is %s  %p   %s\n", Founds->Found_Nodes[i]->List->name, Founds->Found_Nodes[i]->List, Founds->Found_Nodes[i]->List->type);

				if( (node_path = Path(Founds->Found_Nodes[i]->List, NULL)) != NULL){
					printf(" Path is %s \n", node_path);
					free(node_path);
				}
				
				 if(Cat(Founds->Found_Nodes[i]->List, "--all", "-P", "-L", "*", (char *)NULL) != 0)
					Error("CatData");
				 
				 printf("Number of links is %d\n", Founds->Found_Nodes[i]->List->lcounter);
				 for(j=0; j < Founds->Found_Nodes[i]->List->lcounter; j++)
					 printf("-  %p\n", Founds->Found_Nodes[i]->List->linknode[j]->List);
			}
		DestroyFound(&Founds);
		}
		else
		{
			printf(" No founds\n");
		}
		
		Fwrite(Gnode, "ADA", (char *)NULL);
		printf("\n\n\n\n");

// 		 if(Cat(Anode, "--all", "-P", "-L", "*", (char *)NULL) != 0)
//  	                   Error("CatData");
// 		
// 		printf("\n\n\n\n");
		
// 		printf("\n\n\n\n");
// 		if(Cat(Gnode, "--all", "-P", "-L", "*", (char *)NULL) != 0)
// 			Error("CatData");
// 		

		if(Umount(&Anode) != 1)
			Perror("Umount");
		
// 		printf("Number of removed nodes is %ld\n", Rm(&Anode , "/Main_DATA_Structure", "--clean_empty_refs_to_links", "/*", (char *)NULL) );
		
		
		if(Cat(Anode, "--all", "-P", "-L", "*", (char *)NULL) != 0)
//  	                   Error("CatData");
// 			   exit(0);

		if(Cat(Gnode,  "--all", "--links", "-P", "-L", "*", (char *)NULL) != 0)
			Error("CatData");
		Fwrite(Gnode, "ADA_EMPTYLINK", (char *)NULL);
		printf("\n\n\n\n");
		
// 		Send_to_tcpipsocket(Gnode, "localhost", 4096, (char *)NULL);

// 		printf(" Number of empty links is %ld \n", ln_cleanempytlinks(&Gnode,  (opts_t *)NULL) );
		printf(" Number of empty links is %ld \n", Ln(NULL, NULL, NULL, &Gnode, NULL, NULL, "--clean_empty_refs_to_links", (char *)NULL));	
		printf(" Number of empty links is %ld \n", Ln(NULL, NULL, NULL, &Gnode, NULL, NULL, "--clean_empty_links", (char *)NULL));	
		
		printf("\n\n\n\n CLEANING EMPTY LINKS == \n\n\n");


		dim = (size_t *) malloc( 1* sizeof(size_t));
		dim[0] = 10;
// 		if(  (NewList = Mklist("MADE_LIST", "I", 1, dim, (node_t **)NULL, (const char *)NULL, (const char *)NULL, (char *)NULL)) == 0)
	 	if(  (NewList = Mklist("MADE_LIST", "I", 1, dim, &Gnode, "/main", "./", (char *)NULL)) == 0)
			Error("Mklist");
		free(dim);
		for (i=0; i<10; i++)
 			NewList->data.i[i]=2*i;

 		if(Cat(NewList,  "--all", "--links", "-P", "-L", "*", (char *)NULL) != 0)
 			Error("CatData");
		
// 		Add(&NewList, &Gnode, "/main/grid3", "/*/*", (char *)NULL);
// 		Add(&NewList, &Gnode, "/main", "./", (char *)NULL);
			
// 		NewList = NULL;
		dim = (size_t *) malloc( 1* sizeof(size_t));
		dim[0] = 5;
// 		ada = (int *)malloc(5 * sizeof(int));
		for (i=0; i<5; i++)
 			ada[i]=3*i;
		
		if(  (NewList = Mklist("MADE_LIST_ADA", "I", 1, dim, &Gnode, "/main", "./", "--no_malloc", (char *)NULL)) == 0)
			Error("Mklist");
		free(dim);
// 		for (i=0; i<5; i++)
//  			NewList->data.i[i]=2*i;
 		NewList->data.i = ada;
		
		if(Cat(Gnode,  "--all", "--links", "-P", "-L", "*", (char *)NULL) != 0)
			Error("CatData");

		if(Umount(&Gnode) != 1)
			Perror("Umount");
		
// 		free(ada);
		
// 		if(Umount(&Anode) != 1)
// 			Perror("Umount");	
		
	}
		
		
		exit(0);
		
	
// 		
// 		
// 		exit(0);
// 		
	   
	   
/*	   if( Fwrite(Gnode,  "ADA_TEST") != 0)
		   Perror("Lin*ked_test: Fwrite");
		   */
	   
// 	     if( ( Found_Nodes = Find(Gnode, , "--recursive" ,"Belonging_to_ADDDATA", (char *)NULL)) == NULL){
  		if( ( Founds = Find(Gnode,  "--recursive" ,"boundary", (char *)NULL)) == NULL){

			printf("No subset found\n"); exit(0);
		}
		else
		{
			for(i=0; i < Founds->founds; i++){
			printf(" Found name is %s  %p   %s\n", Founds->Found_Nodes[i]->List->name, Founds->Found_Nodes[i]->List, Founds->Found_Nodes[i]->List->type);
			}
	
		DestroyFound(&Founds);

		}
		
		
		
		printf(" cut data\n");
		if(Cat(Gnode, "--recursive", "-D", "-P", "BBB_DATA_DADA", (char *)NULL) != 0)
			Error("CatData");
		printf("\n\n\n");

	   
	   printf("Number of removed nodes is %ld\n", Rm(&Gnode , "--recursive" , "--ignore", "BBB_DATA_DADA", (char *)NULL) );
	   if(Cat(Gnode, "--all", "-P", "-L", "*", (char *)NULL) != 0)
	                   Error("CatData");
	   
	   
	   	socketnr =  cli_open_socket("localhost", 4096);
		write_to_socket(1, Gnode,  socketnr);
		close(socketnr);
	   
	   
	   if(Umount(&Gnode) != 1)
                  Perror("Umount");
  	exit(0);
	   
// 	}
	   
	   
	   
//	   send_to_tcpipsocket(Gnode, "localhost", 4096);
			   
//	    RecNode = send_receive_tcpipsocket(Gnode, "localhost", 4096);
          
	    
	    printf("\n\n\n Umounting \n\n\n");

	    printf("Number of removed nodes is %ld\n", Rm(&Tmpnode , "--recursive" , "--ignore", "BBB_DATA_DADA", (char *)NULL) );
//	    if(Cat(RecNode, "--all", "-P", "-L", "*", (char *)NULL) != 0)
	                   Error("CatData");
	    
	    
	    
	   
	/*      socketnr =  cli_open_socket("localhost", 4096);
		write_to_socket(1, Gnode,  socketnr);
	      close(socketnr);
*/

	if(Umount(&Gnode) != 1)
                  Perror("Umount");
	if(Umount(&RecNode) != 1)
                  Perror("Umount");
    
//	}    
    exit(0);
    
    
    
    for (i=0; i<1000000; i++){
    
/*
 * loop over list and print it
 */
	   printf("\n\n\n Cat \n\n\n");
   if(Cat(Gnode, "-D", "-P", "-L", "*", (char *)NULL) != 0)
	    	Error("CatData");
	printf("\n\n\n");
   

   
	}


      if(Umount(&Gnode) != 1)
                  Perror("Umount");


 //  }
   
   
   	   printf("\n\n\n Cat \n\n\n");
   if(Cat(Gnode, "--recursive", "-D", "-P", "BBB_DATA_DADA", (char *)NULL) != 0)
	    	Error("CatData");
	printf("\n\n\n");

/*
 * function allocates **find_t pointer, after use must be freed
 */	  
	 printf("Looking for Additional_direcotry node\n");

   if( ( Founds = Find(Gnode, "Additional_directory", (char *)NULL)) == NULL){

	   printf("No subset found\n"); exit(0);
   }
   else
   {
	for(i=0; i < Founds->founds; i++){
		printf(" Found name is %s  %p   %s\n", Founds->Found_Nodes[i]->List->name, Founds->Found_Nodes[i]->List, Founds->Found_Nodes[i]->List->type);
	}
   }
   
   
   	 printf("Cat Additional_direcotry node\n");

        if(Cat(Founds->Found_Nodes[0]->List, "-D", "-P", "-L","*", (char *)NULL) != 0)
	    	Error("CatData");
	
	printf("writing to socklet\n");
	
	   socketnr =  cli_open_socket("localhost", 4096);
		write_to_socket(1, Founds->Found_Nodes[0]->List,  socketnr);
	   close(socketnr);

   	 printf("Removing Additional_directory node\n");

	Tmpnode = Founds->Found_Nodes[0]->List;

//        printf("Number of removed nodes is %ld\n", Rm(&Tmpnode , "--recursive" , "--ignore", "BBB_DATA_DADA", (char *)NULL) );

	printf("Number of removed nodes is %ld\n", Rm(&Founds->Found_Nodes[0]->List , "--recursive" , "*", (char *)NULL) );
	
//	if(Cat(Found_Nodes[0]->List, "-d", "-P", "*", (char *)NULL) != 0)
//	    	Error("CatData");

        printf("Freeing memory - number of founds is %ld\n", Founds->founds);
   
	DestroyFound(&Founds);

   
	printf("\n\n\n Cat \n\n\n");
	if(Cat(Gnode, "-D", "-P", "-L", "*", (char *)NULL) != 0)
	Error("CatData");
  
  
    printf("\n\n\nRemoving nodes Master head node\n\n\n");

    if(Umount(&Gnode) != 1)
	    Perror("Umount");
    
    printf("\n %p", Gnode);
	
  return 0;

}
