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
 *     Header file tcpip_socket_op.h
 *
 *     Date: 2012-05-18
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 *
 */



#ifndef __TCPIP_SOCKET_OP_H__
#define __TCPIP_SOCKET_OP_H__

#ifdef __cplusplus 
#define CPP_C "C"
#else
#define CPP_C
#endif

extern CPP_C  lmint_t m3l_server_openbindlistensocket(lmint_t, lmchar_t* Options, ...);
extern CPP_C  lmint_t m3l_cli_open_socket(const lmchar_t *, lmint_t, lmchar_t* Options, ...);

#endif
