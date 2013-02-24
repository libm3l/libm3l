/*
 *     Copyright (C) 2012  Adam Jirasek
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *     
 *     contact: libm3l@gmail.com
 * 
 */



/*
 *     Function Client1.c
 *
 *     Date: 2013-02-23
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
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 * 
 */




#include "libm3l.h"
#include "ACK.h"


int main(int argc, char *argv[])
{
	node_t *Gnode=NULL, *RecNode=NULL, *SolverName = NULL, *TmpNode=NULL;
	pid_t  childpid;
	size_t *dim, i, j;

	int sockfd, portno, n, status, ch_stat, *tmpint;

        socklen_t clilen;
        struct sockaddr_in cli_addr;
	char *answer="This is answer";

	int nmax;
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
 * make "header" with solver name - Server will use the name to decide what to do
 */
	SolverName = client_name("CLIENT1");
/*
 * open socket - because we use more then just send - receive scenario
 * we need to open socket manualy and used Send_receive function with hostname = NULL, ie. as server
 * portno is then replaced by socket number
 */
	for(i=0; i<nmax; i++){

		printf("\n\n--------------------------------    i = %ld\n\n", i);
	
		if(i == nmax-1){
		
			dim = (size_t *) malloc( 1* sizeof(size_t));
			dim[0] = 1;
/*
 * if at the end of iterational process, add STOP data set with value of STOP = 1
 */
			if(  (TmpNode = m3l_Mklist("STOP", "I", 1, dim, &SolverName, "/Client_Data", "./",  (char *)NULL)) == 0)
				Error("m3l_Mklist");
			tmpint = (int *)m3l_get_data_pointer(TmpNode);
			tmpint[0] = 1;
			free(dim);
		}
/*
 * open socket, IP address of server is in argv[1], port number is in portno
 */
		if ( (sockfd =  m3l_cli_open_socket(argv[1], portno, (char *)NULL)) < 0)
			Error("Could not open socket");
/*
 * send solver name to server and get back ACKN answer
 */
		if( (RecNode = m3l_Send_receive_tcpipsocket(SolverName,(char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL)) == NULL)
				Perror("Send_receive");
/*
 * Do not care what is in RecNode, it should be ACKN, we just need to terminate data stream
 */
		if(m3l_Umount(&RecNode) != 1)
			Perror("m3l_Umount");
/*
 * send data set to server and get back answer - this data set contains data set which contains payload data
 * solver_name makes the initial node, other data added to that node
 */
		Gnode = client_name("Text from Client1");
	
		dim = (size_t *) malloc( 1* sizeof(size_t));
		dim[0] = 1;
/*
 * add iteraztion number
 */
		if(  (TmpNode = m3l_Mklist("Iteration_Number", "I", 1, dim, &Gnode, "/Client_Data", "./", (char *)NULL)) == 0)
				Error("m3l_Mklist");
		TmpNode->data.i[0] = i;
/*
 * add pressure array, array has 10 pressure with some values
 */	
		dim[0] = 10;
		if(  (TmpNode = m3l_Mklist("numbers", "D", 1, dim, &Gnode, "/Client_Data", "./", (char *)NULL)) == 0)
				Error("m3l_Mklist");
		tmpdf = (double *)m3l_get_data_pointer(TmpNode);
		for(j=0; j<10; j++)
			tmpdf[j] = i*j*1.1;
		free(dim);
/*
 * send the data set to Clinet2 through Server and wait for an answer from Client2
 */
		if( (RecNode = m3l_Send_receive_tcpipsocket(Gnode,(char *)NULL, sockfd, "--encoding" , "IEEE-754",  (char *)NULL)) == NULL)
				Perror("Send_receive");
/*
 *answer from Client2 is in RecNoide,  print answer set on screen and remove received data set
 */
		if(m3l_Cat(RecNode, "--all", "-P", "-L",  "*",   (char *)NULL) != 0)
			Error("CatData");
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
