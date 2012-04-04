#ifndef __PERROR_H__
#define __PERROR_H__

void Perror(const char *);

#endif

#ifndef __WARNING_H__
#define __WARNING_H__

void Warning(const char *);

#endif

#ifndef __ERROR_H__
#define __ERROR_H__

void Error(const char *);

#endif


#ifndef __SIG_CHILD_H__
#define __SIG_CHILD_H__

void sig_chld(int );

#endif


#ifndef __FORK_H__
#define __FORK_H__

int Fork(void);

#endif

#ifndef __STROL_H__
#define __STROL_H__

SIZE_T Strol(char *);

#endif


#ifndef __STRTOLOWER_H__
#define __STRTOLOWER_H__

char *StrToLower(char *); 

#endif

