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
