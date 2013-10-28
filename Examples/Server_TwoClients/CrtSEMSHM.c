
// #include "libm3l_header.h"
#include "libm3l.h"
#include "Local_Header.h"
#include "CrtSEMSHM.h"

#define SEMKEYPATH "/dev/null"  /* Path used on ftok for semget key  */
#define SHMKEYPATH "/dev/null"  /* Path used on ftok for shmget key  */

int CrtSMH_Int(size_t number, int SHMKEYID)
{
  int shmid;
   key_t  shmkey;
/* 
 * Generate an IPC key for the shared 
 * memory segment.  Typically, an application specific path and 
 * id would be used to generate the IPC key.
 */
    if ( (shmkey = ftok(SHMKEYPATH,SHMKEYID)) == (key_t)-1 )
      Perror("ftok()\n");
/* 
 * Create a shared memory segment using the IPC key.  The        
 * size of the segment is a constant.  The specified permissions 
 * give everyone read/write access to the shared memory segment. 
 * If a shared memory segment already exists for this key,   
 * return an error.                                              
 */
    if ( (shmid = shmget(shmkey, number*sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == -1)
      Perror("shmget()\n");
 
 return shmid;
}


int CrtSMH_Char(size_t number, int SHMKEYID)
{
   int shmid;
   key_t shmkey;
/* 
 * Generate an IPC key for the shared 
 * memory segment.  An application specific path and 
 * id is used to generate the IPC key.
 */
    if ( (shmkey = ftok(SHMKEYPATH,SHMKEYID)) == (key_t)-1 )
      Perror("ftok()\n");
/* 
 * Create a shared memory segment using the IPC key.  The        
 * size of the segment is a constant.  The specified permissions 
 * give everyone read/write access to the shared memory segment. 
 * If a shared memory segment already exists for this key,   
 * return an error.                                              
 */
    if ( (shmid = shmget(shmkey, number*sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) == -1)
      Perror("shmget()\n");
 
 return shmid;
}


int DelSMH_Int(const void *shm_address, int shmid)
{
/*
 * Function is called from singal handler and can be called multiple times
 * test errno != EINVAL is there for this purpose
 */
int status;
struct shmid_ds shmid_struct;
/*
 *  deattach shared memory segment
 */
    if ( (status = shmdt(shm_address)) == -1)
      Perror("shmdt()\n");
/*
 *  delete shared memory segment
 */
    if ( (status = shmctl(shmid, IPC_RMID, &shmid_struct)) == -1)
      if(errno != EINVAL)
      	Perror("shmctl()\n");
}


int DelSMH_Char(int *shm_address, int shmid)
{
  /*
 * Function is called from singal handler and can be called multiple times
 * test errno != EINVAL is there for this purpose
 */
int status;
struct shmid_ds shmid_struct;
/*
 *  deattach shared memory segment
 */
    if ( (status = shmdt(shm_address)) == -1)
      Perror("shmdt()\n");
/*
 *  delete shared memory segment
 */
    if ( (status = shmctl(shmid, IPC_RMID, &shmid_struct)) == -1)
      if(errno != EINVAL)
      	Perror("shmctl()\n");
}






int CrtSEM(int number, int SEMKEYID)
{
  int semid,i, status;
  key_t semkey;
  
  short *sarray;
/* 
 * Generate an IPC key for the semaphore set 
 * An application specific path and 
 * id is used to generate the IPC key.
 */
     if ( (semkey = ftok(SEMKEYPATH,SEMKEYID)) == (key_t)-1 )
      Perror("main: ftok() for sem failed\n");  
/* 
 * Create a semaphore set using the IPC key.  The number of
 * semaphores in the set is "number".  If a semaphore set already
 * exists for the key, return an error. The specified permissions
 * give everyone read/write access to the semaphore set.         
 */
    if ( (semid = semget( semkey, number, 0666 | IPC_CREAT | IPC_EXCL )) == -1)
      Perror("main: semget() failed\n");
/*
 * set initial value of all semaphores 0
 * allocate field of shorts, fill it by 0
 */
//    if ( (sarray = (short *)malloc(number *sizeof(short))) == 0)
//      Perror("malloc()");
//    
//    for (i=1; i<number; i++)
//      sarray[i] = 0;
//     
//     if ( (status = semctl( semid, 1, SETALL, sarray)) == -1)
//        Perror("semctl()"); 
//     free(sarray);

  return semid;
    
}


int DelSEM(int semid)
{
/*
 * Function is called from singal handler and can be called multiple times
 * test errno != EINVAL is there for this purpose
 */  
  int status;

  if ( (status = semctl( semid, 1, IPC_RMID )) == -1)
    if(errno != EINVAL) 
      Perror("semctl()"); 
}

