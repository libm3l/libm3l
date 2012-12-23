/*
 *     Copyright (C) 2012  Adam Jirasek
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU Lesser General Public License for more details.
 * 
 *     You should have received a copy of the GNU Lesser General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *     
 *     contact: libm3l@gmail.com
 * 
 */



/*
 *     Function Check_EOFbuff.c
 *
 *     Date: 2012-09-20
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






#include "Header.h"
#include "Check_EOFbuff.h"


lmint_t Check_EOFbuff(lmchar_t *buff, lmchar_t *eofbuff, ssize_t n, size_t eoblen, lmchar_t *EOFBUFF)
{
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
		*(eofbuff+i-counter) = *(eofbuff+i);
// 		eofbuff[i-counter] = eofbuff[i];
	
	for (i=0; i<counter; i++)
		*(eofbuff+i+eoblen-counter) = *(buff+n-counter+i);
// 		eofbuff[i+eoblen-counter] = buff[n-counter+i];

	eofbuff[eoblen]='\0';
			
	if(strncmp(eofbuff, EOFBUFF, eoblen) == 0){
		return 1;}
	else{
		return 0;
	}
}
