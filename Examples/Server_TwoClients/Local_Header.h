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
 *     Header file Local_Header.h
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


#ifndef __LOCAL_HEADER_H__
#define __LOCAL_HEADER_H__

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/stat.h>

lmint_t id, shmid, shmid1;  /* Internal identifier of the semaphore. */
lmint_t *shm_n;
lmchar_t *shm_buff;
node_t *TmpACKN;

#endif
