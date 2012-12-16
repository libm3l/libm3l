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
 *     Header file Header.h
 *
 *     Date: 2012-07-01
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 *
 */



#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h> 
#include <malloc.h>
#include <limits.h> 
#include <stdarg.h>     
#include <getopt.h>
#include <ctype.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <malloc.h>
#include <resolv.h>
// #include <openssl/ssl.h>
// #include <openssl/err.h>
// #include <arpa/inet.h>
#include <float.h>
#include <stdint.h>
#include <inttypes.h>

/*
 * TCP/IP communication option
 */

//#define MAXLINE  2048               /* Max length of buffer - NOTE MUST BE LARGER OR EQUAL TO strlent(EOFbuff)*/
#define MAXLINE  	8 
#define MAX_WORD_LENGTH  260        /* Max length of word in buffer for TCP/IP, determining mainly for how many digits can be sent over */
#define SEPAR_SIGN ','              /* Separation symbol between words in TCP/IP */
#define EOFbuff "-EOMB-"              /* end of buffer TCP/IP */
#define EOBlen 6                      /* strlen of EOFbuff  */
#define TEXT_SEPAR_SIGN '`'              /* sign determining the beginning and end of text (C, UC, SC) in disk file */
/*
 * node_t data specification - NOTE: these MUST be smaller then MAX_WORD_LENGTH
 */
#define MAX_TYPE_LENGTH   30        /* Max length of type in format */
#define MAX_NAME_LENGTH   255       /* Max length of list name */

#define SIZE_T size_t
#define PTRDF_T ptrdiff_t


//#define FLOAT_MEMCP SPRINTF  /* if defined, use memcpy when IEEE-754 in Write2Socket and ReadSocket, itherwise use strncpy */
#define FLOAT_MEMCP MEMCP
//#define ULLONG_MAX 18446744073709551615

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#endif
