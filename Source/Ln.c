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
 *     Function Ln.c
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
#include "internal_format_type.h"

#include "Ln.h"
#include "FunctionsPrt.h"
#include "ln_list.h"

extern lmint_t optind;
static lmint_t verbose_flag;

/*
 * routine Links Slist to Tlist
 */
lmsize_t m3l_Ln(node_t **SList, const lmchar_t *s_path, const lmchar_t *s_path_loc, node_t **TList, const lmchar_t *t_path, const lmchar_t *t_path_loc, lmchar_t * Options, ...)
{

	lmchar_t *word, **opt;
	opts_t *Popts, opts;
	lmsize_t args_num, len, i, ln_tot_nodes;
	va_list args;
	lmint_t c, init_call;
	lmint_t option_index;
	
	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_k = '\0'; opts.opt_b = '\0'; opts.opt_l = '\0';
	opts.opt_linkscleanemptrefs = '\0'; opts.opt_linkscleanemptlinks = '\0'; opts.opt_nomalloc = '\0'; opts.opt_add = '\0';
	
	// opts.opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc

	option_index = 0;
	ln_tot_nodes=0;
	init_call = 2;
/*
 * check if data set exists
 */
// 	if(SList == NULL){
// 		Warning("Cp: NULL source list");
// 		return -1;
// 	}
	
	if(*TList == NULL){
		Warning("Cp: NULL target list");
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
				{"add",     	no_argument,       0, 'a'},
				{"clean_empty_refs_to_links",     no_argument,       0, 'c'},   /* for LINK: free rather then nulify linkedlist structure in referenced nodes */
				{"clean_empty_links",     no_argument,       0, 'e'},   /* for LINK: free rather then nulify linkedlist structure in referenced nodes */
				{"ignore",     no_argument,       0, 'i'},
				{"DIR",        no_argument,       0, 'd'},
				{"FILE",       no_argument,       0, 'f'},
				{"LINK",       no_argument,       0, 'l'},
				{"IGNORE",     no_argument,       0, 'I'},
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "acdefiklI", long_options, &option_index);
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

				case 'a':
/*
 * add case - if target name already exist and option is specified, do not overwrite it, add a new item with the same name
 */
					opts.opt_add = 'a';
				break;
				case 'c':
/*
 * clean empty links references
 */
					opts.opt_linkscleanemptrefs = 'c';
				break;
				
				case 'e':
/*
 * clean empty links references
 */
					opts.opt_linkscleanemptlinks = 'e';
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
 * look for FILE only
 */
				case 'l':
					opts.opt_l = 'l';
				break;				
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
		if( opts.opt_d == 'd' && opts.opt_f == 'f'){  /* NOTE  check for more incompatible options */
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

	ln_tot_nodes = m3l_ln_caller(SList, s_path, s_path_loc, TList, t_path, t_path_loc, Popts);	
	

	return ln_tot_nodes;
}
