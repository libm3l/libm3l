/*
 * routine is a concurent server
 * 
 * opens socket, binds it to a port which is given as an input argument
 * and converst it to listening port
 * 
 * after receiven connect request from client, forks and spawns a child
 * which handels reques for each connecting client.
 * 
 * it then receives packets with numbers separated by comma (,) symbol
 * extraxts each number and saves it. The maximum length of the number is MAXLENGTH
 * 
 * works with TCP / IPv4LL 
 * 
 * Author: Adam Jirasek 
 * Date:   21-May-2011
 */
#include "libm3l.h"


int main(int argc, char *argv[])
{
  
	node_t *Gnode=NULL, *TmpNode=NULL, *RecNode=NULL;
	pid_t  childpid;
	size_t *dim;

    	 int sockfd, newsockfd, portno, n, status, ch_stat;

     socklen_t clilen;
     struct sockaddr_in cli_addr;
	opts_t *Popts, opts;

	char *answer="This is answer";

	opts.opt_linkscleanemptlinks = '\0';  // clean empty links
	opts.opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	opts.opt_linkscleanemptrefs = '\0'; // clean empty link references
	opts.opt_tcpencoding = 't'; // serialization and encoding when sending over TCP/IP

	Popts = &opts;

     if (argc < 2) {
       fprintf(stderr,"ERROR, no port number provided\n");
       exit(1);
     }

	portno = atoi(argv[1]);
 /*
 * create, bind and liste socket
 */
     if ( (sockfd = m3l_server_openbindlistensocket(portno, (char *)NULL) ) < 0 )
		Perror("Open_Bind_Listen");
      
/* =============================== */      

      while(1){
/*
 * accept connection
 */

	printf(" ACCEPT socket \n");
	
	clilen = sizeof(cli_addr);
	
      if ( (newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0){
       if(errno = EINTR){ /* If Interrupted system call, restart - back to while ()  UNP V1 p124  */
	perror("accept()");
	 continue;}
       else
	 Perror("accept()");
      }

		printf(" ... OK\n");

      
      if ( (childpid = Fork())  == 0) { 
/*
 * child process - Receiver/Sender 
 *
 * close listening socket
 */
      if (( status = close(sockfd)) != 0)
	 Perror("close()");


	if(  (Gnode = m3l_Mklist("Send_head_node", "DIR", 0, 0, (node_t **)NULL, (const char *)NULL, (const char *)NULL, (char *)NULL)) == 0)
		Perror("m3l_Mklist")	;

	dim = (size_t *) malloc( 1* sizeof(size_t));
		dim[0] = 15;
		
	if(  (TmpNode = m3l_Mklist("TEXT_OF_ANSWER", "C", 1, dim, &Gnode, "/Send_head_node", "./", "--no_malloc", (char *)NULL)) == 0)
			Error("m3l_Mklist");
	TmpNode->data.c = answer;

	dim[0] = 1;
	if(  (TmpNode = m3l_Mklist("PID", "I", 1, dim, &Gnode, "/Head_node", "./", (char *)NULL)) == 0)
			Error("m3l_Mklist");
	TmpNode->data.i[0] = getpid();
	
	free(dim);

// 	printf("\n\n");
// 	if( Cat(m3l_Gnode, "--all", "-P", "-L","--links",  "*",   (char *)NULL) != 0 )
// 		Error("CatData");
// 	printf("\n\n");

		printf(" Reading socket \n");


// 	if( (RecNode = m3l_read_socket(newsockfd,  Popts)) == NULL)
// 	if( (RecNode = m3l_read_socket(newsockfd,  (opts_t *)NULL)) == NULL)
	if( (RecNode = m3l_Receive_tcpipsocket((const char *)NULL, newsockfd, "--encoding" , "IEEE-754", (char *)NULL)) == NULL)
		Error("Error during reading data from socket");

	if(m3l_Cat(RecNode, "--all", "-P", "-L","--links",  "*",   (char *)NULL) != 0)
		Error("CatData");

// 		printf(" PID is %d %d\n", getpid(), getppid());


// 	sleep(1);

// 	if ( m3l_write_to_socket(1, Gnode,  newsockfd,  Popts) < 0)
// 	if ( m3l_write_to_socket(1, Gnode,  newsockfd,  (opts_t *)NULL) < 0)
	if (  m3l_Send_to_tcpipsocket(Gnode, NULL , newsockfd, "--encoding" , "IEEE-754", "--header", (char *)NULL) < 0)
		Error("Error during writing data to socket");

	printf("\n\n");
	 if (( status = close(newsockfd)) != 0)
	 	Perror("close()"); 

	if(m3l_Umount(&Gnode) != 1)
		Perror("Umount");
	if(m3l_Umount(&RecNode) != 1)
		Perror("Umount");


     exit(0);
/*
 * end of child
 */
      }
      else{
/*
 * parent process
 * close accepted socket and loop around
 */
	if (( status = close(newsockfd)) != 0)
		Perror("close()");	
		while ( (status = wait(&ch_stat)) != childpid) {
				if (status < 0 && errno == ECHILD)
					break;
				errno = 0;	
		}
	
			printf(" Grand Parent: all children are dead\n");
// 			exit(0);

     	}  


	}  /* End of while(1) */

	close(sockfd);	

     return 0; 
}
