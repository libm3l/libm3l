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
 *     contact: clibm3l@gmail.com
 * 
 */



/*
 *     Function tcpip_socket_op.c
 *
 *     Author: Adam Jirasek
 *     Date: 2012-06-24
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
 *     Date		Version		Patch number		Author			Descritpion
 *
 */




/*
 * function opens, binds and listen socket for a server
 */

#include "Header.h"
#include "format_type.h"
#include "tcpip_socket_op.h"


int openbindlistensocket(int portno)
{
/*
 * function opens, binds socket and makes it listening
 */
	int sockfd, on, status;
	struct sockaddr_in serv_addr, cli_addr;
	struct hostent *hostPtr = NULL;
	char hostname[80] = ""; 
/*
 * create socket
 */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		Perror("socket()");
/*
 * specify TCP needed info
 */     
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);
 /* 
  *  dissable bind checking, TIME_WAIT    
  */
	on = 1;
	if ( (status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on))) == -1)
		Perror("setsockopt(...,SO_REUSEADDR,...)");
/*
 * linger at closing the cocket to ensure all data is transmitted
 */
	struct linger linger = { 0 };
	linger.l_onoff = 1;
	linger.l_linger = 30;
	if ( (status = setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (const char *) &linger, sizeof(linger)) ) == -1)
		Perror("setsockopt(...,SO_LINGER,...)");
/*
 *  bind the socket
 */     
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		Perror("bind()");
/*
 * convert socket to listening socket
 */     
	if ( listen(sockfd, 10) != 0 )
	{
		Perror("Can't configure listening port");
		abort();
	}
    
	return sockfd;
    
}
