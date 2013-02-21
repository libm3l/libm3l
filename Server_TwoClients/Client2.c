/*
 * example of client2
 * Client2 receives data from Client1, calculates average number of double array
 * and send back this number and its own PID
 * Author: Adam Jirasek 
 * Date:   Aug-2012
 */
#include "libm3l.h"
#include "ACK.h"


int main(int argc, char *argv[])
{
	node_t *Gnode=NULL, *RecNode=NULL, *SolverName = NULL, *TmpNode=NULL;
	pid_t  childpid;
	size_t *dim, i, j;
	ssize_t n1;
	char buff[MAXLINE];

        find_t *SFounds;

	int sockfd, portno, n, status, ch_stat, nmax, pmax, *tmpint;

        socklen_t clilen;
        struct sockaddr_in cli_addr;
	char *answer="This is answer";

	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 100000000L;    /* 0.1 secs */
// 	tim.tv_nsec = 5000000L;
	double ave;
	
	double *tmpdf;

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
 * make "header" with solver name, Server will use it to direct messages
 */
	SolverName = client_name("Client2");
/*
 * open socket - because we use more then just send - receive scenario
 * we need to open socket manualy and used Send_receive function with hostname = NULL, ie. as server
 * portno is then replaced by socket number
 */	
	for(i=0; i<nmax; i++){

		printf("\n\n--------------------------------    i = %ld\n\n", i);
	
		if(i == nmax-1){
/*
 * if last iteration, add STOP to the message and set its value to 1
 */
			dim = (size_t *) malloc( 1* sizeof(size_t));
			dim[0] = 1;
	
			if(  (TmpNode = m3l_Mklist("STOP", "I", 1, dim, &SolverName, "/Client_Data", "./",  (char *)NULL)) == 0)
				Error("m3l_Mklist");
			tmpint = (int *)m3l_get_data_pointer(TmpNode);
			tmpint[0] = 1;
			free(dim);
		}
	
		if ( (sockfd =  m3l_cli_open_socket(argv[1], portno, (char *)NULL)) < 0)
			Error("Could not open socket");
/*
 * send solver name to server and get back ACKN answer
 */
		if( m3l_Send_to_tcpipsocket(SolverName,(char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL) < 1)
			Perror("Send_receive");
/*
 * Do not care what is in RecNode, it should be ACKN, we just need it to terminate data stream
 *
 * send data set to server and get back answer - this data set conmtains payload, ie. "real, useful" data
 * solver name makes a node, other data will be added to this node
 */
		Gnode = client_name("Text from Client2");
	
		dim = (size_t *) malloc( 1* sizeof(size_t));
		dim[0] = 1;
/*
 * add PID number of this process
 */
		if(  (TmpNode = m3l_Mklist("PID", "I", 1, dim, &Gnode, "/Client_Data", "./", (char *)NULL)) == 0)
				Error("m3l_Mklist");
		tmpint = (int *)m3l_get_data_pointer(TmpNode);

		tmpint[0] = getpid();
		free(dim);
/* 
 * Receive data from Client1
 */
		if( (RecNode = m3l_Receive_tcpipsocket( (char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL)) == NULL)
				Perror("Send_receive");
/*
 * wait 0.05 secs 
 */
	
		if(nanosleep(&tim , &tim2) < 0 )
			Error("Nano sleep system call failed \n");
/*
 * print data set from Client1 on screen 
 */
		if(m3l_Cat(RecNode, "--all", "-P", "-L",  "*",   (char *)NULL) != 0)
			Error("CatData");
/*
 * find pressure in Client1 data set, calculate the average value and print on the screen
 * when locating data set. m3l_Locate returns allocated structure with found nodes. 
 * we expect that there is only one data set called pressure and it is one dimensional array of double numbers
 * otherwise we can find out how many data sets with name "pressure" were found,
 * what is their type and dimensions
 * Do not forget free borrowed memory
 */
		if( (SFounds = m3l_Locate(RecNode, "/Client_Data/numbers", "/*/*", (char *)NULL)) != NULL){

			TmpNode = m3l_get_Found_node(SFounds, 0);
			pmax = m3l_get_List_totdim(TmpNode); 
			tmpdf = (double *)m3l_get_data_pointer(TmpNode);
			
			ave = 0;

			for(j=0; j<pmax;j++)
				ave = ave + tmpdf[j];

			ave = ave/pmax;

			printf("\n\n Average value of pressures is %.16lf\n", ave);
/*
 * add this value to answer which will be later send to Client1
 */
			dim = (size_t *) malloc( 1* sizeof(size_t));
			dim[0] = 1;

			if(  (TmpNode = m3l_Mklist("Average_Value", "D", 1, dim, &Gnode, "/Solver", "./", (char *)NULL)) == 0)
					Error("m3l_Mklist");
			TmpNode->data.df[0] = ave;
			free(dim);
/*
 * free borrowed memory, 
 */
			m3l_DestroyFound(&SFounds);
		}
		else
		{
			printf(" No founds\n");
		}
/*
 * send answer to Client1, the answer contains the average value of pressures
 */
		if( m3l_Send_to_tcpipsocket(Gnode,(char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL) < 1)
			Perror("Send_receive");
/* 
 * clean borrowed memory
 */
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
