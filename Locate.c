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
 * routine finds the list
 */
find_t *Locate(node_t *List, const char *path, const char *path_loc, char * Options, ...)
{
	
	find_t *Founds, *Founds_Loc;
	node_t *Tmp_node;
	path_t *parsed_path;
	
 	char *word, **opt, *search_term, *node_path;
	opts_t *Popts, opts;
	size_t args_num, len, i;
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
/*
 * get the values of option, for that, allocate opts ** array
 */
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
		for(i=2; i<=args_num; i++){
			word = va_arg(args, char *);
			len = strlen(word);
			if ( (opt[i] = malloc( (args_num+1)*sizeof(char *) )) == NULL)
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
		opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_L = '\0';
		optind = 0;
		while (1)
		{
			static struct option long_options[] =
			{
				{"ignore",     	no_argument,    	0, 'i'},  /* ignore case */
				{"DIR",        	no_argument,     	0, 'd'},  /* only DIR */
				{"FILE",       	no_argument,    	0, 'f'},  /* only file, at the moment it means not DIR */
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
// 		opts.opt_L = 'L';
	}
/*
 * free array opt **
 */
	if(List == NULL){
		Warning("Locate: NULL list");
		return (find_t *)NULL;
	}

	Popts = &opts;
/*
 * parse path
 */
	printf(" parsing path \n");

	if( (parsed_path = parse_path(path)) == NULL){
		Error("Error in path");
		return (find_t *)NULL;
	}
	
	for (i=0; i< parsed_path->seg_count; i++)
		printf(" Segment %d is %s\n", i, parsed_path->path[i]);
	printf(" Paths is %c \n", parsed_path->abspath);
/*
 * call find function with specified options
 * First look if ../ are in path or if path is absolute path
 * set initial node, if path contains ../ go to higher lever
 */
	Tmp_node = List;
	if(parsed_path->abspath == 'A'){
		
		while(Tmp_node->parent != NULL)Tmp_node = Tmp_node->parent;
/*
 * check if first segment is identical to name of initial node
 */
// 		printf("1 -- %p   %s  %s\n ",Tmp_node, parsed_path->path[0], Tmp_node->name);
// 		printf(" strlen is %d   %d\n", strlen(Tmp_node->name), strncmp(Tmp_node->name, parsed_path->path[0], strlen(Tmp_node->name)));

		if(strncmp(Tmp_node->name, parsed_path->path[0], strlen(Tmp_node->name)) != 0){
			Error("Wrong absolute path");
			destroy_pars_path(&parsed_path);
			return (find_t *)NULL;
		}
		
// 		printf(" strlen is %d   %d\n", strlen(Tmp_node->name), strncmp(Tmp_node->name, parsed_path->path[0], strlen(Tmp_node->name)));
	}
	else{

		for(i=0; i<parsed_path->seg_count; i++){
			if(strncmp(parsed_path->path[i], "..", 2) == 0){
				if ( (Tmp_node = Founds->Home_Node->parent) == NULL){
					Error("Wrong path");
					destroy_pars_path(&parsed_path);
					return (find_t *)NULL;
				}
			}
		}
	}

	if( strncmp(Tmp_node->type, "DIR", 3) != 0){
		Warning("List in locate is not DIR");
		free(search_term);
		destroy_pars_path(&parsed_path);
		return (find_t *)NULL;
	}
/* 
 * this function returns back found_t **pointer which has "founds" number of items
 * do not forget to free it when you do not need it
 */
	if ( (search_term = strdup(parsed_path->path[parsed_path->seg_count-1])) == NULL)
			Perror("strdup");
	if(opts.opt_i == 'i')search_term = StrToLower(search_term);
		
	if ( (Founds = Find_caller(Tmp_node, search_term, Popts)) == NULL){
		free(search_term);
		destroy_pars_path(&parsed_path);
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
			
			if( (node_path = Path(Founds->Found_Nodes[i]->List)) != NULL){
				printf(" Path is %s \n", node_path);
				free(node_path);
			}
			
		}
/*
 * call locator to select sets
 */		
		printf(" Going to locator\n");
		if ( (Founds_Loc = locator(Founds, path_loc, Popts)) == NULL){
			printf(" After NULL locator\n");

			DestroyFound(&Founds);
			destroy_pars_path(&parsed_path);
			free(search_term);
			return (find_t *)NULL;
		}
		
		printf(" After locator\n");


	}	
//		NOTE: if(word != NULL) free(word);
	free(search_term);
	DestroyFound(&Founds);
 	destroy_pars_path(&parsed_path);

	return Founds_Loc;
}


