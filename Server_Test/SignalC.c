#include "libm3l.h"
#include "Local_Header.h"
#include "SignalC.h"

void catch_int(int sig_num)
{
    sigset_t mask_set;     /* used to set a signal masking set. */
    sigset_t old_set;      /* used to store the old mask set.   */
/*
 * re-set the signal handler again to catch_int, for next time 
 */
    signal(SIGINT, catch_int);
/*
 * mask any further signals while you are inside the handler. 
 */
    sigfillset(&mask_set);
    sigprocmask(SIG_SETMASK, &mask_set, &old_set);
 
//      DelSMH_char(, shmid);
//     DelSMH_Double(Pressure, shmidP);
//     DelSMH_Double(X, shmidX);
//     DelSMH_Double(Y, shmidY);
//     DelSMH_Double(Z, shmidZ);
    DelSEM(id); 
    DelSMH_Int(shm_buff, shmid);
    DelSMH_Int(shm_n, shmid1);
    exit(0);
    
    /* restore the old signal mask */
    sigprocmask(SIG_SETMASK, &old_set, NULL);
}