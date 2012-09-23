/*
 * example of client
 * Client creates a data set containing a text and array of int numbers
 * sends it to server and gets back answer with PID of server
 * then send a subset and gets back answer with PID of server
 * Author: Adam Jirasek 
 * Date:   Aug-2012
 */
#include "libm3l.h"
#include "ACK.h"


int main(int argc, char *argv[])
{
	node_t *Gnode=NULL, *RecNode=NULL, *SolverName = NULL, *TmpNode=NULL;
	pid_t  childpid;
	size_t *dim, i;

//        find_t *SFounds;

	int sockfd, portno, n, status, ch_stat;

       socklen_t clilen;
       struct sockaddr_in cli_addr;
	char *answer="This is answer";

// 	opts_t *Popts, opts;
// 
// 	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_k = '\0'; opts.opt_b = '\0';opts.opt_l = '\0';
// 	opts.opt_add = '\0';
// 	Popts = &opts;
/*
 * get port number
 */
     if (argc < 3) {
       fprintf(stderr,"ERROR, no IPaddress and port number provided\n");
       exit(1);
     }
 	portno = atoi(argv[2]);
/*
 * make "header" with solver name
 */
	SolverName = solver_name("Edge");
/*
 * open socket - because we use more then just send - receive scenario
 * we need to open socket manualy and used Send_receive function with hostname = NULL, ie. as server
 * portno is then replaced by socket number
 */	


	for(i=0; i<1000001; i++){

	printf("----------------------------------------------------------------------------------------    i = %d\n", i);


	if ( (sockfd =  m3l_cli_open_socket(argv[1], portno, (char *)NULL)) < 0)
		Error("Could not open socket");


/*
 * send solver name to server and get back ACKN answer
 */
	if( (RecNode = m3l_Send_receive_tcpipsocket(SolverName,(char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL)) == NULL)
			Perror("Send_receive");
// 	sleep(1);
/*
 * Do not care what is in RecNode, it should be ACKN
 */
	if(m3l_Umount(&RecNode) != 1)
		Perror("m3l_Umount");
/*
 * send data set to server and get back answer
 */
	Gnode = solver_name("Takovy_nejaky_text from Edge");

	dim = (size_t *) malloc( 1* sizeof(size_t));
	dim[0] = 1;
	if(  (TmpNode = m3l_Mklist("PID", "I", 1, dim, &Gnode, "/Solver", "./", (char *)NULL)) == 0)
			Error("m3l_Mklist");
	TmpNode->data.i[0] = i;
	
	free(dim);
	
	if( (RecNode = m3l_Send_receive_tcpipsocket(Gnode,(char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL)) == NULL)
			Perror("Send_receive");
/*
 * print answer set on screen and remove received data set
 */
// 	printf("\n\nserver answered:\n\n\n");
// 	if(m3l_Cat(RecNode, "--all", "-P", "-L",  "*",   (char *)NULL) != 0)
//                    Error("CatData");
	if(m3l_Umount(&RecNode) != 1)
		Perror("m3l_Umount");

	if(m3l_Umount(&Gnode) != 1)
		Perror("m3l_Umount");

	if( close(sockfd) == -1)
		Perror("close");	

	}

	if(m3l_Umount(&SolverName) != 1)
		Perror("m3l_Umount");


     return 0; 
}
