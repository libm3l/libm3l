/*
 * function frees memory of the node 
 * copyright ï¿½ 2012 Adam Jirasek
 *
 *
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Mklist.h"
#include "udf_rm.h"

extern int optind;
static int verbose_flag;

/*
 * routine Links Slist to Tlist
 */
node_t *Mklist(const char *name, const char *type, size_t ndim, size_t *dim, char * Options, ...)
{
	node_t *List;
	char *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len, i;
	va_list args;
	int c;
	int option_index;
	tmpstruct_t TMPSTR;
	
	opts.opt_n = '\0';
	
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
				{"nullify",     no_argument,       0, 'n'},
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "i", long_options, &option_index);
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

				case 'n':
/*
 * nullify field
 */
					opts.opt_i = 'n';
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

	if(name == NULL){
		Warning("Missing name of list");
		return (node_t *) NULL;
	}
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", name) < 0)
		   Perror("snprintf");
	
	if(type == NULL){
		Warning("Missing type of list");
		return (node_t *) NULL;
	}
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", type) < 0)
		   Perror("snprintf");

	if (strncmp(TMPSTR.Type,"LINK",4) == 0){
		Warning("can not create LINK");
		return (node_t *) NULL;
	}
	
	TMPSTR.ndim = ndim;
	TMPSTR.dim=NULL;

	if(strncmp(TMPSTR.Type,"DIR",3) != 0 ){
		if(ndim == 0){
			Warning("missing number of dimensions");
			return (node_t *) NULL;
		}
		else{
			if(dim != NULL){
				TMPSTR.dim=dim;
			}
			else{
				Warning("missing dimensions");
				return (node_t *) NULL;
			}
		}
	}
/*
 * locate nodes using find function
 */
 /*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
	if( (List = AllocateNode(TMPSTR)) == NULL){
		Error("Allocate");
		return (node_t *) NULL;
	}
	return List;
}