
/*
 * Array examples
 */
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Cat.h"
#include "server_openbindlistensocket.h"
#include "ReadSocket.h"

#define LINESZ 1024

int main(void) 
{	
    node_t *Gnode=NULL, *Node, *Tmpnode;
    find_t **FoundNodes;
    
    int i, count,countgrp, sockfd,newsockfd, status;
    
     socklen_t clilen;
     struct sockaddr_in cli_addr;
    
    size_t founds;
    
    char name[255], type[30];
    char *pc,buff[LINESZ];

    int *ada, num, wc;
	pid_t childpid;

	printf("Opening socket\n");	

/*
 *	netstat -an | grep 4096	
 * 	lsof -i :4096
 */
    sockfd = openbindlistensocket(4096);

    printf("before while\n");	
	
      while(1){
	    
        clilen = sizeof(cli_addr);
/*
 * accept connection
 */

    printf("before accept\n");	

      if ( (newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0){
       if(errno = EINTR) /* If Interrupted system call, restart - back to while ()  UNP V1 p124  */
	 continue;
       else
	 Perror("accept()");
      }
      
          printf("after accept\n");	

      
      if ( (childpid = Fork())  == 0) { 
/*
 * child process - Receiver/Sender 
 *
 * close listening socket
 */
	    printf("In child, pid is %d\n", getpid());	

      if (( status = close(sockfd)) != 0)
	 Perror("close()");
      
          Gnode = read_socket(newsockfd);

          printf("\n\n\n Cat \n\n\n");
	if(Cat(Gnode, "--all", "-P", "-L", "*", (char *)NULL) != 0)
	    	Error("CatData");
	printf("\n\n\n");
      
          if(Umount(&Gnode) != 1)
	    Perror("Umount");
	printf(" GNODE unmounted\n");
     exit(0);
/*
 * end of child
 */
      }
      else{

       if (( status = close(newsockfd)) != 0)
	 Perror("close()"); 
          }
       
     }  /* End of while(1) */
     
      if (( status = close(sockfd)) != 0)
	 Perror("close()");
     
      
      	wait(&status);
	
	printf(" Grand Parent: all children are dead\n"); 
      
      exit(0);  	
	
	
	
   
 
  return 0;

}
