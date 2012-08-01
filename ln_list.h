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
 *     Header file ln_list.h
 *
 *     Date: 2012-06-30
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 *
 */



#ifndef __LN_LIST_H__
#define __LN_LIST_H__

extern size_t m3l_ln_caller(node_t **, const char *, const char *, node_t **, const char *, const char *, opts_t *);
extern int m3l_AllocateLinkInfo(node_t **, node_t *);
extern size_t m3l_ln_cleanemptylinks(node_t **,  opts_t *);
extern int m3l_ln_cleanemptylinksref(node_t **,  opts_t *);

#endif
