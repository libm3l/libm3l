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
 *     Function tcpip_socket_IOop.c
 *
 *     Date: 2012-07-01
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




#include "Header.h"
#include "format_type.h"
#include "FunctionsPrt.h"

#include "tcpip_socket_IOop.h"
#include "tcpip_socket_op.h"
#include "Write2Socket.h"
#include "ReadSocket.h"
#include "ln_list.h"
#include "rm_list.h"
#include "udf_rm.h"
/*
 * routine Links Slist to Tlist
 */

int m3l_Send_to_tcpipsocket(node_t *Lnode, const char *hostname, int portnumber, char * Options, ...){

	node_t *List;
	char *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len,i;
	va_list args;
	int c;
	int option_index;
	
	opts.opt_linkscleanemptlinks = '\0';  // clean empty links
	opts.opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	opts.opt_linkscleanemptrefs = '\0'; // clean empty link references
	opts.opt_tcpencoding = 't'; // serialization and encoding when sending over TCP/IP
	opts.opt_MEMCP = 'S';  // type of buffering
	
	option_index = 0;
/*
 * get number of options
 */	
	if(Options != NULL){
		va_start(args, Options);
		args_num = 1;
		len = strlen(Options);

		while((word = va_arg(args, char *)) != NULL){
			args_num++;
		}
		va_end(args);
		args_num++;
/*
 * get the values of option, for that, allocate opts ** array
 */
		if ( (opt = (char**)malloc( (args_num)*sizeof(char *) )) == NULL)
			Perror("malloc");
/*
 * get the value of the first argument
 */
		va_start(args, Options);
/*
 * array member [0] will be empty
 */
		if ( (opt[0] = (char *)malloc( sizeof(char) )) == NULL)
				Perror("malloc");
	
 		len = strlen(Options);	
		if ( (opt[1] = (char *)malloc( (len+1) * sizeof(char ) )) == NULL)
				Perror("malloc");
		strncpy(opt[1], Options, len);
		opt[1][len] = '\0';
/*
 * get the value of other arguments
 */	
		for(i=2; i<args_num; i++){
			word = va_arg(args, char *);
			len = strlen(word);
			if ( (opt[i] = (char *)malloc( (len+1)*sizeof(char) )) == NULL)
				Perror("malloc");
			strncpy(opt[i], word, len);
			opt[i][len] = '\0';
		}
/*
 * end args
 */
		va_end(args);
/*
 * get meaning of options
 * first - reset opting = 0 to reinitialize getopt_long
 */
		optind = 0;
		while (1)
		{
			static struct option long_options[] =
			{
				{"clean_empy_links",     no_argument,       	     0, 'e'},
				{"encoding",     required_argument,                  0, 'c'},
				{"buffering",    required_argument,           	     0, 'b'},
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "b:ec:h", long_options, &option_index);
/*
 * Detect the end of the options 
 */
			if (c == -1)
				break;

			switch (c)
			{
				case 0:
 
					if (long_options[option_index].flag != 0)
						break;
			
					printf ("option %s", long_options[option_index].name);
					if (optarg)
						printf (" with arg %s", optarg);
					printf ("\n");
					break;
				
				case 'b':
/*
 * choose how to copy data to IO buffer
 */
					if( strncmp(optarg, "MEMCPY", 6) == 0){
/*
 * MEMCPY
 */
						opts.opt_MEMCP = 'M';
					}
					else{
/*
 * STRNCPY
 */
						opts.opt_MEMCP = 'S';
					}
				break;
				
				case 'c':
/*
 * choose encoding and serialization
 */
					if( strncmp(optarg, "IEEE-754", 8) == 0){
/*
 * IEEE-754 encoding for numbers
 */
						opts.opt_tcpencoding = 'I';
					}
					else if(strncmp(optarg, "raw", 3) == 0){
/*
 * raw data sending
 */
						opts.opt_tcpencoding = 'r';
					}
				break;
				case 'e':
/*
 * clean empty list
 */
					opts.opt_linkscleanemptlinks = 'e';
				break;
/* 
 * Error, getopt_long already printed an error message
 */
				default:
				abort ();
			}
		}
/*
 * free array opt **
 */
		for(i=0; i<args_num; i++)
			free(opt[i]);
		free(opt);	
	}
	else
	{
/*
 * no additional options provided
 * get the value of the first argument, as not options are specified the argument is the name to look for
 */
//  		opts.opt_r = 'r';
// 		opts.opt_L = 'L';  NOTE - needs to be specified
	}
/*
 * locate nodes using find function
 */
	Popts = &opts;
	
 	if( m3l_send_to_tcpipsocket(Lnode, hostname, portnumber, Popts) < 0){
		return -1;
	}
	else{
		return 1;
	}
}



/*
 * routine Links Slist to Tlist
 */
node_t *m3l_Send_receive_tcpipsocket(node_t *Lnode, const char *hostname, int portnumber, char * Options, ...)
{

	node_t *List;
	char *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len,i;
	va_list args;
	int c;
	int option_index;
	
	opts.opt_linkscleanemptlinks = '\0';  // clean empty links
	opts.opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	opts.opt_linkscleanemptrefs = '\0'; // clean empty link references
	opts.opt_tcpencoding = 't'; // serialization and encoding when sending over TCP/IP
	opts.opt_shutdown = '\0'; // shutdown when finished with sending
	opts.opt_MEMCP = 'S';  // type of buffering
	
	option_index = 0;
/*
 * get number of options
 */	
	if(Options != NULL){
		va_start(args, Options);
		args_num = 1;
		len = strlen(Options);

		while((word = va_arg(args, char *)) != NULL){
			args_num++;
		}
		va_end(args);
		args_num++;
/*
 * get the values of option, for that, allocate opts ** array
 */
		if ( (opt = (char**)malloc( (args_num)*sizeof(char *) )) == NULL)
			Perror("malloc");
/*
 * get the value of the first argument
 */
		va_start(args, Options);
/*
 * array member [0] will be empty
 */
		if ( (opt[0] = (char *)malloc( sizeof(char) )) == NULL)
				Perror("malloc");
	
 		len = strlen(Options);	
		if ( (opt[1] = (char *)malloc( (len+1) * sizeof(char ) )) == NULL)
				Perror("malloc");
		strncpy(opt[1], Options, len);
		opt[1][len] = '\0';
/*
 * get the value of other arguments
 */	
		for(i=2; i<args_num; i++){
			word = va_arg(args, char *);
			len = strlen(word);
			if ( (opt[i] = (char *)malloc( (len+1)*sizeof(char) )) == NULL)
				Perror("malloc");
			strncpy(opt[i], word, len);
			opt[i][len] = '\0';
		}
/*
 * end args
 */
		va_end(args);
/*
 * get meaning of options
 * first - reset opting = 0 to reinitialize getopt_long
 */
		optind = 0;
		while (1)
		{
			static struct option long_options[] =
			{
				{"clean_empy_links",     no_argument,              0, 'e'},
				{"encoding",     required_argument,                0, 'c'},
				{"shutdown",           no_argument,                0, 's'},
				{"buffering",    required_argument,           0, 'b'},
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "b:ec:h", long_options, &option_index);
/*
 * Detect the end of the options 
 */
			if (c == -1)
				break;

			switch (c)
			{
				case 0:
 
					if (long_options[option_index].flag != 0)
						break;
			
					printf ("option %s", long_options[option_index].name);
					if (optarg)
						printf (" with arg %s", optarg);
					printf ("\n");
					break;

				case 'b':
/*
 * choose how to copy data to IO buffer
 */
					if( strncmp(optarg, "MEMCPY", 6) == 0){
/*
 * MEMCPY
 */
						opts.opt_MEMCP = 'M';
					}
					else{
/*
 * STRNCPY
 */
						opts.opt_MEMCP = 'S';
					}
				break;
				case 'c':
/*
 * choose encoding and serialization
 */
					if( strncmp(optarg, "IEEE-754", 8) == 0){
/*
 * IEEE-754 encoding for numbers
 */
						opts.opt_tcpencoding = 'I';
					}
					else if(strncmp(optarg, "raw", 3) == 0){
/*
 * raw data sending
 */
						opts.opt_tcpencoding = 'r';
					}
				break;	
				case 'e':
/*
 * clean empty list
 */
					opts.opt_linkscleanemptlinks = 'e';
				break;
				case 's':
/*
 * shutdown when done with sending
 */
					opts.opt_shutdown = 's';
				break;
/* 
 * Error, getopt_long already printed an error message
 */
				default:
				abort ();
			}
		}
/*
 * free array opt **
 */
		for(i=0; i<args_num; i++)
			free(opt[i]);
		free(opt);	
	}
	else
	{
/*
 * no additional options provided
 * get the value of the first argument, as not options are specified the argument is the name to look for
 */
//  		opts.opt_r = 'r';
// 		opts.opt_L = 'L';  NOTE - needs to be specified
	}
/*
 * locate nodes using find function
 */
	Popts = &opts;
	
 	if( (List = m3l_send_receive_tcpipsocket(Lnode, hostname, portnumber, Popts)) == NULL){
		return (node_t *)NULL;
	}
	else{
		return List;
	}
}



/*
 * routine Links Slist to Tlist
 */
node_t *m3l_Receive_send_tcpipsocket(node_t *Lnode, const char *hostname, int portnumber, char * Options, ...)
{

	node_t *List;
	char *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len,i;
	va_list args;
	int c;
	int option_index;
	
	opts.opt_linkscleanemptlinks = '\0';  // clean empty links
	opts.opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	opts.opt_linkscleanemptrefs = '\0'; // clean empty link references
	opts.opt_tcpencoding = 't'; // serialization and encoding when sending over TCP/IP
	opts.opt_shutdown = '\0'; // shutdown when done with receiving
	opts.opt_MEMCP = 'S';  // type of buffering
	
	option_index = 0;
/*
 * get number of options
 */	
	if(Options != NULL){
		va_start(args, Options);
		args_num = 1;
		len = strlen(Options);

		while((word = va_arg(args, char *)) != NULL){
			args_num++;
		}
		va_end(args);
		args_num++;
/*
 * get the values of option, for that, allocate opts ** array
 */
		if ( (opt = (char**)malloc( (args_num)*sizeof(char *) )) == NULL)
			Perror("malloc");
/*
 * get the value of the first argument
 */
		va_start(args, Options);
/*
 * array member [0] will be empty
 */
		if ( (opt[0] = (char *)malloc( sizeof(char) )) == NULL)
				Perror("malloc");
	
 		len = strlen(Options);	
		if ( (opt[1] = (char *)malloc( (len+1) * sizeof(char ) )) == NULL)
				Perror("malloc");
		strncpy(opt[1], Options, len);
		opt[1][len] = '\0';
/*
 * get the value of other arguments
 */	
		for(i=2; i<args_num; i++){
			word = va_arg(args, char *);
			len = strlen(word);
			if ( (opt[i] = (char *)malloc( (len+1)*sizeof(char) )) == NULL)
				Perror("malloc");
			strncpy(opt[i], word, len);
			opt[i][len] = '\0';
		}
/*
 * end args
 */
		va_end(args);
/*
 * get meaning of options
 * first - reset opting = 0 to reinitialize getopt_long
 */
		optind = 0;
		while (1)
		{
			static struct option long_options[] =
			{
				{"clean_empy_links",     no_argument,       0, 'e'},
				{"encoding",     required_argument,                  0, 'c'},
				{"shutdown",           no_argument,                    0, 's'},				
				{"buffering",    required_argument,           0, 'b'},
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "b:ec:hs", long_options, &option_index);
/*
 * Detect the end of the options 
 */
			if (c == -1)
				break;

			switch (c)
			{
				case 0:
 
					if (long_options[option_index].flag != 0)
						break;
			
					printf ("option %s", long_options[option_index].name);
					if (optarg)
						printf (" with arg %s", optarg);
					printf ("\n");
					break;

				case 'b':
/*
 * choose how to copy data to IO buffer
 */
					if( strncmp(optarg, "MEMCPY", 6) == 0){
/*
 * MEMCPY
 */
						opts.opt_MEMCP = 'M';
					}
					else{
/*
 * STRNCPY
 */
						opts.opt_MEMCP = 'S';
					}
				break;
				
				case 'c':
/*
 * choose encoding and serialization
 */
					if( strncmp(optarg, "IEEE-754", 8) == 0){
/*
 * IEEE-754 encoding for numbers
 */
						opts.opt_tcpencoding = 'I';
					}
					else if(strncmp(optarg, "raw", 3) == 0){
/*
 * raw data sending
 */
						opts.opt_tcpencoding = 'r';
					}
				break;
				case 'e':
/*
 * clean empty list
 */
					opts.opt_linkscleanemptrefs = 'e';
				break;
				case 's':
/*
 * shutdown when done with sending
 */
					opts.opt_shutdown = 's';
				break;
/* 
 * Error, getopt_long already printed an error message
 */
				default:
				abort ();
			}
		}
/*
 * free array opt **
 */
		for(i=0; i<args_num; i++)
			free(opt[i]);
		free(opt);	
	}
	else
	{
/*
 * no additional options provided
 * get the value of the first argument, as not options are specified the argument is the name to look for
 */
//  		opts.opt_r = 'r';
// 		opts.opt_L = 'L';  NOTE - needs to be specified
	}
/*
 * locate nodes using find function
 */
	Popts = &opts;
	
 	if( (List =m3l_receive_send_tcpipsocket(Lnode, hostname, portnumber, Popts)) == NULL){
		return (node_t *)NULL;
	}
	else{
		return List;
	}
}


node_t *m3l_Receive_tcpipsocket(const char *hostname, int portnumber, char * Options, ...)
{

	node_t *List;
	char *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len,i;
	va_list args;
	int c;
	int option_index;
	
	opts.opt_linkscleanemptlinks = '\0';  // clean empty links
	opts.opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
	opts.opt_tcpencoding = 't'; // serialization and encoding when sending over TCP/IP
	opts.opt_MEMCP = 'S';  // type of buffering
	
	option_index = 0;
/*
 * get number of options
 */	
	if(Options != NULL){
		va_start(args, Options);
		args_num = 1;
		len = strlen(Options);

		while((word = va_arg(args, char *)) != NULL){
			args_num++;
		}
		va_end(args);
		args_num++;
/*
 * get the values of option, for that, allocate opts ** array
 */
		if ( (opt = (char**)malloc( (args_num)*sizeof(char *) )) == NULL)
			Perror("malloc");
/*
 * get the value of the first argument
 */
		va_start(args, Options);
/*
 * array member [0] will be empty
 */
		if ( (opt[0] = (char *)malloc( sizeof(char) )) == NULL)
				Perror("malloc");
	
 		len = strlen(Options);	
		if ( (opt[1] = (char *)malloc( (len+1) * sizeof(char ) )) == NULL)
				Perror("malloc");
		strncpy(opt[1], Options, len);
		opt[1][len] = '\0';
/*
 * get the value of other arguments
 */	
		for(i=2; i<args_num; i++){
			word = va_arg(args, char *);
			len = strlen(word);
			if ( (opt[i] = (char *)malloc( (len+1)*sizeof(char) )) == NULL)
				Perror("malloc");
			strncpy(opt[i], word, len);
			opt[i][len] = '\0';
		}
/*
 * end args
 */
		va_end(args);
/*
 * get meaning of options
 * first - reset opting = 0 to reinitialize getopt_long
 */
		optind = 0;
		while (1)
		{
			static struct option long_options[] =
			{
				{"clean_empy_links",     no_argument,         0, 'e'},
				{"encoding",     required_argument,           0, 'c'},
				{"buffering",    required_argument,           0, 'b'},
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "b:ec:", long_options, &option_index);
/*
 * Detect the end of the options 
 */
			if (c == -1)
				break;

			switch (c)
			{
				case 0:
 
					if (long_options[option_index].flag != 0)
						break;
			
					printf ("option %s", long_options[option_index].name);
					if (optarg)
						printf (" with arg %s", optarg);
					printf ("\n");
					break;

				case 'b':
/*
 * choose how to copy data to IO buffer
 */
					if( strncmp(optarg, "MEMCPY", 6) == 0){
/*
 * MEMCPY
 */
						opts.opt_MEMCP = 'M';
					}
					else{
/*
 * STRNCPY
 */
						opts.opt_MEMCP = 'S';
					}
				break;
				
				case 'c':
/*
 * choose encoding and serialization
 */
					if( strncmp(optarg, "IEEE-754", 8) == 0){
/*
 * IEEE-754 encoding for numbers
 */
						opts.opt_tcpencoding = 'I';
					}
					else if(strncmp(optarg, "raw", 3) == 0){
/*
 * raw data sending
 */
						opts.opt_tcpencoding = 'r';
					}
				break;
				case 'e':
/*
 * clean empty list
 */
					opts.opt_linkscleanemptrefs = 'e';
				break;
/* 
 * Error, getopt_long already printed an error message
 */
				default:
				abort ();
			}
		}
/*
 * free array opt **
 */
		for(i=0; i<args_num; i++)
			free(opt[i]);
		free(opt);	
	}
	else
	{
/*
 * no additional options provided
 * get the value of the first argument, as not options are specified the argument is the name to look for
 */
//  		opts.opt_r = 'r';
// 		opts.opt_L = 'L';  NOTE - needs to be specified
	}
/*
 * locate nodes using find function
 */
	Popts = &opts;
	
 	if( (List = m3l_receive_tcpipsocket(hostname, portnumber, Popts)) == NULL){
		return (node_t *)NULL;
	}
	else{
		return List;
	}
}


/*
 * function opens socket, writes data to it, reads data from it and close the socket
 */
node_t *m3l_receive_tcpipsocket(const char *hostname, int portnumber, opts_t *Popts)
{
	node_t *Gnode;
	int socketnr;

	if(portnumber < 1){
		Warning("m3l_receive_to_tcpipsocket:  wrong port/socket number");
		return (node_t *)NULL;
	}
	if(hostname == NULL){
/*
 * server side, portnumber is socket number
 */
		if( (Gnode = m3l_read_socket(portnumber, Popts)) == NULL)
			Error("Error during reading data from socket");
	}
	else{
/*
 * client side
 */
		if ( (socketnr =  m3l_cli_open_socket(hostname, portnumber, (char *)NULL)) < 0)
			Error("Could not open socket");
		if( (Gnode = m3l_read_socket(socketnr, Popts)) == NULL)
			Error("Error during reading data from socket");
		if( close(socketnr) == -1)
			Perror("close");
	}
/*
 * if required, clean empty links
 */
	if(Popts->opt_linkscleanemptlinks == 'e')
		 m3l_ln_cleanemptylinks(&Gnode,  Popts) ;
	
	return Gnode;
}

/*
 * function opens socket, writes data to it and close the socket
 */
int m3l_send_to_tcpipsocket(node_t *Lnode, const char *hostname, int portnumber, opts_t *Popts)
{
	int socketnr;

	if(Lnode == NULL){
		Warning("m3l_send_to_tcpipsocket:  NULL Lnode");
		return -1;
	}
	if(portnumber < 1){
		Warning("m3l_send_to_tcpipsocket:  wrong port/socket number");
		return -1;
	}
/*
 * if required, clean empty links
 */
	if(Popts->opt_linkscleanemptlinks == 'e')
		 m3l_ln_cleanemptylinks(&Lnode,  Popts);

	if(hostname == NULL){
/*
 * server side, portnumber is socket number
 */
		if ( m3l_write_to_socket(1, Lnode,  portnumber, Popts) < 0)
			Error("Error during writing data to socket");
	}
	else{
/*
 * client side
 */
		if ( (socketnr =  m3l_cli_open_socket(hostname, portnumber, (char *)NULL)) < 0)
			Error("Could not open socket");
		if ( m3l_write_to_socket(1, Lnode,  socketnr, Popts) < 0)
			Error("Error during writing data to socket");
		if( close(socketnr) == -1)
			Perror("close");
	}	
	return 1;
}
/*
 * function opens socket, writes data to it, reads data from it and close the socket
 */
node_t *m3l_send_receive_tcpipsocket(node_t *Lnode, const char *hostname, int portnumber, opts_t *Popts)
{
	node_t *Gnode;
	int socketnr;

	if(Lnode == NULL){
		Warning("m3l_send_receive_tcpipsocket:  NULL Lnode");
		return (node_t *)NULL;
	}
	if(portnumber < 1){
		Warning("m3l_send_to_tcpipsocket:  wrong port/socket number");
		return (node_t *)NULL;
	}
/*
 * if required, clean empty links
 */
	if(Popts->opt_linkscleanemptlinks == 'e')
		 m3l_ln_cleanemptylinks(&Lnode,  Popts) ;

	if(hostname == NULL){
		if ( m3l_write_to_socket(1, Lnode,  portnumber, Popts) < 0)
			Error("Error during writing data to socket");
		if( (Gnode = m3l_read_socket(portnumber, Popts)) == NULL)
			Error("Error during reading data from socket");
	}
	else{
/*
 * client side
 */
		if ( (socketnr =  m3l_cli_open_socket(hostname, portnumber, (char *)NULL)) < 0)
			Error("Could not open socket");
		if ( m3l_write_to_socket(1, Lnode,  socketnr, Popts) < 0)
			Error("Error during writing data to socket");
/*
 * shutdown socket for writing
 */
		if(Popts->opt_shutdown = 's'){
			if( shutdown(socketnr,SHUT_WR) != 0)
				Perror("shutdown");
		}

		if( (Gnode = m3l_read_socket(socketnr, Popts)) == NULL)
			Error("Error during reading data from socket");

		if( close(socketnr) == -1)
			Perror("close");
	}	
/*
 * if required, clean empty links
 */
	if(Popts->opt_linkscleanemptlinks == 'e')
		 m3l_ln_cleanemptylinks(&Gnode,  Popts) ;
	
	return Gnode;
}

/*
 * function opens socket, reads data from it , writes data to it and close the socket
 */
node_t *m3l_receive_send_tcpipsocket(node_t *Lnode, const char *hostname, int portnumber, opts_t *Popts)
{
	node_t *Gnode;
	int socketnr;

	if(Lnode == NULL){
		Warning("m3l_receive_send_tcpipsocket:  NULL Lnode");
		return (node_t *)NULL;
	}
	if(portnumber < 1){
		Warning("m3l_send_to_tcpipsocket:  wrong port/socket number");
		return (node_t *)NULL;
	}
/*
 * if required, clean empty links
 */
	if(Popts->opt_linkscleanemptlinks == 'e')			
		m3l_ln_cleanemptylinks(&Lnode,  Popts) ;

	if(hostname == NULL){
/*
 * server side, portnumber is socket number
 */
		if( (Gnode = m3l_read_socket(portnumber, Popts)) == NULL)
			Error("Error during reading data from socket");
		
		if ( m3l_write_to_socket(1, Lnode,  portnumber, Popts) < 0)
			Error("Error during writing data to socket");
	}
	else{
/*
 * client side
 */
		if ( (socketnr =  m3l_cli_open_socket(hostname, portnumber, (char *)NULL)) < 0)
			Error("Could not open socket");
		if( (Gnode = m3l_read_socket(socketnr, Popts)) == NULL)
			Error("Error during reading data from socket");
/*
 * shutdown socket for reading
 */
		if(Popts->opt_shutdown = 's'){
			if( shutdown(socketnr,SHUT_RD) != 0)
				Perror("shutdown");
		}
		if ( m3l_write_to_socket(1, Lnode,  socketnr, Popts) < 0)
			Error("Error during writing data to socket");
		if( close(socketnr) == -1)
			Perror("close");
	}
/*
 * if required, clean empty links
 */
	if(Popts->opt_linkscleanemptlinks == 'e')
		 m3l_ln_cleanemptylinks(&Gnode,  Popts) ;

	return Gnode;
}

