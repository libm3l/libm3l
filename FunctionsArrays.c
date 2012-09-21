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
#include "FunctionsArrays.h"

/*
 * function get index of 1D arrat from n-dimensional array
 */
size_t get_ind(size_t ndim, size_t *ind,  size_t *dim)
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
size_t get_4nd(size_t i, size_t j, size_t k, size_t l, size_t im, size_t jm, size_t km, size_t lm)
{
	size_t ret_ind; 	
 	ret_ind = i*jm*km*lm+j*km*lm+k*lm+l;
	
	return ret_ind;
}

/*
 * function get index of 1D arrat from 3-dimensional array
 */
size_t get_3ind(size_t i, size_t j, size_t k, size_t im, size_t jm, size_t km)
{
	size_t ret_ind; 	
 	ret_ind = i*jm*km+j*km+k;
	
	return ret_ind;
}

/*
 * function get index of 1D arrat from 3-dimensional array
 */
size_t get_2ind(size_t i, size_t j, size_t im, size_t jm)
{
	size_t ret_ind; 	
 	ret_ind = i*jm+j;
	
	return ret_ind;
}