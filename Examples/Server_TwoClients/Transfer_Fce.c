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
 *     Function Transfer_Fce.c
 *
 *     Date: 2013-02-22
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
#include "Local_Header.h"
#include "Transfer_Fce.h"

/*
 *  process ReadSocketCopy2SHM reads from socket to shared memory and 
 *  ReadSHMCopy2Socket sends then data to another socket
 */

/*
 * upon start, the semaphores are 0 0 
 * 
 * ReadSocketCopy2SHM starts with semaphores euqal to 0 1
 * and leaves with 1 0
 * 
 * ReadSHMCopy2Socket starts with semaphores euqal to 1 0 
 * and leaves with 0 1
 * 
 * when entire operation is done, ReadSHMCopy2Socket sets semaphoers to 1 1
 */

int ReadSocketCopy2SHM(int descrpt)
{
/*
 * function reads buffer from socket and place it into the 
 * shared memory segment
 * then it sets appropriate semaphores
 */
	struct sembuf operations[2];   /* An "array" of one operation to perform on the semaphore. */
	int retval, eofbuffcond;
	char prevbuff[EOBlen+1];
	ssize_t n;

	shm_n[0] = 1;

	bzero(prevbuff,EOBlen+1);
	
	bzero(shm_buff, strlen(shm_buff));
	eofbuffcond = 0;
	
	while (1){    /*  while (*shm_n )  */

		bzero(shm_buff, MAXLINE);

		if(eofbuffcond == 1 ){
/*
 * previous reading was the last one
 * leave cycle, before that change semaphore 0 so that ReadSHMCopy2Socket can continue
 */
			shm_n[0] = 0;
			operations[0].sem_num 	  = 0;/* Which semaphore in the semaphore array : */
			operations[0].sem_op      = 1;/* Which operation? Add 1 to semaphore value : */
			operations[0].sem_flg     = 0;/* Set the flag so we will wait : */
			if ( (retval = semop(id, operations, 1)) != 0)
				Perror("semop()");
			return 1;
		}

		if (  (n = read(descrpt,shm_buff,MAXLINE-1)) == -1)
			Perror("read");
		shm_buff[n] = '\0';
/*
 * check if the end of entire buffer is reached
 */		
		eofbuffcond = Check_EOFbuff(shm_buff,prevbuff, n,EOBlen, EOFbuff);
/*
 * say ReadSHMCopy2Socket it can continue
 */
		operations[0].sem_num 	     = 0;/* Which semaphore in the semaphore array : */
		operations[0].sem_op         = 1;/* Which operation? Add 1 to semaphore value : */
		operations[0].sem_flg        = 0;/* Set the flag so we will wait : */
		if ( (retval = semop(id, operations, 1)) != 0)
			Perror("semop()");
		
/*
 * and wait until ReadSHMCopy2Socket sends data over the socket
 */		
		operations[0].sem_num =  1;
		operations[0].sem_op  = -1;
		operations[0].sem_flg =  0;

		if ( (retval = semop(id, operations, 1)) != 0)
			Perror("semop()");
	}
	
	return 0;

}

int ReadSHMCopy2Socket(int descrpt)
{
/*
 * function reads buffer from socket and place it into the 
 * shared memory segment
 * then it sets appropriate semaphores
 */
	struct sembuf operations[2];   /* An "array" of one operation to perform on the semaphore. */
	int retval;
	size_t size, n;
	short  sarray[1];
	
	while(1){
/*
 * wait until given a signal go ahead
 */
		operations[0].sem_num   =  0;
		operations[0].sem_op    = -1;
		operations[0].sem_flg   =  0;
		if ( (retval = semop(id, operations, 1)) != 0)
			Perror("semop()");
/*
 * if ReadSocketCopy2SHM reached end, *shm_n == 0, leave cycle otherwise
 * send entire segment to socket
 */
		if( shm_n[0] > 0 ){
			size = strlen(shm_buff);
			if(size > 0){
				if ( (n = write(descrpt,shm_buff,size)) < size)
					Perror("write()");
			}
		}
		else
		{
/*
 * upon leaving set all semaphores to 0 and counter for reading *shm_n = 1
 * so that next time the cycle can start again
 */
			operations[0].sem_num   = 0;
			operations[0].sem_op    = 1;
			operations[0].sem_flg   = 0;

			operations[1].sem_num   = 1;
			operations[1].sem_op    = 1;
			operations[1].sem_flg   = 0;
			if ( (retval = semop(id, operations, 2)) != 0)
				Perror("semop()");		

			
			shm_n[0] = 1;
		
			return 1;
		}
/*
 * tell ReadSocketCopy2SHM to go on with another socket reading
 */
		operations[0].sem_num  = 1;
		operations[0].sem_op   = 1;
		operations[0].sem_flg  = 0;
		if ( (retval = semop(id, operations, 1)) != 0)
			Perror("semop()");
	}
	return 0;
}