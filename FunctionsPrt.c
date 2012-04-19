/*
 * copyright ï¿½ 2012 Adam Jirasek
 */
 
#include "Header.h"
#include "format_type.h"
#include "FunctionsPrt.h"

 
int sig_pid = 9999;

/*
 *
  */
void Perror(const char *msg)
{
	perror(msg);
	exit(1);
}

void Error(const char *msg)
{
	printf("ERROR:  %s\n", msg);
	exit(1);
}

/*
 *
  */
void Warning(const char *msg)
{
	printf("Warning:  %s\n", msg);
	exit(1);
}

/*
 *
  */
void sig_chld(int signo)
{
	pid_t   pid;
	int	stat;
	while( (sig_pid = waitpid(-1, &stat, WNOHANG)) >0 );
	return;
}

/*
 *
  */
int Fork(void)
{
	int childpid;
        if ( (childpid = fork()) < 0) 
		Perror("fork()");
	return childpid;
}

/*
 *
  */
SIZE_T Strol(char *text)
{
  char *err;
  SIZE_T i;

	i = strtol(text,&err,0);
	if (err == text){
	  Error("Strol:  Error when reading and converting integer number");} /* complete garbage input */ 
	else if (*err != 0){
	  Perror("Strol:  Error when reading and converting integer number");}/* trailing garbage */
	else if ((i == LONG_MAX || i == LONG_MIN) && errno == ERANGE)
	  Perror("strol");
	return i;
}


/*
 * strig to lower case
 */
char *StrToLower(char *s) 
{ 
	char *cs;

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
char *StrToUpper(char *s) 
{ 
	char *cs;

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

/*
 * function allocates sting and saves the 
 * path of the node in it. The string is a return parameter
 * NOTE - needs to be feed after use
 */

char *Path(node_t *List)
{
	char   *path;
	char   **segs;
	node_t *Tmp;
	size_t count,i,*len, tot_len, tot_len1, j;
/*
 * find the number of all nodes up to Master Head Node
 */
	Tmp = List;
	count = 1;
	while(Tmp->parent != NULL){
		count++;
		Tmp = Tmp->parent;
	}
/*
 * allocate arrays
 */

	if ( (segs = (char**)malloc( (count)*sizeof(char **) )) == NULL)
		Perror("malloc");
	if ( (len = (size_t *)malloc( (count)*sizeof(size_t *) )) == NULL)
		Perror("malloc");
/*
 * fill segs with segments of the path
 */
	Tmp = List;
	i=0;
	tot_len = 0;
	while(Tmp->parent != NULL){
		len[i] = strlen(Tmp->name);

		if ( (segs[i] = (char*)malloc( (len[i]+1)*sizeof(char *) )) == NULL)
			Perror("malloc");
		if( snprintf(segs[i], len[i]+1,"%s",Tmp->name) < 0)
			Perror("snprintf");
		
		Tmp = Tmp->parent;
		tot_len = tot_len + len[i] + 1;
		i++;
	}
/*
 * save length of the segment
 */
	len[i] = strlen(Tmp->name);
	if ( (segs[i] = (char*)malloc( (len[i]+1)*sizeof(char *) )) == NULL)
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
	if ( (path = (char*)malloc( (tot_len + 1)*sizeof(char **) )) == NULL)
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


path_t *parse_path(const char *path)
{
	path_t *Path;
	char **text;
	char *pc;
	size_t counter,i, j, st,k;

	char abspath;

	abspath = 'R';
/*
 * parse the path
 */
	if(path[0] == '/'){
		counter = 0;
		i = 0;
		abspath = 'A';
	}
	else if(path[0] == '.' && path[1] == '/'){
		counter = 0;
		i = 1;
	}
	else{
		i = 0;
		counter = 1;
	}

	while(path[i] != '\0'){
/*
 * if symbol is / and not the end of the string 
 */
		if(path[i++]  == '/' && path[i] != '\0'){
/*
 * ignore all / symbols following immediatelly this one
 */
			while( path[i] == '/' && path[i] != '\0') i++;
/*
 * increase counter of the words in string
 */
		 	counter++;

			if(path[i] == '\0'){
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
	if ( (text = (char **)malloc(counter * sizeof(char **))) == NULL)
		perror("malloc");

	for(i=0; i< counter; i++)
		if ( (text[i] = (char *)malloc( (MAX_NAME_LENGTH + 1) * sizeof(char *))) == NULL)
			perror("malloc");
/*
 * store individual words in array
 */
	if(path[0] == '/'){
		i = 2;
	}
	else if(path[0] == '.' && path[1] == '/'){
		i = 2;
	}
	else
	{
		i= 0;
	}

	st = 0;
	j = 0;

		while(path[i] != '\0'){
		if( st < MAX_NAME_LENGTH)
			text[j][st++] = path[i];

		if(path[i++]  == '/' && path[i] != '\0'){
			while( path[i] == '/' && path[i] != '\0') i++;
			text[j][st-1] = '\0';
			st = 0;
			j++;
			if(j > counter) exit(0);
			
			if(path[i] == '\0')
				break;
		}
	}
/*
 * allocate pointer to Path structure and populate it
 */	
	if ( (Path = (path_t*)malloc( sizeof(path_t *) )) == NULL)
		Perror("malloc");
	
	Path->path = text;  		/* segments of path */
	Path->abspath = abspath;	/* Realative (R) or absolute (A) path */
	Path->seg_count = counter;	/* Number of segments in path */
		
	return Path;
}

/*
 * function frees pointer allocated in parse_path function
 */
int destroy_pars_path(path_t **Path)
{
	size_t i;
	
	for (i=0; i< (*Path)->seg_count; i++)
		free( (*Path)->path[i]);
	free( (*Path)->path);
	
	free( (*Path));
	(*Path) = NULL;
		
}