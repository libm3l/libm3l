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
 *     Header file FunctionsArrays.h
 *
 *     Date: 2012-07-06
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 *
 */





#ifndef __FUNCTIONSARRAYS_H__
#define __FUNCTIONSARRAYS_H__

extern size_t m3l_get_ind(size_t , size_t *, size_t *);
extern size_t m3l_get_4ind(size_t , size_t , size_t , size_t, size_t, size_t, size_t, size_t);
extern size_t m3l_get_3ind(size_t , size_t , size_t , size_t, size_t, size_t);
extern size_t m3l_get_2ind(size_t , size_t , size_t , size_t );
extern void *m3l_get_data_pointer(node_t *);

#endif
