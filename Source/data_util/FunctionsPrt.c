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
 *     Function FunctionsPrt.c
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




 
#include "libm3l_header.h"
#include "format_type.h"
#include "FunctionsPrt.h"

// lmint_t sig_pid = 9999;

/*
 *
 */
void Perror(const lmchar_t *msg)
{
	perror(msg);
	exit(1);
}

void Error(const lmchar_t *msg)
{
	printf("ERROR:  %s\n", msg);
	exit(1);
}

/*
 *
  */
void Warning(const lmchar_t *msg)
{
	printf("Warning:  %s\n", msg);
// 	exit(1);
}

/*
 * Converts string to long
 */
lmsize_t Strol(lmchar_t *text)
{
	lmchar_t *err;
	lmsize_t i;

  	i = strtol(text,&err,0);
	if (err == text){
/* 
 * string is not a number
 */
		printf("'%s'\n", text);
		Error("Strol:  Error when reading and converting integer number");
	}
	else if (*err != 0){
		printf("'%s'\n", text);
		Perror("Strol:  Error when reading and converting integer number");
/* 
 *str began with a number but has junk left over at the end 
 */
	}
	else if ((i == LONG_MAX || i == LONG_MIN) && errno == ERANGE){
/* 
 * the value of string does not fit in unsigned long long
 */
		printf("'%s'\n", text);
		Perror("Strol:  Error when reading and converting integer number");
	}
	return i;
}

/*
 * converts hex string to unsigned 32bit
 */

lmulint_t Strtoul(lmchar_t *str, lmint_t base){
	
	lmchar_t  *end;
	Uint64_t result;
	
	errno = 0;
	result = strtoull(str, &end, base);
	if (result == 0 && end == str) {
/* 
 * string is not a number
 */
		Error("Strtoul: Error when reading and converting float number");
		exit(0);
	} 
	else if (result == ULONG_MAX && errno) {
/* 
 * the value of string does not fit in unsigned long long
 */
		Error("Strtoul: Error when reading and converting float number");
		exit(0);
	} 
	else if (*end != 0) {
/* 
 *str began with a number but has junk left over at the end 
 */
		Error("Strtoul: Error when reading and converting float number");
		exit(0);
	}
	return result;
}
/*
 * converts hex string to unsigned 64bit
 */
Uint64_t Strtoull(lmchar_t *str, lmint_t base){
	
	lmchar_t  *end;
	Uint64_t result;
	
	errno = 0;
	result = strtoull(str, &end, base);
	if (result == 0 && end == str) {
/* 
 * string is not a number
 */
		Error("Strtoull: Error when reading and converting float number");
		exit(0);
	} 
	else if (result == ULLONG_MAX && errno) {
/* 
 * the value of string does not fit in unsigned long long
 */
		Error("Strtoull: Error when reading and converting float number");
		exit(0);
	} 
	else if (*end != 0) {
/* 
 *str began with a number but has junk left over at the end 
 */
		Error("Strtoull: Error when reading and converting float number");
		exit(0);
	}
	
	return result;
}



/*
 * strig to lower case
 */
lmchar_t *StrToLower(lmchar_t *s) 
{ 
	lmchar_t *cs;

	if( s == NULL)
		return NULL;

	cs = s;

	while(*s != '\0') 
	{ 
		if (*s >= 'A' && *s <= 'Z')
			*s = *s+ 'a' - 'A';

		s++;
	} 
	return cs;	
}

/*
 * strig to lower case
 */
lmchar_t *StrToUpper(lmchar_t *s) 
{ 
	lmchar_t *cs;

	if( s == NULL)
		return NULL;

	cs = s;

	while(*s != '\0') 
	{ 
		if (*s >= 'a' && *s <= 'z')
			*s = *s+ 'A' - 'a';

		s++;
	} 
	return cs;	
}




void *RD_MemcpyD(Uint64_t *di, lmchar_t *type, lmsize_t length)
{	
	memcpy(di, &type[0], length);
	return NULL;
}

void *RD_MemcpyF(Uint32_t *di, lmchar_t *type, lmsize_t length)
{	
	memcpy(di, &type[0], length);
	return NULL;
}
void *RD_StrtoullD(Uint64_t *di, lmchar_t *type, lmsize_t length)
{	
	*di = Strtoull(type, length);
	return NULL;
}

void *WR_MemcpyD(lmchar_t *buff, Uint64_t *di, lmsize_t length)
{	
	memcpy( &buff[0], di, length);
	buff[length] = SEPAR_SIGN;
	buff[length+1] = '\0';
	return NULL;
}

void *WR_snprintfD(lmchar_t *buff, Uint64_t *di, lmsize_t length){
	lmsize_t n;
	
// 	if( (n=snprintf(buff, length, "%016" PRIx64 "%c", *di, SEPAR_SIGN)) < 0)
	if( (n=snprintf(buff, length, "%" PRIx64 "%c", *di, SEPAR_SIGN)) < 0)
		Perror("snprintf");
	buff[n] = '\0';
	return NULL;
}

void *WR_MemcpyF(lmchar_t *buff, Uint32_t *di, lmsize_t length)
{	
	memcpy( &buff[0], di, length);
	buff[length] = SEPAR_SIGN;
	buff[length+1] = '\0';

	return NULL;
}

// void *WR_MemcpyI(lmchar_t *buff, void *i, lmsize_t length)
// {	
// 	memcpy( &buff[0], i, length);
// 	buff[length] = SEPAR_SIGN;
// 	buff[length+1] = '\0';
// }

void *WR_snprintfF(lmchar_t *buff, Uint32_t *di, lmsize_t length){
	lmsize_t n;
	
// 	if( (n=snprintf(buff, length, "%08" PRIx32 "%c", *di, SEPAR_SIGN)) < 0)
	if( (n=snprintf(buff, length, "%" PRIx32 "%c", *di, SEPAR_SIGN)) < 0)
		Perror("snprintf");
	buff[n] = '\0';
	return NULL;
}


/*
 * function allocates sting and saves the 
 * path of the node in it. The string is a return parameter
 * NOTE - needs to be feed after use
 */

lmchar_t *m3l_Path(node_t *List, node_t *Orig_List)
{
/*
  * function finds path of the list up to Orig_List
 * If Orig_List == NULL, then the path goes to master head node
 */
	lmchar_t   *path;
	lmchar_t   **segs;
	node_t 	   *Tmp;
	lmsize_t count,i,*len, tot_len, tot_len1, j;
/*
 * find the number of all nodes up to Master Head Node
 */
	Tmp = List;
	count = 1;
	while(Tmp->parent != NULL){
		if(Tmp == Orig_List)break;
		count++;
		Tmp = Tmp->parent;
	}
/*
 * allocate arrays
 */
	if ( (segs = (lmchar_t **)malloc( (count)*sizeof(lmchar_t *) )) == NULL)
		Perror("malloc");
	if ( (len = (lmsize_t *)malloc( (count)*sizeof(lmsize_t) )) == NULL)
		Perror("malloc");
/*
 * fill segs with segments of the path
 */
	Tmp = List;
	i=0;
	tot_len = 0;
	while(Tmp->parent != NULL){

		if(Tmp == Orig_List) break;

		len[i] = strlen(Tmp->name);
		if ( (segs[i] = (lmchar_t *)malloc( (len[i]+1)*sizeof(lmchar_t) )) == NULL)
			Perror("malloc");
		if( snprintf(segs[i], len[i]+1,"%s",Tmp->name) < 0)
			Perror("snprintf");
		
		Tmp = Tmp->parent;
		tot_len = tot_len + len[i] + 1;
// 		i++;
		if(i++ > count-1){
			Error("Count");
			for(j=0; j<=i; j++)
				free(segs[i]);
			free(segs);
			free(len);
			return NULL;
		}
	}
/*
 * save length of the segment
 */
	len[i] = strlen(Tmp->name);
	if ( (segs[i] = (lmchar_t *)malloc( (len[i]+1)*sizeof(lmchar_t) )) == NULL)
			Perror("malloc");
		if( snprintf(segs[i], len[i]+1,"%s",Tmp->name) < 0)
			Perror("snprintf");
/*
 * get total length of the path
 */
	tot_len = tot_len + len[i] + 1;
/*
 * allocate string for path + 1 additional for '\0'
 */		
	if ( (path = (lmchar_t *)malloc( (tot_len + 1)*sizeof(lmchar_t) )) == NULL)
		Perror("malloc");
	
	tot_len1 = 0;
/*
 * fill the string + last '\0'
 */
	for (i = count; i-- > 0; ){
		path[tot_len1++] = '/';
		for(j=0; j<len[i]; j++){
			path[tot_len1++] = segs[i][j];}
	}
	if( tot_len1 > tot_len)
		Error("malloc problem in Path");

	path[tot_len1] = '\0';
/*
 * free temporary arrays
 */
	for(i=0; i<count; i++)
		free(segs[i]);
	free(segs);
	free(len);
/*
 * return path
 */
	return path;
}


/*
 * function segments path (./../../../home/etc/etc/
 */
path_t *m3l_parse_path(const lmchar_t *path)
{
/*
 * NOTE - segmentation fault if path starts with / // // // etc nonsence
 */
	path_t *Path;
	const lmchar_t *pc, *pc1;
	lmsize_t counter, j, st;
	lmchar_t abspath;
/*
 * check that the path makes sense, ie. no spaces tabs and newlines are in
 * disregard empty spaces and tabs at the beginning 
 */
	pc = path;
	while( (*pc == ' ' || *pc == '\t') && *pc != '\0'  )pc++;
/*
 * check that if the path starts with ~ followed by /
 */
	if( *pc != '\0' && *pc == '~' && *++pc != '/'){
			Error(" Wrong path");
			return (path_t *) NULL ;
		}
/*
 * look for empty spaces in path, if they occur, return NULL
 */		
	while(*pc != '\0'){
		if(*pc == ' ' || *pc == '\t'){
			Error(" Wrong path");
			return (path_t *) NULL;
		}
		pc++;
	}
		
	abspath = 'R';
/*
 * parse the path
 */
	pc = path;
	while( (*pc == '\t' || *pc == ' ') &&  *pc != '\0') pc++;
/*
 * path starts with /
 */
	if(*pc == '/' ){
		counter = 0;
		abspath = 'A';
	}
/*
 * path starts with ./
 */
// 	else if(*pc == '.' && *(pc+1) == '/' && *pc != '\0' && *(pc+1) != '\0'){
// 		counter = 0;
// 		pc++;
// 	}
	else if(*pc == '.'){
		counter = 0;
		pc++;
		abspath = 'R';
	}
/*
 * path starts with ~/   - Absolute path
 */	
	else if(*pc == '~' ){
		counter = 0;
		pc++;
		abspath = 'A';
	}	
	else{
		counter = 1;
	}
/*
 * remember this position
 */
	pc1 = pc;
/*
 * count number fo segment of path
 */		
	while(*pc != '\0'){
/*
 * if symbol is / and not the end of the string 
 */
		if(*(pc++)  == '/' && *pc != '\0'){
/*
 * remove all multiple / symbols
 */
			while( *pc == '/' && *pc != '\0') pc++;
/*
 * increase counter of the words in string
 */
		 	counter++;

			if(*pc == '\0'){
/*
 * if end of string, leave loop
 */
				counter--;
				break;
			}
		}
	}
/*
 * allocate array for words in path
 */
	if(counter > 0){
 		if ( (Path = (path_t *)malloc( sizeof(path_t) )) == NULL)
 			Perror("malloc");
		
		if ( (Path->path = (lmchar_t **)malloc(counter * sizeof(lmchar_t *))) == NULL)
			Perror("malloc");

		for(j=0; j< counter; j++){
			if ( (Path->path[j] = (lmchar_t *)malloc( (MAX_NAME_LENGTH + 1) * sizeof(lmchar_t) )) == NULL)  /* NOTE this is a problem  before it was sizeof(lmchar_t *) and it worked */
				Perror("malloc");
		}

		
		Path->seg_count = 0;	
	}
	else
	{
		Error("Wrong path specification");
		return (path_t *) NULL;
	}
/*
 * store individual words in array
 */
	pc = path;
/*
 * get rid of tabs, spaces, and dots, must be the same as at the begining of this function
 */
// 	while( *pc == '\t' || *pc == ' ' || *pc == '.'  &&  *pc != '\0') pc++;	
	pc = pc1;
	
	if(*pc == '/' && *pc != '\0'){
		pc++;
	}
// 	else if(*pc == '.' && *(pc+1) == '/' && *pc != '\0' && *(pc+1) != '\0'){
// 		pc += 2;
// 	}

	st = 0;
	j = 0;

	while(*pc != '\0'){
/*
 * save the segment of the path
 */
		if( st < MAX_NAME_LENGTH && j < counter){
			Path->path[j][st++] = *pc;
		}
		else{
			Error(" Path too long");
/*
 * free Path
 */
			m3l_destroy_pars_path(&Path);
			return (path_t *)NULL;
		}
/*
 * if the last symbol of the path segment is '/' replace it by '\0'
 * it occurs whent the specified path ends with / symbol
 */
		if(*pc  == '/' ){
			Path->path[j][st-1] = '\0';
/*
 * remove all multiple / symbols
 */
			while( *pc == '/' && *pc != '\0') pc++;
// 			st = 0;
/* 
 * if \ is not trailing edge \, increase counter of segments in path and set st (count of characters in next segment to 0)
 */
			if( *pc != '\0') {
				j++;  st = 0;
				if(j > counter){
					Error(" Path too long");
					m3l_destroy_pars_path(&Path);
					return (path_t *) NULL;
				}
			}
			else
				break;
// 			if(*pc == '\0')
// 				break;
		}	
/*
 * if next symbol is '/' remove it
 */
		else if(*(pc++)  == '/' && *pc != '\0'){
/*
 * remove all multiple / symbols
 */
			while( *pc == '/' && *pc != '\0') pc++;
			Path->path[j][st-1] = '\0';

			st = 0;
			j++;
			if(j > counter){
				Error(" Path too long");
				m3l_destroy_pars_path(&Path);
				return (path_t *) NULL;
			}
			
			if(*pc == '\0')
				break;
		}
	}
/*
 * end the last segment string
 */
	if( st < MAX_NAME_LENGTH && j < counter){
		Path->path[j][st++] = '\0';
	}
	else{
		Error(" Path too long - can not add terminating character");
		m3l_destroy_pars_path(&Path);
		return (path_t *) NULL;
	}
	Path->abspath 	= abspath;	/* Realative (R) or absolute (A) path */
	Path->seg_count     = counter;	/* Number of segments in path */

	return (path_t *)Path;
}
/*
 * function frees pointer allocated in parse_path function
 */
void m3l_destroy_pars_path(path_t **Path)
{
/*
 * NOTE - unsuccesfull return must be finished
 */
	lmsize_t i;
		
	for (i=0; i< (*Path)->seg_count; i++)
		free( (*Path)->path[i]);
	free((*Path)->path);

	free(*Path) ;
	(*Path) = NULL;
	return;
	
}

get_arg_t m3l_get_arguments(const lmchar_t *text)
{
/*
 * function parse arguments in text
 *
 * Arguments are: 	Letter = value
 * 			(s) (S) Letter_name = value
 * 
 * if s or S is specified, the argument refers to sub-dir of the data set
 * and is followed by the _name_ of the data set
 */
	const lmchar_t *pc;
	lmchar_t arg;
	lmsize_t i, n;
	get_arg_t argsstr;
/*
 * disregard empty spaces and tabs at the beginning 
 */
	pc = text;
	while( (*pc == ' ' || *pc == '\t') && *pc != '\0'  )pc++;
/*
 * get the first letter, check that it is not '\0'
 */
	argsstr.arg = '*';
	argsstr.first = '\0';
	argsstr.s_name[0] = '\0';
	argsstr.args[0] = '\0';

	if(*pc == '\0'){
		Error("No argument");;
		argsstr.retval = -1;
		return argsstr;
	}
	else{
		argsstr.retval = 0;
	}

	arg = *pc++;	
	if( arg == 's' || arg == 'S'){
/*
 * sub-data set will be specified
 */
		argsstr.first = arg;
		if(*pc == '\0' || *pc == ' ' ){; /* make sure no empty spaces are there */
			Error("Wrong argument");
			argsstr.arg = '\0';
			argsstr.retval = -1;
			return argsstr;
		}
		argsstr.arg = *pc++;
		if(*pc == '\0' || *pc != '_' ){; /* must be _ symbol */
			Error("Wrong argument");
			argsstr.arg = '\0';
			argsstr.retval = -1;
			return argsstr;
		}
		pc++;
		
		i = 0;
		while(*pc != '\0' && *pc != '='){
			while(*pc == ' ' && *pc != '\0')pc++;
			argsstr.s_name[i++] = *pc++;
			if(i > MAX_NAME_LENGTH){
				Error(" too long argument field");
				argsstr.retval = -1;
				return argsstr;
			}
		}
		
		argsstr.s_name[i] = '\0';

/*
 * if argument to use is name (N), copy the subsate name to argsstr.args
 */
		if(argsstr.arg == 'N'){

			if( (n=snprintf(argsstr.args, MAX_NAME_LENGTH, "%s", argsstr.s_name)) < 0)
				Perror("argsstr.args");

			argsstr.args[n] = '\0';

// 			argsstr.retval = 0;
// 			return argsstr;
		}
		else{
/*
 * parse the last argument
 */		
			if(*pc == '=') pc++;
			
			while(*pc == ' ' && *pc != '\0'  )pc++;			/* jump over possible spaces */
					
			i = 0;
			while(*pc != '\0' ){
				while(*pc == ' ' && *pc != '\0')pc++;
				argsstr.args[i++] = *pc++;
				if(i > MAX_NAME_LENGTH){
					Error(" too long argument field");
					argsstr.retval = -1;
					return argsstr;
				}
			}
			
			argsstr.args[i] = '\0';
		}
	}
	else
	{
/*
 * data set will be specified
 * jump over = and spaces
 */
		argsstr.arg = arg;

		i = 0;
		while(*pc != '\0' && *pc != '='){
			while(*pc == ' ' && *pc != '\0')pc++;
				argsstr.s_name[i++] = *pc++;
				if(i > MAX_NAME_LENGTH){
					Error(" too long argument field");
					argsstr.retval = -1;
					return argsstr;
			}
		}
		
 		argsstr.s_name[i] = '\0';
		
		if(*pc == '=') pc++;
		
		while(*pc == ' ' && *pc != '\0'  )pc++;			/* jump over possible spaces */
				
		i = 0;
		while(*pc != '\0' ){
			while(*pc == ' ' && *pc != '\0')pc++;
			argsstr.args[i++] = *pc++;
			if(i > MAX_NAME_LENGTH){
				Error(" too long argument field");
				argsstr.retval = -1;
				return argsstr;
			}
		}
		
		argsstr.args[i] = '\0';
	}
	argsstr.retval = 0;
	return argsstr;
}



