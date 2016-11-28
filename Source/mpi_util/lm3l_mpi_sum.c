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
 *     Function add_list.c
 *
 *     Date: 2016-10-26
 * 
 * 
 *
 *
 *     Description:
 * 
 *
 *     Input parameters:
 * 
 *
 *     Return value:
 * 
 * 
 *
 *     Modifications:
 *     Date		Version		Patch number		CLA 
 *
 *
 *     Description
 *
 */
#include "../data_util/libm3l.h"
#include "mpi.h"


lmint_t m3l_mpi_sum(lmint_t communicator, node_t *Lnode){
    
    lmsize_t tot_dim;
/*
 * if not in communicator, return
 */
    if(communicator == MPI_COMM_NULL)return;
    
  	if (strncmp(Lnode->type,"LD",2) == 0){  /* long double */
	}
	else if(strncmp(Lnode->type,"D",1) == 0){  /* double */
	}
	else if(strncmp(Lnode->type,"F",1) == 0){  /* float */
	}
/*
 * chars
 */
	else if(strncmp(Lnode->type,"UC",2) == 0){  /* char */
	}
	else if (strncmp(Lnode->type,"SC",2) == 0){  /* signed char */
	}
	else if(strncmp(Lnode->type,"C",1) == 0){  /* char */
	}
/*
 * integers
 */
	else if(strncmp(Lnode->type,"ULLI",4) == 0){  /* unsigned long long  int */
	}
	else if(strncmp(Lnode->type,"SLLI",4) == 0){  /* signed long long  int */
	}
	else if(strncmp(Lnode->type,"LLI",3) == 0){  /* signed long long  int */
	}
	else if(strncmp(Lnode->type,"ULI",3) == 0){  /* unsigned long int */
	}
	else if(strncmp(Lnode->type,"USI",3) == 0){  /* unsigned short int */
	}
	else if(strncmp(Lnode->type,"SI",2) == 0){  /* short int */
	}
	else if(strncmp(Lnode->type,"UI",2) == 0){  /* unsigned int */
	}
	else if(strncmp(Lnode->type,"LI",2) == 0){  /* long  int */
	}
	else if(strncmp(Lnode->type,"I",1) == 0){  /* int */
	}
/*
 * not supported
 */
	else
	{
		Warning("Unknown type");
	}
        
}

