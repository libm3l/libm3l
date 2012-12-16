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
	node_t *Gnode=NULL, *RecNode=NULL, *SolverName = NULL, *Solver1Name = NULL, *TmpNode=NULL;
	pid_t  childpid;
	size_t *dim, i;
	ssize_t n1;
	char buff[MAXLINE];


//        find_t *SFounds;

	int sockfd, portno, n, status, ch_stat, *tmpint;

        socklen_t clilen;
        struct sockaddr_in cli_addr;
	char *answer="This is answer";

	struct timespec tim, tim2;
	tim.tv_sec = 0;
//	tim.tv_nsec = 100000000L;    /* 0.1 secs */
	tim.tv_nsec = 5000000L;


	int nmax;

	nmax = 5;

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
	SolverName = solver_name("TRINITAS_REC");
	Solver1Name = solver_name("TRINITAS_SEND");
/*
 * open socket - because we use more then just send - receive scenario
 * we need to open socket manualy and used Send_receive function with hostname = NULL, ie. as server
 * portno is then replaced by socket number
 */	
	for(i=0; i<nmax; i++){

		printf("----------------------------------------------------------------------------------------    i = %d\n", i);

		if(i == nmax-1){

			dim = (size_t *) malloc( 1* sizeof(size_t));
			dim[0] = 1;
			
			if(  (TmpNode = m3l_Mklist("STOP", "I", 1, dim, &SolverName, "/Solver", "./",  (char *)NULL)) == 0)
				Error("m3l_Mklist");
			TmpNode->data.i[0] = 1;
			
			free(dim);

		}

		if ( (sockfd =  m3l_cli_open_socket(argv[1], portno, (char *)NULL)) < 0)
			Error("Could not open socket");
/*
 * send solver name to server and get payload from Client1
 */
		if( (RecNode = m3l_Send_receive_tcpipsocket(SolverName,(char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL) ) == NULL)
			Perror("Send_receive");

		if(m3l_Cat(RecNode, "--all", "-P", "-L",  "*",   (char *)NULL) != 0)
			Error("CatData");
/*
 * print answer set on screen and remove received data set
 */		
		if(m3l_Umount(&RecNode) != 1)
			Perror("m3l_Umount");
		if(nanosleep(&tim , &tim2) < 0 )
			Error("Nano sleep system call failed \n");
		

		if( close(sockfd) == -1)
			Perror("close");
/*
 * reopen socket and send answer
 */
		Gnode = solver_name("Takovy_nejaky_text from CSM");

		dim = (size_t *) malloc( 1* sizeof(size_t));
		dim[0] = 1;
		if(  (TmpNode = m3l_Mklist("PID", "I", 1, dim, &Gnode, "/Solver", "./", (char *)NULL)) == 0)
				Error("m3l_Mklist");
		
		tmpint = (int *)m3l_get_data_pointer(TmpNode);
		tmpint[0] = getpid()*i;
		
		free(dim);
		
		if ( (sockfd =  m3l_cli_open_socket(argv[1], portno, (char *)NULL)) < 0)
			Error("Could not open socket");
		
		if( (RecNode = m3l_Send_receive_tcpipsocket(Solver1Name,(char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL) ) == NULL)
			Perror("Send_receive");
/*
 * Do not care what you receive it is ACKN
 * Send payload data
 */
		if(m3l_Send_to_tcpipsocket(Gnode,(char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL) < 1)
			Perror("Send_receive");
		
		if(m3l_Umount(&Gnode) != 1)
			Perror("m3l_Umount");

		if( close(sockfd) == -1)
			Perror("close");
	}


	if(m3l_Umount(&SolverName) != 1)
		Perror("m3l_Umount");
	if(m3l_Umount(&Solver1Name) != 1)
		Perror("m3l_Umount");
     return 0; 
}
