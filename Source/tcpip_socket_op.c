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
 *     Function tcpip_socket_op.c
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


/*
 * function opens, binds and listen socket for a server
 */

#include "libm3l_header.h"
#include "format_type.h"
#include "tcpip_socket_op.h"
#include "FunctionsPrt.h"


lmint_t m3l_server_openbindlistensocket(lmint_t portno, lmchar_t* Options, ...)
{
/*
 * function opens, binds socket and makes it listening (for server)
 */
	lmint_t sockfd, on, status;
	struct sockaddr_in serv_addr, cli_addr;
	struct hostent *hostPtr = NULL;
/*
 * create socket
 */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		Perror("socket()");
/*
 * specify TCP needed info
 */     
	bzero((lmchar_t *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);
 /* 
  *  dissable bind checking, TIME_WAIT    
  */
	on = 1;
	if ( (status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const lmchar_t *) &on, sizeof(on))) == -1)
		Perror("setsockopt(...,SO_REUSEADDR,...)");
/*
 * linger at closing the cocket to ensure all data is transmitted
 */
	struct linger linger = { 0 };
	linger.l_onoff = 1;
	linger.l_linger = 30;
	if ( (status = setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (const lmchar_t *) &linger, sizeof(linger)) ) == -1)
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



lmint_t m3l_cli_open_socket(const lmchar_t * server_addr, lmint_t portno, lmchar_t* Options, ...)
{
	lmint_t sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
    
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		Perror("socket()");

/*
 * link_bandwidth (100Mbps data link) * RTT (0.05secs)
 */
/*        sock_buf_size = 625*8; 
	retval = setsockopt( sockfd, SOL_SOCKET, SO_SNDBUF,
                   (lmchar_t *)&sock_buf_size, sizeof(sock_buf_size) );
	retval = setsockopt( sockfd, SOL_SOCKET, SO_RCVBUF,
                   (lmchar_t *)&sock_buf_size, sizeof(sock_buf_size) );
*/

/*
 * get server name
 */    
	if( (server = gethostbyname(server_addr)) == NULL)
		Perror("gethostbyname()");
/*
 * connect to socket
 */
	bzero((lmchar_t *) &serv_addr, sizeof(serv_addr));
    
	serv_addr.sin_family = AF_INET;
	bcopy((lmchar_t *)server->h_addr, (lmchar_t *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
    
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		Perror("ERROR connecting");
	
/*
 * for test purposes, specify TCP_NODELAY, ie. dissable Nagle's algorithm
 */
	
	
//     int flag = 1;
//        int result = setsockopt(sockfd,            /* socket affected */
//                                 IPPROTO_TCP,     /* set option at TCP level */
//                                 TCP_NODELAY,     /* name of option */
//                                 (lmchar_t *) &flag,  /* the cast is historical
//                                                         cruft */
//                                 sizeof(int));    /* length of option value */
//         if (result < 0)
//            Perror("setsockop()");	
    
	return sockfd;
}

