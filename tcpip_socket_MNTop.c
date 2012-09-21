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
 *     Function tcpip_socket_MNTop.c
 *
 *     Date: 2012-09-21
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
#include "udf_rm.h"


int m3l_2WayHandShake(const char *hostname, int portnumber, char * Options, ...){

	node_t *List;
	char *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len,i;
	va_list args;
	int c;
	int option_index;
	
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
				{"Send_Receive",     no_argument,       	     		0, 's'},
				{"Receive_Send",     required_argument,                   	0, 'r'},		
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "ec:h", long_options, &option_index);
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

				case 'r':
/*
 * receive send
 */
					opts.opt_r = 'r';
				break;
				case 's':
/*
 * send_receive
 */
					opts.opt_s = 's';
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
		Error("m3l_2WayHandShake: Missing options");
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
	
//  	if( m3l_send_to_tcpipsocket(Lnode, hostname, portnumber, Popts) < 0){
// 		return -1;
// 	}
// 	else{
// 		return 1;
// 	}
}




node_t *MkTCPIPHeader(opts_t *Popts)
{
	node_t *Tmpnode, *AddNode;
	tmpstruct_t TMPSTR;
	
	opts_t *Poptadd, opts;
/*
 * options for add and remove list
 */
	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_k = '\0'; opts.opt_b = '\0';opts.opt_l = '\0';
/*
 * options for allocate list
 */	
	opts.opt_n = '\0'; opts.opt_b = '\0'; opts.opt_nomalloc = '\0';
	Poptadd = &opts;
/*
 * make header node
 */
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List), "%s", lm3lTCPIPHeader) < 0)
		   Perror("snprintf");
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", "DIR") < 0)
		   Perror("snprintf");
	
	TMPSTR.ndim = 0;
	TMPSTR.dim=NULL;
 /*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
	if( (Tmpnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
		Error("Allocate");
		return (node_t *) NULL;
	}
/*
 * add info about coding
 */
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", "TCPIP_encoding") < 0)
		   Perror("snprintf");
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", "C") < 0)
		   Perror("snprintf");
	
	TMPSTR.ndim = 1;
	if( (TMPSTR.dim=(size_t *)malloc(TMPSTR.ndim * sizeof(size_t))) == NULL)
		Perror("malloc");
	TMPSTR.dim[0]=2;
 /*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
	if( (AddNode = m3l_AllocateNode(TMPSTR, Poptadd)) == NULL){
		Error("Allocate");
		free(TMPSTR.dim);
		if( m3l_rm_list(1, &Tmpnode, Popts) < 0)
			Warning("problem in rm_list");
		return (node_t *) NULL;
	}
	
	free(TMPSTR.dim);
	
	AddNode->data.c[0] = Popts->opt_tcpencoding;
	AddNode->data.c[1] = '\0';
	
	if( m3l_add_list(&AddNode, &Tmpnode,  Poptadd) < 0){
		Warning("problem in ladd_list");
		if( m3l_rm_list(1, &Tmpnode, Poptadd) < 0)
			Warning("problem in rm_list");
		if( m3l_rm_list(1, &AddNode, Poptadd) < 0)
			Warning("problem in rm_list");
		return (node_t *)NULL;
	}
	
	return Tmpnode;
}