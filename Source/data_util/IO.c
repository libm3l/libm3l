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
 *     Function IO.c
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




 
#include "libm3l_header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "IO.h"
#include "ReadDescriptor.h"
#include "ReadBDescriptor.h"
#include "WriteData.h"
#include "WriteBData.h"
#include "ln_list.h"
#include "Set_Default_Parameters.h"
#include "FunctionsPrt.h"

static node_t *m3l_Read_list(const lmchar_t *, opts_t *);
static lmint_t m3l_Write_list(node_t *, const lmchar_t *, opts_t *);

/*
 * routine Links Slist to Tlist
 */
node_t *m3l_Fread(const lmchar_t *name, lmchar_t * Options, ...)
{

	node_t *List;
	lmchar_t *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len,i;
	va_list args;
	lmint_t c;
	lmint_t option_index;
	
// 	opts.opt_linkscleanemptlinks = '\0';  // clean empty links
// 	opts.opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
// 	opts.opt_linkscleanemptrefs = '\0'; // clean empty link references

	Popts = &opts;
	m3l_set_Fread(&Popts);

	option_index = 0;
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
		opt[0][0]='\0';

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
				{"clean_empy_links",     no_argument,       0, 'e'},
                {"format",     	required_argument,	0, 'f' },
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "e", long_options, &option_index);
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

				case 'e':
/*
 * clean empty list
 */
					opts.opt_linkscleanemptlinks = 'e';
				break;
                
                case 'f':
/*
 * format
 */
					if( strncmp(optarg, "b", 6) == 0){
/*
 * IEEE-754 encoding for numbers
 */
						opts.opt_format = 'b';
					}
					else if(strncmp(optarg, "a", 3) == 0){
/*
 * raw data sending
 */
						opts.opt_format = 'a';
					}
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
	
 	if( (List = m3l_Read_list(name, Popts)) == NULL){
		return (node_t *)NULL;
	}
	else{
		return List;
	}
}

/*
 * function reads file and allocates structure
 */
node_t *m3l_Read_list(const lmchar_t *name, opts_t *Popts)
{
	FILE *fp;
	node_t *Lnode;

	Lnode = NULL;

	printf("Reading file %s\n", name);

	if ( (fp = fopen(name,"r")) == NULL)
		Perror("fopen");

    if(Popts->opt_format == 'a'){
        if( (Lnode = m3l_read_file(fp, Popts)) == NULL)
            Perror("read_file");
    }else{
        if( (Lnode = m3l_read_Bfile(fp, Popts)) == NULL)
            Perror("read_file"); 
    }
 /*
  * end of reading the file   - while (   ( fgets(buff, MAXLINE, fp) != NULL) ) {  -- loop
  */
	if( fclose (fp) != 0)
		Perror("fclose");
/*
 * if required, clean empty links
 */
	if(Popts->opt_linkscleanemptlinks == 'e')
		 m3l_ln_cleanemptylinks(&Lnode,  Popts) ;

	return Lnode;
}



lmint_t m3l_Fwrite(node_t *Lnode,  const lmchar_t *name, lmchar_t * Options, ...)
{

	lmchar_t *word, **opt;
	opts_t *Popts, opts;
	size_t args_num, len,i;
	va_list args;
	lmint_t c;
	lmint_t option_index;
	
	Popts = &opts;
	m3l_set_Fwrite(&Popts);
	
// 	opts.opt_linkscleanemptlinks = '\0';  // clean empty links
// 	opts.opt_nomalloc = '\0'; // if 'm', do not malloc (used in Mklist --no_malloc
// 	option_index = 0;
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
		opt[0][0]='\0';

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
				{"clean_empy_links",     no_argument,       0, 'e'},
                {"format",     	required_argument,	0, 'f' },
				{0, 0, 0, 0}
			};
 /*
  * getopt_long stores the option index here. 
  */
			c = getopt_long (args_num, opt, "ef:", long_options, &option_index);
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

				case 'e':
/*
 * clean empty list
 */
					opts.opt_linkscleanemptlinks = 'e';

                case 'f':
/*
 * format
 */
					if( strncmp(optarg, "b", 6) == 0){
/*
 * IEEE-754 encoding for numbers
 */
						opts.opt_format = 'b';
					}
					else if(strncmp(optarg, "a", 3) == 0){
/*
 * raw data sending
 */
						opts.opt_format = 'a';
					}
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
    
    if( m3l_Write_list(Lnode, name, Popts) < 0){
       return -1;
    }
    else{
       return 0;
    }
}




lmint_t m3l_Write_list(node_t *Lnode, const lmchar_t *name, opts_t * Popts)
{
	FILE *fp;

	printf("Writing file %s\n", name);
/*
 * if required, clean empty links
 */
	if(Popts->opt_linkscleanemptlinks == 'e')
		 m3l_ln_cleanemptylinks(&Lnode,  Popts) ;

	if ( (fp = fopen(name,"w")) == NULL)
		Perror("fopen");

    if(Popts->opt_format == 'a'){
        if( m3l_WriteData(1, Lnode, fp) != 0)
            Perror("WriteData");
    }else{
        if( m3l_WriteBData( Lnode, fp) != 0)
            Perror("WriteData");   
    }
 /*
  * end of reading the file   - while (   ( fgets(buff, MAXLINE, fp) != NULL) ) {  -- loop
  */
	if( fclose (fp) != 0)
		Perror("fclose");

	return 0;
}
