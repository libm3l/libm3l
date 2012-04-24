/*
 * Header file with declarations
 * copyright � 2012 Adam Jirasek
 */

#ifndef __DATA_H__
#define __DATA_H__

typedef union{
	void *v;
/*
 *  reals
*/
	float         *f;
	double        *df;
	long  double  *ldf;
/*
 * chars
 */
	char           *c;
	signed char    *sc;
	unsigned char  *uc;
/*
 * integers
 */
	short  int         *si;
	unsigned short int *usi;
	int           *i;
	unsigned int  *ui;
	long  int     *li;
	unsigned long int       *uli;
	long long int           *lli;
	signed long long int    *slli;
	unsigned long long  int *ulli;

	size_t *st;
	ptrdiff_t *ptrdf;

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
	char *type;
 /*
  *  name of the data set
  */ 
	char *name;
 /*
  * if list contains field, give a number of dimensions (ndim) 
  * and field dimensions (fdim)
  */
	size_t *fdim;
	size_t ndim;
	data_t data;
 /* 
  * pointers to the next list, parent list and child list and if the list is link, the original of the link
  */
	struct node *next, *prev, *parent, *child, *link;
 /*
  * structure used for linking information, lcounter is a number of linknode array
  */
	struct node_t **linknode;
	size_t lcounter;

} node_t;


/*
 * find_t structure
 */
typedef struct find{
	node_t *List;
} find_t;



#endif