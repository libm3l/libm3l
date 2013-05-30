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
 *     Function Cat.c
 *
 *     Date: 2012-06-12
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

#include "Cat.h"
#include "FunctionsPrt.h"
#include "find_list.h"
#include "cat_list.h"



extern lmint_t optind;
static lmint_t verbose_flag;

/*
 * routine finds the list
 */
lmint_t m3l_Cat(node_t *List, lmchar_t * Options, ...)
{
	
 	lmchar_t *word, **opt, *search_term, *search_term1;
	size_t args_num, len, i, founds;
	find_t *Founds;
	node_t *Tmp1;
	opts_t *Popts, opts;
	va_list args;
	lmint_t c, status, status1;
	lmint_t option_index;
		
	opts.opt_s = '0';opts.opt_p = '0'; opts.opt_d = '0' ; opts.opt_f = '0';opts.opt_l = '0';opts.opt_L = '0';

	
	option_index = 0;
/*
 * empty list
 */
	if(List == NULL){
		Warning("CatData: NULL list");
		return -1;
	}
/*
 * get number of options
 */	
	if(Options == NULL)
		return -1;

	va_start(args, Options);
	args_num = 1;
	len = strlen(Options);

	while((word = va_arg(args, lmchar_t *)) != NULL){
		args_num ++;
	}
	va_end(args);
/*
 * get the values of option, for that, allocate opts ** array
 */
	if(args_num > 1){

		if ( (opt = (lmchar_t**)malloc( (args_num+1)*sizeof(lmchar_t *) )) == NULL)
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
		opt[0][0]='\0';

 		len = strlen(Options);	
		if ( (opt[1] = (lmchar_t *)malloc( (len+1)*sizeof(lmchar_t) )) == NULL)
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
 * get the name to look for
 */
 		search_term1 = va_arg(args, lmchar_t *);
		if ( (search_term = strdup(search_term1)) == NULL)
			Perror("strdup");
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
/*
 * options for cat_list
 */
				{"all",        no_argument,       0, 'a'}, /* prints out all information including data */
				{"detailed",   no_argument,       0, 'D'}, /* prints out information about node, its type and dimensions */
				{"quick",      no_argument,       0, 'q'}, /* prints node name */
				{"pointer",    no_argument,       0, 'P'}, /* prints out the values of the node pointer */
//				{"head",       no_argument,       0, 'h'},  /* writes path to the List node */
//				{"HEAD",       no_argument,       0, 'H'},  /* writes paths to the master head node */
				{"listsubdir", no_argument,       0, 'L'},  /* if item is DIR, print content af subDIRS too */
				{"links",        no_argument,     0, 'l'},  /* if item is LIST, print ist content  instead of LIST*/
/*
 * options for find_list
 */
				{"ignore",     no_argument,       0, 'i'}, /* ignore case */
				{"DIR",        no_argument,       0, 'd'}, /* searches only DIR */
				{"FILE",       no_argument,       0, 'f'}, /* searches only FILE */
				{"recursive",  no_argument,       0, 'r'}, /* searches recursively */
				{"IGNORE",     no_argument,       0, 'I'}, /* searches all but given pattern */
//				{"link",  	no_argument,  	  0, 'L'},  /* search in linked targets */
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "adDhHfiIlLPqr", long_options, &option_index);
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
 * all
 */
					opts.opt_s = 'a';
				break;

				case 'D':
/*
 * details
 */
					opts.opt_s = 'd';
				break;
				
				case 'L':
/*
 * List directories
 */
					opts.opt_L = 'L';
				break;
/*
 * List LIST content instead
 */
				case 'l':
					opts.opt_l = 'l';
				break;
/*
 * quick
 */
				case 'q':
					opts.opt_s = 'q';
				break;
/*
 * add pointers
 */
				case 'P':
					opts.opt_p = 'p';
				break;
				
				
/*
 * options for find_list
 */				
				
				case 'i':
/*
 * ignore case
 */
					opts.opt_i = 'i';
					search_term = StrToLower(search_term);
				break;

				case 'I':
/*
 * ignore case
 */
					opts.opt_I = 'I';
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
				case 'r':
					opts.opt_r = 'r';
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
 * check if incompatible options
 */
		if( opts.opt_d == 'd' && opts.opt_f == 'f'){
			Warning("Incompatible options -d -f");
			free(search_term);
			return -1;
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
		va_start(args, Options);
		if ( (search_term = strdup(Options)) == NULL)
			Perror("strdup");
		va_end(args);
	}
/*
 * call find function with specified options
 */	
	Popts = &opts;
/*
 * if no name specified, cat entire List node
 */
	if( *search_term == '*'){
		if ( (status = m3l_cat_list(1, List, Popts)) != 0)
			Warning("Cat");
        	free(search_term);
		return status;
	}
	else
	{
/*
 * cat specified names and remove them
 */

		status1 = 0;

		if ( (Founds = m3l_Find_caller(1, List, search_term, Popts)) == NULL){
			free(search_term);
			return -1;
		}
		else
		{
			printf(" number of founds is %ld \n", founds);

			for (i=0; i< Founds->founds; i++){
				Tmp1 = Founds->Found_Nodes[i]->List;
				
				if ( (status = m3l_cat_list(1, Tmp1, Popts)) != 0)
				Warning("Cat");
				
				if(status < status1) status1 = status;
			}
			
			m3l_DestroyFound(&Founds);

			free(search_term);

			return status1;
		}
	}
/*
 * unspecified error
 */
	return -2;

}