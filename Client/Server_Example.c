/*
 * example of simple for-per request server.
 * Server receives a data set, print it out and send back an answer with it's PID number 
 * Author: Adam Jirasek 
 * Date:   Aug-2012
 */
#include "libm3l.h"

int main(int argc, char *argv[])
{
	node_t *Gnode=NULL, *TmpNode=NULL, *RecNode=NULL;
	pid_t  childpid;
	size_t *dim;

    	 int sockfd, newsockfd, portno, n, status, ch_stat;
	int sum, i, tot_dim, *tmpint;

       socklen_t clilen;
       struct sockaddr_in cli_addr;
	char *answer="This is answer";

       find_t *SFounds;
	opts_t *Popts, opts;

	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_k = '\0'; opts.opt_b = '\0';opts.opt_l = '\0';
	opts.opt_add = '\0';
	Popts = &opts;

/*
 * get port number
 */
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
	clilen = sizeof(cli_addr);
	
      if ( (newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0){
          if(errno = EINTR){ /* If Interrupted system call, restart - back to while ()  UNP V1 p124  */
	  perror("accept()");
	 continue;}
       else
	 Perror("accept()");
      }


      if ( (childpid = Fork())  == 0) { 
/*
 * child process - Receiver/Sender 
 *
 * close listening socket
 */
      if (( status = close(sockfd)) != 0)
	 Perror("close()");
/*
 * create data set with answer
 * creating data sets require array with dimensions. 
 */

/*
 * create head node, it's parent is NULL
 */
	if(  (Gnode = m3l_Mklist("Send_head_node", "DIR", 0, 0, (node_t **)NULL, (const char *)NULL, (const char *)NULL, (char *)NULL)) == 0)
		Perror("m3l_Mklist")	;
/*
 * malloc array with dimensions, 
 * array is one dimensional, malloc array[1] and fill it with dimension
 */
	dim = (size_t *) malloc( 1* sizeof(size_t));
	dim[0] = strlen(answer)+1;;
/*
 * crate list TEXT_OF_ANSWER and add it to main list unde Send_head_node
 * because the list points to already existing array (answer) create with --no_malloc option
 */
	if(  (TmpNode = m3l_Mklist("TEXT_OF_ANSWER", "C", 1, dim, &Gnode, "/Send_head_node", "./", "--no_malloc", (char *)NULL)) == 0)
			Error("m3l_Mklist");
	TmpNode->data.c = answer;
/*
 * create another list name of PID, it will contain an integer number
 * fill the data set with PID of the process
 */
	dim[0] = 1;
	if(  (TmpNode = m3l_Mklist("PID", "I", 1, dim, &Gnode, "/Head_node", "./", (char *)NULL)) == 0)
			Error("m3l_Mklist");
	TmpNode->data.i[0] = getpid();
/*
 * answer is complete, free dim
 */
	free(dim);
/*
 * wait until client send the data
 */
	if( (RecNode = m3l_Receive_tcpipsocket((const char *)NULL, newsockfd, "--encoding" , "IEEE-754", (char *)NULL)) == NULL)
		Error("Error during reading data from socket");
/*
 * ... once it sends the data, print them on the screen
 */		
	printf("\n\n   Server: received data set from client \n");
	if(m3l_Cat(RecNode, "--all", "-P", "-L","--links",  "*",   (char *)NULL) != 0)
		Error("CatData");
	printf("\n\n------------------------------------------------------------------------------------------------\n\n\n");

/*
 * find data set Pressures and if it exists calculate summ and average value
 * if data set does not exist you may get warning "Warning:  Wrong absolute path", just ignore it
 */
	if ( (SFounds = m3l_locator_caller(RecNode, "/main/Pressures", "/*/*", Popts)) != NULL){
/*
 * if data set found calculate sum and average value. We know the array is integer otherwise 
 * it can be found from 
 */
	TmpNode = SFounds->Found_Nodes[0]->List;
	sum = 0;
	tot_dim = 1;

/*
 * get total array dimension
 */
// 	for(i=0; i<TmpNode->ndim; i++)
// 		tot_dim = tot_dim * TmpNode->fdim[i];
	
	tot_dim = m3l_get_List_totdim(TmpNode);
/*
 * calculate sum and average
 */
	tmpint = (int *)m3l_get_data_pointer(TmpNode);		

	for (i=0; i<tot_dim; i++) 
// 		sum = sum + TmpNode->data.i[i];
		sum = sum + tmpint[i];

	printf(" \n\nSum is   %d,  average value is %d\n", sum, sum/tot_dim);

	m3l_DestroyFound(&SFounds);
	printf("\n\n------------------------------------------------------------------------------------------------\n\n\n");




	}




/*
 * send answer to client, before that print answer on screen
 */
	printf("\n\n   Server: my answer to client is \n");
	if( m3l_Cat(Gnode, "--all", "-P", "-L","--links",  "*",   (char *)NULL) != 0 )
		Error("CatData");
	printf("\n\n");

	if (  m3l_Send_to_tcpipsocket(Gnode, NULL , newsockfd, "--encoding" , "IEEE-754", (char *)NULL) < 0)
		Error("Error during writing data to socket");
/*
 * close newsockfd
 */
	 if (( status = close(newsockfd)) != 0)
	 	Perror("close()"); 
/*
 * remove both received and sent data sets
 */
	if(m3l_Umount(&Gnode) != 1)
		Perror("Umount");
	if(m3l_Umount(&RecNode) != 1)
		Perror("Umount");

	printf("\n\n------------------------------------------------------------------------------------------------\n\n\n");

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
     	}  


	}  /* End of while(1) */

	close(sockfd);	

     return 0; 
}
