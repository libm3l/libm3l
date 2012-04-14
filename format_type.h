/*
 * Header file with declarations
 * copyright � 2012 Adam Jirasek
 */

#ifndef __DATA__
#define __DATA__

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
#endif

#ifndef  __NODE_T__
#define  __NODE_T__

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
  * pointers to the next list, parent list and child list
  */
	struct node *next, *prev, *parent, *child;
} node_t;

#endif


#ifndef  __FIND_T__
#define __FIND_T__
/*
 * find_t structure
 */
typedef struct find{
	node_t *List;
	char   *path;
} find_t;

#endif