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
 *     Header file format_type.h
 *
 *     Date: 2012-07-01
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



#ifndef __DATA_H__
#define __DATA_H__

typedef union{
	void *v;
/*
 *  reals
*/
	lmfloat_t         	*f;
	lmdouble_t        	*df;
	lmlongdouble_t  	*ldf;
/*
 * chars
 */
	lmchar_t           	*c;
	lmsignchar_t    	*sc;
	lmusignchar_t  		*uc;
/*
 * integers
 */
// 	short  int         *si;
// 	unsigned short int *usi;
// 	int           *i;
// 	unsigned int  *ui;
// 	long  int     *li;
// 	unsigned long int       *uli;
// 	long long int           *lli;
// 	signed long long int    *slli;
// 	unsigned long long  int *ulli;

	lmshint_t		*si;
	lmushint_t	 	*usi;
	lmint_t			*i;
	lmuint_t		*ui;
	lmlint_t		*li;
	lmulint_t		*uli;
	lmllint_t		*lli;
	lmsllint_t		*slli;
	lmsllint_t 		*ulli;

	lmsize_t		 *st;
	lmptrdiff_t		 *ptrdf;

} data_t;


typedef struct node {
/*
 *      specifies type of structure
 *                      f  - float
 *                      d - double
 *                      lf - long double
 *                      c - char
 *                      i - integer
 *                      ui - unsigned integer
 *                      li - long integer
 *                      uli - unsigned long integer
 *                      slli  - signed long long integer
 *                      ulli  - unsigned long long integer
 */
 
 /*
  *  spedifies type of the data set
  */
	lmchar_t *type;
 /*
  *  name of the data set
  */ 
	lmchar_t *name;
 /*
  * if list contains field, give a number of dimensions (ndim) 
  * and field dimensions (fdim)
  */
	lmsize_t *fdim;
	lmsize_t ndim;
	data_t data;
 /* 
  * pointers to the next list, parent list and child list and if the list is link, the original of the link
  */
	struct node *next, *prev, *parent, *child;
 /*
  * structure used for linking information, lcounter is a number of linknode array
  */
	struct find_str **linknode;
	lmsize_t lcounter;
 /*
  * parameter specifying if data.[type] has been malloc'ed lmchar_t != 'n' or not lmchar_t == 'n'
  */	
	lmchar_t no_malloc;

}node_t;


/*
 * find_t structure
 */
typedef struct find_str{
	node_t *List;
} find_str_t;

typedef struct find{
	find_str_t **Found_Nodes;
	lmsize_t founds; 		/* number of founds */
	node_t *Home_Node;  	/* node from which the found was originated */
} find_t;


#endif
