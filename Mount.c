
 
#include "Header.h"
#include "format_type.h"
#include "internal_format_type.h"

#include "Mount.h"
#include "FunctionsPrt.h"
#include "udf_rm.h"


/*
 * function creates the master head node of a specified name
 */

node_t *Mount(const char *name)
{
	tmpstruct_t TMPSTR;
	node_t *Dnode;
/*
 * copy name of the node to the TMPSTR
 */
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", name) < 0)
		Perror("snprintf");
/*
 * set type of node DIR
 */
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", "DIR") < 0)
		Perror("snprintf");
/*
 * node does not contain any data, set ndim = 0
 */
	TMPSTR.ndim = 0;
/*
 * allocate node and if success, return it otherwise return NULL
 */	
	if( (Dnode = AllocateNode(TMPSTR)) == NULL){
		Error("Allocate");
		return NULL;
	}
	
	return Dnode;
}
