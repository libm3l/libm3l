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
 *     Function FunctionsPrt.c
 *
 *     Date: 2012-06-24
 * 
 * 
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
#include "Server_FunctionsPrt.h"

lmint_t sig_pid = 9999;


/*
 *
  */
void sig_chld(lmint_t signo)
{
	pid_t   pid;
	lmint_t	stat;
	while( (sig_pid = waitpid(-1, &stat, WNOHANG)) >0 );
	return;
}

/*
 *
 */
lmint_t Fork(void)
{
	lmint_t childpid;
        if ( (childpid = fork()) < 0) 
		Perror("fork()");
	return childpid;
}

