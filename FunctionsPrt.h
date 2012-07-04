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
 *     Author: Adam Jirasek
 *     Date: 2012-05-21
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		Author			Descritpion
 *
 */



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

path_t *parse_path(const char *);

void destroy_pars_path(path_t **);

get_arg_t get_arguments(const char *);

#endif

