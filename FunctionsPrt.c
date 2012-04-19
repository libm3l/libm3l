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

	if ( (segs = (char**)malloc( (count)*sizeof(char **) )) == NULL)
		Perror("malloc");
	if ( (len = (size_t *)malloc( (count)*sizeof(size_t *) )) == NULL)
		Perror("malloc");
	
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
	len[i] = strlen(Tmp->name);
	if ( (segs[i] = (char*)malloc( (len[i]+1)*sizeof(char *) )) == NULL)
			Perror("malloc");
		if( snprintf(segs[i], len[i]+1,"%s",Tmp->name) < 0)
			Perror("snprintf");

	tot_len = tot_len + len[i] + 1;
		
	if ( (path = (char*)malloc( (tot_len + 1)*sizeof(char **) )) == NULL)
		Perror("malloc");
	
	tot_len1 = 0;
	
	for (i = count; i-- > 0; ){
		path[tot_len1++] = '/';
		for(j=0; j<len[i]; j++){
			path[tot_len1++] = segs[i][j];}
	}
	if( tot_len1 > tot_len)
		Error("malloc problem in Path");
		
	path[tot_len1] = '\0';
	
	for(i=0; i<count; i++)
		free(segs[i]);
	free(segs);
	free(len);
	
	return path;
	
}