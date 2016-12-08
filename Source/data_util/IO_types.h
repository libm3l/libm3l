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
 *     Header file IO_type.h
 *
 *     Date: 2016-11-25
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 *
 */


#include"libm3l_header.h"

#ifndef   __IOSTRUCT_T__
#define  __IOSTRUCT_T__

#ifndef CPP_C
#ifdef __cplusplus 
#define CPP_C "C"
#else
#define CPP_C
#endif
#endif

#define IOLEN  MAX_TYPE_LENGTH + MAX_NAME_LENGTH + 8
#define FLLIOLEN  16 + 4 + 8 + 8


typedef struct IOstr{
     lmchar_t *Name; 
	 lmchar_t *Type;
 	 lmsize_t ndim;
}IOstr_t;



#endif
