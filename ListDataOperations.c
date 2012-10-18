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
 *     Function FunctionsArrays.c
 *
 *     Date: 2012-07-06
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




#include "Header.h"
#include "format_type.h"
#include "ListDataOperations.h"

/*
 * function get index of 1D arrat from n-dimensional array
 */
size_t m3l_get_ind(size_t ndim, size_t *ind,  size_t *dim)
{
	size_t ret_ind, tmp,k,l;
	
	ret_ind = 0;
	for (k=0; k<ndim-1; k++){
		
		tmp = 1;
		
		for (l=k+1; l<ndim; l++)
			tmp = tmp*dim[l];
		
		ret_ind = ret_ind + ind[k]*tmp;
		
	}
	ret_ind = ret_ind + ind[ndim-1];
// 	
//  	ret_ind = i*jm*km+j*km+k;
	
	return ret_ind;
}

/*
 * function get index of 1D arrat from 3-dimensional array
 */
size_t m3l_get_4nd(size_t i, size_t j, size_t k, size_t l, size_t im, size_t jm, size_t km, size_t lm)
{
	size_t ret_ind; 	
 	ret_ind = i*jm*km*lm+j*km*lm+k*lm+l;
	
	return ret_ind;
}

/*
 * function get index of 1D arrat from 3-dimensional array
 */
size_t m3l_get_3ind(size_t i, size_t j, size_t k, size_t im, size_t jm, size_t km)
{
	size_t ret_ind; 	
 	ret_ind = i*jm*km+j*km+k;
	
	return ret_ind;
}

/*
 * function get index of 1D arrat from 3-dimensional array
 */
size_t m3l_get_2ind(size_t i, size_t j, size_t im, size_t jm)
{
	size_t ret_ind; 	
 	ret_ind = i*jm+j;
	
	return ret_ind;
}

/*
 * function gives pointer on ->data.# according to List type
 */
void *m3l_get_data_pointer(node_t *Lnode)
{
	
	if(strncmp( Lnode->type,"DIR",3) == 0)return NULL;
	
	if (strncmp(Lnode->type,"LD",2) == 0){  /* long double */
		return (void *)Lnode->data.ldf;
	}
	else if(strncmp(Lnode->type,"D",1) == 0){  /* double */
		return (void *)Lnode->data.df;
	}
	else if(strncmp(Lnode->type,"F",1) == 0){  /* float */
		return (void *)Lnode->data.f;
	}
/*
 * chars
 */
	else if (strncmp(Lnode->type,"SC",2) == 0){  /* signed char */
		return (void *)Lnode->data.sc;
	}
	else if(strncmp(Lnode->type,"UC",2) == 0){  /* unsigned char */
		return (void *)Lnode->data.uc;
	}
	else if(strncmp(Lnode->type,"C",1) == 0){  /* char */
		return (void *)Lnode->data.c;
	}
/*
 * integers
 */
	else if(strncmp(Lnode->type,"ULLI",4) == 0){  /* unsigned long long  int */
		return (void *)Lnode->data.ulli;
	}
	else if(strncmp(Lnode->type,"SLLI",4) == 0){  /* signed long long  int */
		return (void *)Lnode->data.slli;
	}
	else if(strncmp(Lnode->type,"LLI",3) == 0){  /* long long int */
		return (void *)Lnode->data.lli;
	}
	else if(strncmp(Lnode->type,"ULI",3) == 0){  /* unsigned long int */
		return (void *)Lnode->data.uli;
	}
	else if(strncmp(Lnode->type,"USI",3) == 0){  /* unsigned short int */
		return (void *)Lnode->data.usi;
	}
	else if(strncmp(Lnode->type,"SI",2) == 0){  /* short int */
		return (void *)Lnode->data.si;
	}
	else if(strncmp(Lnode->type,"UI",2) == 0){  /* unsigned int */
		return (void *)Lnode->data.ui;
	}
	else if(strncmp(Lnode->type,"LI",2) == 0){  /* long  int */
		return (void *)Lnode->data.li;
	}
	else if(strncmp(Lnode->type,"I",1) == 0){  /* int */
		return (void *)Lnode->data.i;
	}
/*
 * counters
 */
	else if(strncmp(Lnode->type,"ST",2) == 0){  /* size_t */
		return (void *)Lnode->data.st;
	}
	else if(strncmp(Lnode->type,"PTRDF",1) == 0){  /* ptrdf_t */
		return (void *)Lnode->data.ptrdf;
	}
	
	return NULL;
}
