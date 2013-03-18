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

extern void Perror(const lmchar_t *);
extern void Warning(const lmchar_t *);
extern void Error(const lmchar_t *);
extern lmsize_t Strol(lmchar_t *);
extern Uint64_t Strtoull(lmchar_t *, lmint_t);
extern lmulint_t Strtoul(lmchar_t *, lmint_t);
extern lmchar_t *StrToLower(lmchar_t *); 
extern lmchar_t *m3l_Path(node_t *, node_t *);
extern path_t *m3l_parse_path(const lmchar_t *);
extern void m3l_destroy_pars_path(path_t **);
extern get_arg_t m3l_get_arguments(const lmchar_t *);

extern void *RD_MemcpyD(Uint64_t *, lmchar_t *, lmsize_t );
extern void *RD_MemcpyF(Uint32_t *, lmchar_t *, lmsize_t );
extern void *RD_StrtoullD(Uint64_t *, lmchar_t *, lmsize_t );
extern void *WR_MemcpyD(lmchar_t *, Uint64_t *, lmsize_t);
extern void *WR_snprintfD(lmchar_t *, Uint64_t *, lmsize_t);
extern void *WR_MemcpyF(lmchar_t *, Uint32_t *, lmsize_t);
extern void *WR_snprintfF(lmchar_t *, Uint32_t *, lmsize_t);

#endif

