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
 *     Header file udf_rm.h
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



#ifndef  __FREE_H__
#define  __FREE_H__

extern lmint_t m3l_Free(node_t **);
extern lmint_t m3l_Free_data_str(node_t **);

extern node_t *m3l_AllocateNode(tmpstruct_t, opts_t *);
extern lmint_t m3l_AllocateNodeData(node_t **, tmpstruct_t, opts_t *);

#endif
