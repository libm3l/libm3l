/*
 *     Copyright (C) 2012  Adam Jirasek
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *     
 *     contact: libm3l@gmail.com
 * 
 */



/*
 *     Header file CrtSEMSHM.h
 *
 *     Date: 2013-02-22
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 * 
 */




#ifndef __CRTSEM_H__
#define __CRTSEM_H__

int CrtSMH_Int(size_t, int);
int CrtSMH_Char(size_t, int);
int DelSMH_Int(const void *, int);
int DelSMH_Char(int *, int);
int CrtSEM(int , int );
int DelSEM(int );


#endif
