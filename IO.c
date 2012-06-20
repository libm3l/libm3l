/*
 * IO operations
 */
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "IO.h"
#include "ReadDescriptor.h"
#include "WriteData.h"
#include "ln_list.h"

static node_t *Read_list(const char *, opts_t *);
static int Write_list(node_t *, const char *, opts_t *);

/*
 * routine Links Slist to Tlist
 */
node_t *Fread(const char *name, char * Options, ...)
{

	node_t *List;
	char *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len,i;
	va_list args;
	int c;
	int option_index;
	
	opts.opt_c = '\0';
	
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
				{"clean_empy_links",     no_argument,       0, 'c'},
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "c", long_options, &option_index);
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
 * clean empty list
 */
					opts.opt_c = 'c';
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
	
 	if( (List = Read_list(name, Popts)) == NULL){
		return (node_t *)NULL;
	}
	else{
		return List;
	}
}

/*
 * function reads file and allocates structure
 */
node_t *Read_list(const char *name, opts_t *Popts)
{
	FILE *fp;
	node_t *Lnode;

	Lnode = NULL;

	printf("Reading file %s\n", name);

	if ( (fp = fopen(name,"r")) == NULL)
		Perror("fopen");

	if( (Lnode = read_file(fp)) == NULL)
		Perror("read_file");
 /*
  * end of reading the file   - while (   ( fgets(buff, MAXLINE, fp) != NULL) ) {  -- loop
  */
	if( fclose (fp) != 0)
		Perror("fclose");
/*
 * if required, clean empty links
 */
	if(Popts->opt_c == 'c')
		 ln_cleanempytlinks(&Lnode,  (opts_t *)NULL) ;

	return Lnode;
}








int Fwrite(node_t *Lnode,  const char *name, char * Options, ...)
{

	char *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len,i;
	va_list args;
	int c;
	int option_index;
	
	opts.opt_c = '\0';
	
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
				{"clean_empy_links",     no_argument,       0, 'c'},
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "c", long_options, &option_index);
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
 * clean empty list
 */
					opts.opt_c = 'c';
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
	
 	if( Write_list(Lnode, name, Popts) < 0){
		return -1;
	}
	else{
		return 0;
	}
}




int Write_list(node_t *Lnode, const char *name, opts_t * Popts)
{
	FILE *fp;

	printf("Writing file %s\n", name);
/*
 * if required, clean empty links
 */
	if(Popts->opt_c == 'c')
		 ln_cleanempytlinks(&Lnode,  (opts_t *)NULL) ;

	if ( (fp = fopen(name,"w")) == NULL)
		Perror("fopen");

	if( WriteData(Lnode, fp) != 0)
		Perror("WriteData");
 /*
  * end of reading the file   - while (   ( fgets(buff, MAXLINE, fp) != NULL) ) {  -- loop
  */
	if( fclose (fp) != 0)
		Perror("fclose");

	return 0;
}