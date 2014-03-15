/*
 *     Copyright (C) 2013  Adam Jirasek
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
 *     Header file Set_Default_Parameters.h
 *
 *     Date: 2013-10-26
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 * 
 */






#ifndef __SET_DEFAULT_PARAMETERS_H__
#define __SET_DEFAULT_PARAMETERS_H__

// #include "internal_format_type.h"

extern void m3l_set_Add(opts_t **);
extern void m3l_set_Cat(opts_t **);
extern void m3l_set_Cp(opts_t **);
extern void m3l_set_Detach_List(opts_t **);
extern void m3l_set_Find(opts_t **);
extern void m3l_set_Fread(opts_t **);
extern void m3l_set_Fwrite(opts_t **);
extern void m3l_set_Send_to_tcpipsocket(opts_t **);
extern void m3l_set_Send_receive_tcpipsocket(opts_t **);
extern void m3l_set_Receive_send_tcpipsocket(opts_t **);
extern void m3l_set_Receive_tcpipsocket(opts_t **);
extern void m3l_set_MkTCPIPHeader(opts_t **);

#endif
