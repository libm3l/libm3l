
/*
 * Array examples
 */
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "IO.h"
#include "Find.h"
#include "Find_Source.h"
#include "Rm.h"
#include "Mount.h"
#include "Umount.h"
#include "Cat.h"
#include "tcpip_socket_op.h"

#define LINESZ 1024

int main(void) 
{	
    node_t *Gnode=NULL, *Node, *Tmpnode;
    find_t **FoundNodes;
    
    int i, count,countgrp, socketnr;
    size_t founds;
    
    char name[255], type[30];
    char *pc,buff[LINESZ];

    int *ada, num, wc;

    FILE *fp;

    printf("Going to Fread1\n");
   

//	for (i=0; i<1000000; i++){

    if( (Gnode = Fread("TEST.dat"))  == NULL)
      Perror("Linked_test: Fread");
    
        printf("\n\n\n Umounting \n\n\n");


	   if(Cat(Gnode, "-D", "-P", "-L", "*", (char *)NULL) != 0)
	                   Error("CatData");
	   
	   
	   
	   send_to_tcpipsocket(Gnode, "localhost", 4096);

	   
	/*      socketnr =  cli_open_socket("localhost", 4096);
		write_to_socket(1, Gnode,  socketnr);
	      close(socketnr);
*/

	if(Umount(&Gnode) != 1)
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
   
   
   
   socketnr =  cli_open_socket("localhost", 4096);
   write_to_socket(1, Gnode,  socketnr);
   close(socketnr);
   
   
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

//    if( ( FoundNodes = Find(Gnode, &founds,"-r","BEDA", (char *)NULL)) == NULL){
   if( ( FoundNodes = Find(Gnode, &founds , "Additional_directory", (char *)NULL)) == NULL){
//  if( ( FoundNodes = Find(Gnode, &founds, "-r",  "*", (char *)NULL)) == NULL){

	   printf("No subset found\n"); exit(0);
   }
   else
   {
	for(i=0; i < founds; i++){
		printf(" Found name is %s  %p   %s\n", FoundNodes[i]->List->name, FoundNodes[i]->List, FoundNodes[i]->List->type);
	}
   }
   
   
   	 printf("Cat Additional_direcotry node\n");

        if(Cat(FoundNodes[0]->List, "-D", "-P", "-L","*", (char *)NULL) != 0)
	    	Error("CatData");
	
	printf("writing to socklet\n");

	sleep(5);
	
	   socketnr =  cli_open_socket("localhost", 4096);
		write_to_socket(1, FoundNodes[0]->List,  socketnr);
	   close(socketnr);

   	 printf("Removing Additional_directory node\n");

	Tmpnode = FoundNodes[0]->List;

//        printf("Number of removed nodes is %ld\n", Rm(&Tmpnode , "--recursive" , "--ignore", "BBB_DATA_DADA", (char *)NULL) );

	printf("Number of removed nodes is %ld\n", Rm(&FoundNodes[0]->List , "--recursive" , "*", (char *)NULL) );
	
//	if(Cat(FoundNodes[0]->List, "-d", "-P", "*", (char *)NULL) != 0)
//	    	Error("CatData");

        printf("Freeing memory - number of founds is %ld\n", founds);
   
	DestroyFound(FoundNodes, founds);

   
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
