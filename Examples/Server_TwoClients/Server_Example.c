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
 *     Function Server_Example.c
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
#include "CrtSEMSHM.h"
#include "Local_Header.h"
#include "SignalC.h"

#define SEMKEY 1
#define SEMKEYPATH "/dev/null"  /* Path used on ftok for semget key  */


int main(int argc, char *argv[])
{
	node_t *Gnode=NULL, *RecNode=NULL, *TmpNode = NULL, *ACKN=NULL;
	pid_t  childpid;
	size_t *dim;

	int sockfd, newsockfd, portno, n, status, ch_stat, retval;
	int sum, i, tot_dim, stop;

        socklen_t clilen;
        struct sockaddr_in cli_addr;
	char *answer="This is answer";
	
	char *solver;

	find_t *SFounds, *SFounds1;
	opts_t *Popts, opts;

	pid_t PID;
	
	struct sembuf operations[2];   /* An "array" of one operation to perform on the semaphore. */
	short  sarray[4];

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
	signal(SIGINT, catch_int);
/*
 * SIGCHLD signal handler
 */    
	signal(SIGCHLD,sig_chld);
	signal(SIGUSR1, catch_usr);

 /*
 * create, bind and listen socket
 */
	if ( (sockfd = m3l_server_openbindlistensocket(portno, (char *)NULL) ) < 0 )
		Perror("Open_Bind_Listen");
/*
 * create 4 semaphores and set the initial value
 */
	id = CrtSEM(4, SEMKEY);

	sarray[0] = 0;
	sarray[1] = 0;
	sarray[2] = 1;
	sarray[3] = 1;
/*
 * 1. semaphore - 0 first process reading from socket and saving to shared memory
 *                1 finished reading, second process can write to socket
 * 2. semaphore   0 second process writing from  shared memory to socket
 *                1 first process can start reading from socket
 * 3. semaphore   1. client can grab one fork
 *                0. fork is already taken
 * 4. semaphore   1. client can grab one fork
 *                0. fork is already taken
 */

 /*
  *  The '1' on this command is a no-op, because the SETALL commandis used     
  */
	if ( (retval = semctl( id, 0, SETALL, sarray)) == -1)
		Perror("semctl()4");
/*
 * allocate shared memory and nullify it
 */
	shmid = CrtSMH_Char(MAXLINE+1, 1);
	if ((shm_buff = shmat(shmid, NULL, 0)) == (char *)-1) {
		Perror("shmat");
	}

	memset(shm_buff,0,MAXLINE+1);

	shmid1 = CrtSMH_Int(2, 2);
	if ((shm_n = shmat(shmid1, NULL, 0)) == (int *) -1) {
		Perror("shmat");
	}
	memset(shm_n,0,1);
	shm_n[0] = 1;
	shm_n[1] = 1;
/*
 * create node_t with ACKN message
 */
	ACKN = ackn();
	TmpACKN = ACKN; /* set the value of global TmpACKN so that it can be freed in signal habdler */
/*
 * loop
 */
	PID = getpid();
		
	while(1){
/*
 * accept connection
 */	
	clilen = sizeof(cli_addr);

		if ( (newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0){
			if(errno == EINTR) /* If Interrupted system call, restart - back to while ()  UNP V1 p124  */
				continue;
			else
				Perror("accept()");
		}
/*
 * fork and decide what type of solver is it
 */
		if ( (childpid = Fork())  == 0) { 
/*
 * close listening socket
 */
			if (( status = close(sockfd)) != 0)
				Perror("close()");

			if( (RecNode = m3l_Receive_tcpipsocket((const char *)NULL, newsockfd, "--encoding" , "IEEE-754", (char *)NULL)) == NULL)
				Error("Error during reading data from socket");
		
			if ( (SFounds = m3l_locator_caller(RecNode, "/Client_Data/Name", "/*/*", Popts)) != NULL){
				TmpNode = SFounds->Found_Nodes[0]->List;
			}
			else{
				Error("Client data wrong");
			}

// 			if(m3l_Cat(RecNode, "--all", "-P", "-L",  "*",   (char *)NULL) != 0)
// 				Error("CatData");

			if(strncmp(TmpNode->data.c, "CLIENT1", 4) == 0){

				stop = 0;

				if ( (SFounds1 = m3l_locator_caller(RecNode, "/Client_Data/STOP", "/*/*", Popts)) != NULL){
					stop = SFounds1->Found_Nodes[0]->List->data.i[0];
					m3l_DestroyFound(&SFounds1);
				}		
/*
 * umount node with solver name info
 */
				if(m3l_Umount(&RecNode) != 1)
					Perror("m3l_Umount");
/*
 * you need to send ACKN to terminate reading from socket
 */
				if( m3l_Send_to_tcpipsocket(ACKN, (const char *)NULL, newsockfd, "--encoding" , "IEEE-754", "--SEOB", (char *)NULL) < 1)
					Error("Error during reading data from socket");
/*
 * wait for internal semaphores (1 and 2) being 0 and decrement Edge semaphore (2) - the Edge smeaphore mkes sure only one Edge 
 * client is requesting fork and using connection
 */
				operations[0].sem_num    =  0;   /* Operate on the sem_num sem      */
				operations[0].sem_op     =  0; /* increase by 0   */
				operations[0].sem_flg    =  0; /* Allow a wait to occur             */
				operations[1].sem_num    =  1;   /* Operate on the sem_num sem      */
				operations[1].sem_op     =  0; /* increase by 0   */
				operations[1].sem_flg    =  0; /* Allow a wait to occur             */
				operations[2].sem_num    =  2;
				operations[2].sem_op     =  -1;
				operations[2].sem_flg    =  0;
				if( (retval = semop(id, operations, 3)) != 0)
					Perror("semop()2");
/*
 * wait for CSM client to start
 */
				operations[0].sem_num    = 3;
				operations[0].sem_op     = 0;
				operations[0].sem_flg    = 0;
				if( (retval = semop(id, operations, 1)) != 0)
					Perror("semop()3");
/*
 * read data from socket sent by Edge process
 */
		
				ReadSocketCopy2SHM(newsockfd);
/*
 * wait until second client is done with reading and sending bufer an nullifying semaphore and setting *shm_n counter to 1
 */
				operations[0].sem_num     =  0;   /* Operate on the sem_num sem      */
				operations[0].sem_op      =  -1; /* increase by 0   */
				operations[0].sem_flg     =  0; /* Allow a wait to occur             */
				operations[1].sem_num     =  1;   /* Operate on the sem_num sem      */
				operations[1].sem_op      =  -1; /* increase by 0   */
				operations[1].sem_flg     =  0; /* Allow a wait to occur             */
				if( (retval = semop(id, operations, 2)) != 0)
					Perror("semop()4"); 
/*
 * wait for data sent by CSM and send it back to Edge
 */
				ReadSHMCopy2Socket(newsockfd);
/*
 * wait until reading CSM data is done and all data is sent to Edge
 * once done, increment 2. semaphore indicating fork is free to be used by 
 * another request
 */
				operations[0].sem_num     =  0;   /* Operate on the sem_num sem      */
				operations[0].sem_op      =  0; /* increase by 0   */
				operations[0].sem_flg     =  0; /* Allow a wait to occur             */
				operations[1].sem_num     =  1;   /* Operate on the sem_num sem      */
				operations[1].sem_op      =  0; /* increase by 0   */
				operations[1].sem_flg     =  0; /* Allow a wait to occur             */
				operations[2].sem_num     =  2;
				operations[2].sem_op      =  1;
				operations[2].sem_flg     =  0;
				if( (retval = semop(id, operations, 3)) != 0)
						Perror("semop()5");

				shm_n[1] = shm_n[1]*(1-stop);
				
				if(m3l_Umount(&ACKN) != 1)
					Perror("m3l_Umount");
				
				if(shm_n[1] == 0)
					kill(PID,SIGUSR1);

			}
			else    /*  CLIENT 2 */
			{
				stop = 0;
				if ( (SFounds1 = m3l_locator_caller(RecNode, "/Client_Data/STOP", "/*/*", Popts)) != NULL){
					stop = SFounds1->Found_Nodes[0]->List->data.i[0];
					m3l_DestroyFound(&SFounds1);
				}
/*
 * umount node with solver name info
 */
				if(m3l_Umount(&RecNode) != 1)
					Perror("m3l_Umount");
/*
 * CSM is now waiting for reading you do not need to send ACKN
 *
 * wait for internal semaphores (1 and 2) being 0 and decrement CSM semaphore (2)- the CSM smeaphore mkes sure only one Edge 
 * client is requesting fork and using connection
 */
				operations[0].sem_num    =  0;   /* Operate on the sem_num sem      */
				operations[0].sem_op     =  0; /* increase by 0   */
				operations[0].sem_flg    =  0; /* Allow a wait to occur             */
				operations[1].sem_num    =  1;   /* Operate on the sem_num sem      */
				operations[1].sem_op     =  0; /* increase by 0   */
				operations[1].sem_flg    =  0; /* Allow a wait to occur             */
				operations[2].sem_num    =  3;
				operations[2].sem_op     =  -1;
				operations[2].sem_flg    =  0;
				if( (retval = semop(id, operations, 3)) != 0)
					Perror("semop()6");
/*
 * wait for Edge client to start
 */
				operations[0].sem_num   = 2;
				operations[0].sem_op    =  0;
				operations[0].sem_flg   = 0;
				if( (retval = semop(id, operations, 1)) != 0)
					Perror("semop()7");
/*
 * start getting data from Edge and sending over to CSM
 */
				ReadSHMCopy2Socket(newsockfd);
/*
 * wait until entire process of shipping data from Edge to CSM is finished
 */

				operations[0].sem_num    =  0;   /* Operate on the sem_num sem      */
				operations[0].sem_op     =  0;  /* increase by 0   */
				operations[0].sem_flg    =  0; /* Allow a wait to occur             */
				operations[1].sem_num    =  1;   /* Operate on the sem_num sem      */
				operations[1].sem_op     =  0; /* increase by 0   */
				operations[1].sem_flg    =  0; /* Allow a wait to occur             */
				if( (retval = semop(id, operations, 2)) != 0)
					Perror("semop()8");
/*
 * set read(socket...) counter to 1 so that reading can start
 */
				shm_n[0] = 1;
/*
 * read data sent by CSM process
 */
				ReadSocketCopy2SHM(newsockfd);
/*
 * wait until data is sent to Edge and then 
 * continue, increment 3.semaphore indicating fork is free to be used by 
 * another request
 */		

				shm_n[1] = shm_n[1]*(1-stop);

				operations[0].sem_num    =   0;   /* Operate on the sem_num sem      */
				operations[0].sem_op     =  -1; /* increase by 0   */
				operations[0].sem_flg    =   0; /* Allow a wait to occur             */
				operations[1].sem_num    =   1;   /* Operate on the sem_num sem      */
				operations[1].sem_op     =  -1; /* increase by 0   */
				operations[1].sem_flg    =   0; /* Allow a wait to occur             */
				operations[2].sem_num    =   3;
				operations[2].sem_op     =   1;
				operations[2].sem_flg    =   0;
				if( (retval = semop(id, operations, 3)) != 0)
					Perror("semop()9"); 
				
				if(m3l_Umount(&ACKN) != 1)
					Perror("m3l_Umount");

			}
/*
 * free borrow memory
 */
			m3l_DestroyFound(&SFounds);
/*
 *  exit
 */	
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
		}

	}  /* End of while(1) */
	
	printf(" Ending\n");
	
	if(m3l_Umount(&ACKN) != 1)
		Perror("m3l_Umount");

	DelSMH_Int(shm_buff, shmid);
	DelSMH_Int(shm_n, shmid1);	
	DelSEM(id);

	close(sockfd);	
	wait(&status);

     return 0; 
}
