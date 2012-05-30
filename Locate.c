/*
 * function frees memory of the node 
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Locate.h"
#include "Locator.h"

#include "FunctionsPrt.h"
#include "Find_Source.h"

extern int optind;
static int verbose_flag;
/*
 * routine finds the list and filters it using path_loc specifications
 */
find_t *Locate(node_t *List, const char *path, const char *path_loc, char * Options, ...)
{
	
	find_t *Founds_Loc;
	char *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len, i;
	va_list args;
	int c;
	int option_index;
	
	option_index = 0;
	
	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_L = '\0';
/*
 * check if data set exists
 */
	if(List == NULL){
		Warning("Locate: NULL list");
		return (find_t *)NULL;
	}
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
			if ( (opt[i] = (char *)malloc( (args_num+1)*sizeof(char) )) == NULL)
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
				{"ignore",     	no_argument,    	0, 'i'},  /* ignore case */
				{"DIR",        	no_argument,     	0, 'd'},  /* only DIR */
				{"FILE",       	no_argument,    	0, 'f'},  /* only file, at the moment it means not DIR */
				{"LINK",       	no_argument,    	0, 'l'},  /* only file, at the moment it means not DIR */
// 				{"recursive",  	no_argument,  		0, 'r'},  /* search inside the subdirs too */
// 				{"IGNORE",  	no_argument,    	0, 'I'},  /* search all but search_term */
				{"link",  	no_argument,   		0, 'L'},  /* search in linked targets */
				{0, 0, 0, 0}
			}; 
 /*
  * getopt_long stores the option index here. 
  */
 			c = getopt_long (args_num, opt, "dfiILr", long_options, &option_index);
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

				case 'i':
/*
 * ignore case
 */
					opts.opt_i = 'i';
				break;

// 				case 'I':
// /*
//  * ignore case
//  */
// 					opts.opt_I = 'I';
// 				break;

				case 'L':
/*
 * write target node instead of LINK
 */
					opts.opt_L = 'L';
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
				break
;/*
 * look for LINK only
 */
				case 'l':
					opts.opt_f = 'l';
				break;
// /*
//  * recursive
//  */
// 				case 'r':
// 					opts.opt_r = 'r';
// 				break;

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
			return (find_t *)NULL;
		}
	}
	else
	{
/*
 * no additional options provided
 * get the value of the first argument, set options to default options
 */
		opts.opt_r = 'r';
// 		opts.opt_L = 'L';  NOTE - needs to be specified
	}

	Popts = &opts;
	
	if( (Founds_Loc = locator_caller(List, path, path_loc, Popts)) == NULL)
		return (find_t *)NULL;

	return Founds_Loc;
}


