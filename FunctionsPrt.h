#include "internal_format_type.h"

#ifndef __FUNCTIONPRT_H__
#define __FUNCTIONPRT_H__

void Perror(const char *);

void Warning(const char *);

void Error(const char *);

void sig_chld(int );

int Fork(void);

SIZE_T Strol(char *);

char *StrToLower(char *); 

char *Path(node_t *, node_t *);

path_t parse_path(const char *);

int destroy_pars_path(path_t *);

get_arg_t get_arguments(const char *);

#endif

