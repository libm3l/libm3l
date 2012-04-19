/*
 * function frees memory of the node 
 * copyright ï¿½ 2012 Adam Jirasek
 *
 *
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Rm.h"
#include "FunctionsPrt.h"
#include "Find_Source.h"


extern int optind;
static int verbose_flag;

/*
 * routine finds the list
 */
size_t Rm(node_t **List, char * Options, ...)
{
	node_t *Tmp1;
	find_t **Found_Nodes;
 	char *word, **opt, *search_term, *search_term1;
	opts_t *Popts, opts;
	size_t args_num, len, i, rmnodes, rm_tot_nodes, founds;
	va_list args;
	int c;
	int option_index;
	
	char path[256];
	
	founds = 0;
	option_index = 0;
	rm_tot_nodes=0;
/*
 * check if data set exists
 */
	if(List == NULL){
		Warning("Rm: NULL list");
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

	while((word = va_arg(args, char *)) != NULL){
		args_num ++;
	}
	va_end(args);
/*
 * get the values of option, for that, allocate opts ** array
 */
	if(args_num > 1){

		if ( (opt = (char**)malloc( (args_num+1)*sizeof(char **) )) == NULL)
			Perror("malloc");
/*
 * get the value of the first argument
 */
		va_start(args, Options);
/*
 * array member [0] will be empty
 */
		if ( (opt[0] = malloc( sizeof(char *) )) == NULL)
				Perror("malloc");
	
 		len = strlen(Options);	
		if ( (opt[1] = malloc( (len+1)*sizeof(char *) )) == NULL)
				Perror("malloc");
		strncpy(opt[1], Options, len);
		opt[1][len] = '\0';
/*
 * get the value of other arguments
 */	
		for(i=2; i<args_num; i++){
			word = va_arg(args, char *);
			len = strlen(word);
			if ( (opt[i] = malloc( (args_num+1)*sizeof(char *) )) == NULL)
				Perror("malloc");
			strncpy(opt[i], word, len);
			opt[i][len] = '\0';
		}
/*
 * get the name to look for
 */
 		search_term1 = va_arg(args, char *);
		search_term = strdup(search_term1);
/*
 * end args
 */
		va_end(args);
/*
 * get meaning of options
 * first - reset opting = 0 to reinitialize getopt_long
 */
		opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = '\0'; opts.opt_I = '\0';
		optind = 0;
		while (1)
		{
			static struct option long_options[] =
			{
				{"ignore",     no_argument,       0, 'i'},
				{"DIR",        no_argument,       0, 'd'},
				{"FILE",       no_argument,       0, 'f'},
				{"recursive",  no_argument,       0, 'r'},
				{"IGNORE",     no_argument,       0, 'I'},
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "dfiIr", long_options, &option_index);
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
			return -1;
		}
/*
 * free array opt **
 */
		if(args_num > 1){
			for(i=0; i<args_num; i++)
				free(opt[i]);
			free(opt);
		}	
	}
	else
	{
/*
 * no additional options provided
 * get the value of the first argument, as not options are specified the argument is the name to look for
 */
		va_start(args, Options);
		search_term = strdup(Options);
		va_end(args);
	}
/*
 * locate nodes using find function
 */
	Popts = &opts;
/*
 * if name of file not specified, delete nentire node
 */
	if( *search_term == '*'){		
		if ( (rm_tot_nodes = rm_list(1, List)) < 0){
			Warning("Error when removing node");
			return -1;
		}
	}
	else
	{
/*
 * find specified names and remove them
 */

		if ( (Found_Nodes = Find_caller(*List, &founds, search_term, Popts)) == NULL){
			return -1;
		}
		else
		{
/*
 * write the values of the find result
 */
			printf(" number of founds is %ld \n", founds);

			for (i=0; i<founds; i++){
				Tmp1 = Found_Nodes[i]->List;
			
//				printf("RM    -- Removing %s\n", Tmp1->name);
	
				if ( (rmnodes = rm_list(1, &Tmp1)) > 0){
					rm_tot_nodes = rm_tot_nodes + rmnodes;
					Found_Nodes[i]->List = NULL;
				
				}
/*
 * this should never happen, removing master head node done through function "unmount" == "mount -u"
 */		
				if(*List == Found_Nodes[founds-1]->List){
					printf("Removing Matster Head node\n");
					*List=NULL;
				}
/*
 * NOTE:   the fields are correcty destroyed (or at least I think) but the pointer of the deleted nodes in 
 * 		the main tree are not NULL!!!!! Need to correct it. Possibly important when the 
 * 		deleted node is List
 */
			}
/*
 * free structure returned by Find_caller
 */
			DestroyFound(Found_Nodes, founds);

		}
	}
//	if(word != NULL) free(word);
	if(search_term != NULL) free(search_term);

	return rm_tot_nodes;
}
