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
 *     Function Rm.c
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




 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Rm.h"
#include "FunctionsPrt.h"


extern lmint_t optind;
static lmint_t verbose_flag;

/*
 * routine finds the list
 */
lmsize_t m3l_Rm(node_t **List, const lmchar_t *path, const lmchar_t *path_loc, lmchar_t * Options, ...)
{
	lmchar_t *word, **opt;
	opts_t *Popts, opts;
	lmsize_t args_num, len, i, rm_tot_nodes;
	va_list args;
	lmint_t c, init_call;
	lmint_t option_index;
	
	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_k = '\0';; opts.opt_L = '\0'; opts.opt_l = '\0';
	opts.opt_linkscleanemptrefs = '\0'; 
	
	option_index = 0;
	rm_tot_nodes=0;
	init_call = 2;
/*
 * check if data set exists
 */
	if(*List == NULL){
		Warning("Rm: NULL list");
		return -1;
	}
/*
 * get number of options
 */	
	if(Options != NULL){
		va_start(args, Options);
		args_num = 1;
		len = strlen(Options);

		while((word = va_arg(args, lmchar_t *)) != NULL){
			args_num++;
		}
		va_end(args);
		args_num++;
/*
 * get the values of option, for that, allocate opts ** array
 */
		if ( (opt = (lmchar_t**)malloc( (args_num)*sizeof(lmchar_t *) )) == NULL)
			Perror("malloc");
/*
 * get the value of the first argument
 */
		va_start(args, Options);
/*
 * array member [0] will be empty
 */
		if ( (opt[0] = (lmchar_t *)malloc( sizeof(lmchar_t) )) == NULL)
				Perror("malloc");
	
 		len = strlen(Options);	
		if ( (opt[1] = (lmchar_t *)malloc( (len+1) * sizeof(lmchar_t ) )) == NULL)
				Perror("malloc");
		strncpy(opt[1], Options, len);
		opt[1][len] = '\0';
/*
 * get the value of other arguments
 */	
		for(i=2; i<args_num; i++){
			word = va_arg(args, lmchar_t *);
			len = strlen(word);
			if ( (opt[i] = (lmchar_t *)malloc( (len+1)*sizeof(lmchar_t) )) == NULL)
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
				{"clean_empty_refs_to_links",     no_argument,       0, 'c'},   /* for LINK: free rather then nulify linkedlist structure in referenced nodes */
				{"ignore",     no_argument,       0, 'i'},   /* ignore case */
				{"DIR",        no_argument,       0, 'd'},   /* look fir DIR only */
				{"FILE",       no_argument,       0, 'f'},   /* look for FILE only */
				{"LINK",       no_argument,       0, 'l'},   /* look fir LINK only */
				{"recursive",  no_argument,       0, 'r'},   /* recursive */
				{"IGNORE",     no_argument,       0, 'I'},   /* all but search term */
				{"keepheadnode",   no_argument,       0, 'k'}, /* remove all up to head node, keep head node */
//				{"link",  	no_argument,   		0, 'L'},  /* search in linked targets */
				
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "dfiIklr", long_options, &option_index);
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

				case 'c':
/*
 * for LINK: if link is being removed, the algorithm searches in the target of the link
 * and set the linked list to NULL, rather then free the memory
 * it is faster but can allocate memory of used way too much
 * to clean it, specify c
 */
					opts.opt_linkscleanemptrefs = 'c';
				break;
				
				case 'i':
/*
 * ignore case
 */
					opts.opt_i = 'i';
				break; 
				case 'I':
/*
 * ignore name
 */
					opts.opt_I = 'I';
					printf(" This option is not implemented correctly");
					exit(0);
				break;
/*
 * look for DIR only
 */
				case 'd':
					opts.opt_d = 'd';
				break;
/*
 * look for FILE only
 */
				case 'f':
					opts.opt_f = 'f';
				break;
/*
 * look for LINK only
 */
				case 'l':
					opts.opt_l = 'l';
				break;
/*
 * recursive
 */
				case 'r':
					opts.opt_r = 'r';
				break;
/*
 * preserve - if removing entire tree, preserv the head node
 */
				case 'k':
					opts.opt_k = 'k';
				break;

				case '?':
/* 
 * Error, getopt_long already printed an error message
 */
				break;

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
/*
 * check if incompatible options
 */
		if( opts.opt_d == 'd' && opts.opt_f == 'f'){
			Warning("Incompatible options -d -f");
			return -1;
		}	
	}
	else
	{
/*
 * no additional options provided
 * get the value of the first argument, as not options are specified the argument is the name to look for
 */
 		opts.opt_r = 'r';
// 		opts.opt_L = 'L';  NOTE - needs to be specified
	}
/*
 * locate nodes using find function
 */
	Popts = &opts;
	
	rm_tot_nodes = m3l_rm_caller(List, path, path_loc, Popts);

	return rm_tot_nodes;
}
