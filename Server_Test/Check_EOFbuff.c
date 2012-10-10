
#include "libm3l.h"
#include "Check_EOFbuff.h"


int Check_EOFbuff(char *buff, char *eofbuff, ssize_t n, size_t eoblen, char *EOFBUFF){
/*
 * function check arrival of EOFbuff 
 * - used when only looking for EOFbuff sequence to 
 * terminate reading from socket
 */
	size_t counter,i;
	
	counter = 0;
/*
 * find how many character has buffer, up to eoblen
 */
	if(eoblen < n)
		counter = eoblen ;
	else
		counter = n;

	for (i=counter; i<eoblen; i++)
		eofbuff[i-counter] = eofbuff[i];
		
	for (i=0; i<counter; i++)
		eofbuff[i+eoblen-counter] = buff[n-counter+i];

	eofbuff[eoblen]='\0';
			
	if(strncmp(eofbuff, EOFBUFF, eoblen) == 0){
		return 1;}
	else{
		return 0;
	}
}