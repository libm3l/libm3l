/*
 * function frees memory of the node 
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Find.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"

extern int optind;
static int verbose_flag;
/*
 * routine finds the list
 */
find_t *Find(node_t *List, char * Options, ...)
{
	
	find_t *Founds;
	node_t *Tmp1;
 	char *word, **opt, *search_term, *search_term1, *node_path;
	opts_t *Popts, opts;
	size_t args_num, len, i, j;
	va_list args;
	int c;
	int option_index;
	
	path_t *parsed_path;
	
	option_index = 0;
/*
 * get number of options
 */	
	if(Options == NULL)
		return (find_t *)NULL;

	va_start(args, Options);
	args_num = 1;
	len = strlen(Options);

	while((word = va_arg(args, char *)) != NULL){
		args_num ++;
	}
	va_end(args);
/*
 * get the values of option, for that, allocate opts ** array
 */
	if(args_num > 1){

		if ( (opt = (char**)malloc( (args_num+1)*sizeof(char *) )) == NULL)
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
		if ( (opt[1] = (char *)malloc( (len+1)*sizeof(char) )) == NULL)
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
 * get the name to look for
 */
 		search_term1 = va_arg(args, char *);
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
		opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = '\0'; opts.opt_I = '\0';
		opts.opt_d = '\0'; opts.opt_f = '\0';
		optind = 0;
		while (1)
		{
			static struct option long_options[] =
			{
				{"ignore",     	no_argument,    	0, 'i'},  /* ignore case */
				{"DIR",        	no_argument,     	0, 'd'},  /* only DIR */
				{"FILE",       	no_argument,    	0, 'f'},  /* only file, at the moment it means not DIR */
				{"recursive",  	no_argument,  		0, 'r'},  /* search inside the subdirs too */
				{"IGNORE",  	no_argument,    	0, 'I'},  /* search all but search_term */
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
					search_term = StrToLower(search_term);
				break;

				case 'I':
/*
 * ignore case
 */
					opts.opt_I = 'I';
				break;

				case 'L':
/*
 * search inside the target node instead of LINK
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
				break;
/*
 * recursive search
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
			free(search_term);
			return (find_t *)NULL;
		}
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
 * free array opt **
 */
	if(List == NULL){
		Warning("WriteData: NULL list");
		free(search_term);
		return (find_t *)NULL;
	}

	Popts = &opts;
/*
 * call find function with specified options
 */
	if( strncmp(List->type, "DIR",3) != 0){
		Warning("List in Find is not DIR");
		free(search_term);
		return (find_t *)NULL;
	}
/* 
 * this function returns back found_t **pointer which has "founds" number of items
 * do not forget to free it when you do not need it
 */
	if ( (Founds = Find_caller(List, search_term, Popts)) == NULL){
		free(search_term);
		return (find_t *)NULL;
	}
	else
	{
/*
 * write the values of the find result
 */
		printf(" number of founds is %ld \n", Founds->founds);
		for (i=0; i< Founds->founds; i++){
			printf("Name of found subset is --- pointer is %p\n", Founds->Found_Nodes[i]->List);
			
			if( (node_path = Path(Founds->Found_Nodes[i]->List, NULL)) != NULL){
				printf(" Path is %s \n", node_path);
			
				if( (parsed_path = parse_path(node_path)) == NULL){
					Error("Error in path");
					return (find_t *)NULL;
				}
				
				printf(" \n number of segments is %ld\n", parsed_path->seg_count);
				
				for (j=0; j< parsed_path->seg_count; j++)
					printf(" %ld   --%s--\n ", j, parsed_path->path[j]);
				printf("\n");
				
				printf(" Destroying path\n");
							
				destroy_pars_path(&parsed_path);
				
				printf(" freeing node \n");

				free(node_path);
				
				printf("going to next step \n");

 			}
			
		}
	}	
//		NOTE: if(word != NULL) free(word);
		free(search_term);

	return Founds;
}


