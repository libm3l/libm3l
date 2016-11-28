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
    
    lmdouble_t *darr;
    lmlongdouble_t *ldarr;
    lmfloat_t *farr;
    
    lmint_t *iarr;
    lmusignchar_t *ucarr;
    lmchar_t *carr;
    
    lmsize_t i;
    
/*
 * if not in communicator, return
 */
    if(communicator == MPI_COMM_NULL)return;
 
    tot_dim  = 1;
    for (i=0; i< Lnode->ndim; i++)
        tot_dim = tot_dim * Lnode->fdim[i];
    
    
  	if (strncmp(Lnode->type,"LD",2) == 0){  /* long double */
        if( (ldarr = malloc(sizeof(lmlongdouble_t)*tot_dim)) == NULL)
            Perror("malloc");
        for(i=0; i<tot_dim; i++)
            ldarr[i]=Lnode->data.ldf[i];
            
        MPI_Allreduce(Lnode->data.ldf, ldarr, tot_dim, MPI_LONG_DOUBLE, MPI_SUM, communicator);
         for(i=0; i<tot_dim; i++)
            Lnode->data.ldf[i]=ldarr[i];
         
         free(ldarr);        
        
	}
	else if(strncmp(Lnode->type,"D",1) == 0){  /* double */ 
        if( (darr = malloc(sizeof(lmdouble_t)*tot_dim)) == NULL)
            Perror("malloc");
        for(i=0; i<tot_dim; i++)
            darr[i]=Lnode->data.df[i];
            
        MPI_Allreduce(Lnode->data.df, darr, tot_dim, MPI_DOUBLE, MPI_SUM, communicator);
         for(i=0; i<tot_dim; i++)
            Lnode->data.df[i]=darr[i];
         
         free(darr);
	}
	else if(strncmp(Lnode->type,"F",1) == 0){  /* float */ 
        if( (farr = malloc(sizeof(lmfloat_t)*tot_dim)) == NULL)
            Perror("malloc");
        for(i=0; i<tot_dim; i++)
            farr[i]=Lnode->data.f[i];
            
        MPI_Allreduce(Lnode->data.f, farr, tot_dim, MPI_FLOAT, MPI_SUM, communicator);
         for(i=0; i<tot_dim; i++)
            Lnode->data.f[i]=farr[i];
         
         free(farr);
	}
/*
 * chars
 */
	else if(strncmp(Lnode->type,"UC",2) == 0){  /* char */
        if( (ucarr = malloc(sizeof(lmusignchar_t)*tot_dim)) == NULL)
            Perror("malloc");
        for(i=0; i<tot_dim; i++)
            ucarr[i]=Lnode->data.uc[i];
            
        MPI_Allreduce(Lnode->data.uc, ucarr, tot_dim, MPI_UNSIGNED_CHAR, MPI_SUM, communicator);
         for(i=0; i<tot_dim; i++)
            Lnode->data.f[i]=ucarr[i];
         
         free(ucarr);
	}
// 	else if (strncmp(Lnode->type,"SC",2) == 0){  /* signed char */
// 	}
	else if(strncmp(Lnode->type,"C",1) == 0){  /* char */ 
                if( (carr = malloc(sizeof(lmchar_t)*tot_dim)) == NULL)
            Perror("malloc");
        for(i=0; i<tot_dim; i++)
            carr[i]=Lnode->data.c[i];
            
        MPI_Allreduce(Lnode->data.c, carr, tot_dim, MPI_CHAR, MPI_SUM, communicator);
         for(i=0; i<tot_dim; i++)
            Lnode->data.c[i]=carr[i];
         
         free(carr);
	}
/*
 * integers
 */
// 	else if(strncmp(Lnode->type,"ULLI",4) == 0){  /* unsigned long long  int */
// 	}
// 	else if(strncmp(Lnode->type,"SLLI",4) == 0){  /* signed long long  int */
// 	}
// 	else if(strncmp(Lnode->type,"LLI",3) == 0){  /* signed long long  int */
// 	}
// 	else if(strncmp(Lnode->type,"ULI",3) == 0){  /* unsigned long int */  MPI_UNSIGNED_LONG;
// 	}
// 	else if(strncmp(Lnode->type,"USI",3) == 0){  /* unsigned short int */ MPI_UNSIGNED_SHORT;
// 	}
// 	else if(strncmp(Lnode->type,"SI",2) == 0){  /* short int */ MPI_SHORT;
// 	}
// 	else if(strncmp(Lnode->type,"UI",2) == 0){  /* unsigned int */MPI_UNSIGNED_INT;
// 	}
// 	else if(strncmp(Lnode->type,"LI",2) == 0){  /* long  int */MPI_LONG;
// 	}
// 	else if(strncmp(Lnode->type,"I",1) == 0){  /* int */ MPI_INT;
// 	}
// 	else if(strncmp(Lnode->type,"ST",1) == 0){  /* int */ MPI_LONG;
// 	}
//     else if(strncmp(Lnode->type,"PTRT",1) == 0){  /* int */ MPI_AINT;
// 	}
/*
 * not supported
 */
	else
	{
		Warning("Unknown type");
	}
        
}

