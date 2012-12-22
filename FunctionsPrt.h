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
 *     Header file FunctionsPrt.h
 *
 *     Date: 2012-05-21
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 *
 */



#include "internal_format_type.h"

#ifndef  __FUNCTIONPRT_H__
#define  __FUNCTIONPRT_H__

extern void Perror(const char *);
extern void Warning(const char *);
extern void Error(const char *);
extern void sig_chld(int );
extern int Fork(void);
extern SIZE_T Strol(char *);
extern unsigned long long Strtoull(char *, int);
extern unsigned long Strtoul(char *, int);
extern char *StrToLower(char *); 
extern char *m3l_Path(node_t *, node_t *);
extern path_t *m3l_parse_path(const char *);
extern void m3l_destroy_pars_path(path_t **);
extern get_arg_t m3l_get_arguments(const char *);
extern void *RD_MemcpyD(uint64_t *, char *, size_t );
extern void *RD_MemcpyF(uint32_t *, char *, size_t );
extern void *RD_StrtoullD(uint64_t *, char *, size_t );
extern void *WR_MemcpyD(char *, uint64_t *, size_t);
extern void *WR_snprintfD(char *, uint64_t *, size_t);
extern void *WR_MemcpyF(char *, uint32_t *, size_t);
extern void *WR_snprintfF(char *, uint32_t *, size_t);

#endif

