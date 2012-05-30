
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


#define LINESZ 1024

int main(void) 
{	
    node_t *Gnode=NULL, *Node, *Tmpnode, *RecNode;
    find_t *Founds;
    
    int i, count,countgrp, socketnr, j;
    
    char name[255], type[30];
    char *pc,buff[LINESZ];
    
    path_t *parsed_path;

    int *ada, num, wc;
    get_arg_t argsstr;

    FILE *fp;
    
    char *node_path;

    printf("Going to Fread1\n");
   

	for (j=0; j<1; j++){
		
		printf(" CYCLE %d\n\n", j);

 		if( (Gnode = Fread("INPUT_TEST"))  == NULL)
 			Perror("Linked_test: Fread");
		
	
		
// 		Fwrite(Gnode, "ADA");
		
		
// 
 		if(Cat(Gnode, "--all", "-P", "-L", "*", (char *)NULL) != 0)
 	                   Error("CatData");
// 		
// 		socketnr =  cli_open_socket("localhost", 4096);
// // 		write_to_socket(1, Gnode,  socketnr);
// 		 RecNode = send_receive_tcpipsocket(Gnode, "localhost", 4096);
// 		close(socketnr);
// 		
// 		printf("printing received node RECNODE \n\n ");
// 		
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
// 		if( (Founds = Locate(Gnode, "/main/grid1/*", "/*/SV_name_of_grid=CFD_grid/*", "--ignore", (char *)NULL)) != NULL){
// //  		if( (Founds = Locate(Gnode, "/main/*/boundary", "/*/*/*", "--ignore", (char *)NULL)) != NULL){
// //  		if( (Founds = Locate(Gnode, "/*/*", "/*/*", "--ignore", (char *)NULL)) != NULL){
// 			
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
// 		Cp(Gnode, "/main/grid1/boundary/name", "/*/SV_name_of_grid=CFD_grid/n=1/*", &Gnode, "/main/grid3", "/*/*", (char *)NULL);   
//  		Cp(Gnode, "/main/Additional_directory/Belonging_to_ADDDATA/BBB_DATA_DADA", "/*/*/*/n=2", &Gnode, "/main/ADA", "/*/*", (char *)NULL);   
			   
				   
		Cp(Gnode, "/main/grid1/boundary", "/*/SV_name_of_grid=CSM_grid/*", &Gnode, "/main", "/*", (char *)NULL);   

// 		
		 		if(Cat(Gnode, "--all", "-P", "-L", "*", (char *)NULL) != 0)
 	                   Error("CatData");
// 		
		if(Umount(&Gnode) != 1)
                  Perror("Umount");
		
// 		exit(0);
		
		
// 		parsed_path = parse_path("/home/jka/ada/");
// 		printf(" Number of segments is %ld\n",parsed_path->seg_count );
// 		for (i=0; i< parsed_path->seg_count; i++)
// 			printf(" Segment %d is %s\n", i, parsed_path->path[i]);
// 		destroy_pars_path(&parsed_path);
		
// 		exit(0);
		
	}
			
		exit(0);

		
		
// 		parsed_path = parse_path("~/home/jka/ada/");
// //   		parsed_path = parse_path("~/../../*/N=1-3,5/SI_name=Wall/");
// 
// 		printf(" Number of segments is %ld\n",parsed_path->seg_count );
// 		for (i=0; i< parsed_path->seg_count; i++)
// 			printf(" Segment %d is %s\n", i, parsed_path->path[i]);
// 		
// 		printf(" path is %c \n ", parsed_path->abspath);
// 		
// 		
// 		argsstr = get_arguments(parsed_path->path[3]);
// 		printf("'%c'  %c   '%s'  '%s'\n", argsstr.first, argsstr.arg, argsstr.s_name, argsstr.args);
// 		argsstr = get_arguments(parsed_path->path[4]);
// 		printf("'%c'  %c   '%s'  '%s'\n", argsstr.first, argsstr.arg, argsstr.s_name, argsstr.args);
// 		argsstr = get_arguments(parsed_path->path[5]);
// 		printf("'%c'  %c   '%s'  '%s'\n", argsstr.first, argsstr.arg, argsstr.s_name, argsstr.args);
// // 		
// 		destroy_pars_path(&parsed_path);
		
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


	exit(0);
   	
	printf(" name of directory");
	scanf("%s", name);
	printf("name is %s\n", name); 
	
	i=0;
	count = 0;
	countgrp = 0;
	while(name[i] != '\0'){
	while(name[i++] == '.'){
	
        count++;

	  if(count > 2) perror("Counter");

	  if(name[i+1] == '/'){
             countgrp++;
             count = 0;
          }

	  }
	}
	  
	  printf("length of the name is %d,  groups %d \n", i-1,  countgrp);	
	
  return 0;

}
