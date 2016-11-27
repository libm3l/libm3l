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
 *     Function ReadBDescriptor.c
 *
 *     Date: 2012-06-30
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




 
#include "libm3l_header.h"
#include "format_type.h"
#include "internal_format_type.h"
#include "format_conversion_spec.h"

#include "FunctionsPrt.h"
#include "udf_rm.h"

#include "IO_types.h"


#define EXPR       (**pc != ' ' && **pc != '\t' && **pc != '\n' && **pc != '\0')
#define IFEXPR     ((**pc == ' ' || **pc == '\t' || **pc == '\n') && **pc != '\0')
#define LASTEXPR   (lastchar != ' ' && lastchar != '\t' && lastchar != '\n' && lastchar != '\0')


// static lmint_t m3l_read_Bfile_data_line(lmchar_t *, lmchar_t **, lmssize_t *,node_t **, tmpstruct_t, FILE *f, opts_t *);
// static lmint_t m3l_read_Bfile_data_charline(lmchar_t *, lmchar_t **, lmssize_t *,node_t **, tmpstruct_t, FILE *f);
static node_t *m3l_read_Blist(tmpstruct_t , FILE *f, opts_t *);
static node_t *m3l_read_Bfile_data(FILE *f, opts_t *);
static node_t *m3l_read_file_Bthreadsafe(FILE *, lmsize_t *, opts_t *);

/*
 * Function read just one line from a socket, disregarding comments line
 * It identifies if the line is a header of DATA or DIR list
 * If DATA type of list it allocates a node_t and calls ReadDataLine which reads the
 * actual conent of the list
 * If the list if of DIR type, it calls ReadDir routine which reads DIR types of list (recursive calling)
 */ 

node_t *m3l_read_Bfile(FILE *fp, opts_t *Popts){
	
	lmssize_t pos, *ppos;;
	node_t *Lnode;
	
    ppos = &pos;
    pos =1 ;
	Lnode = m3l_read_file_Bthreadsafe(fp, ppos, Popts);

	return Lnode;
	
}


node_t *m3l_read_file_Bthreadsafe(FILE *fp, lmsize_t *pos, opts_t *Popts)
{
    IOstr_t IOstruct, *pIOstruct;
	pIOstruct = &IOstruct;
    lmsize_t ngotten;
    tmpstruct_t TMPSTR;	
    node_t *Dnode;


    fseek( fp, *pos, SEEK_SET );
	
	if(   (ngotten = fread(pIOstruct ,IOLEN,  1 , fp ))   < 0){
		Perror("fread");
	}
	
     printf(" DATA are:    '%s'  '%s'  %ld \n" ,  pIOstruct->Type, pIOstruct->Name ,   pIOstruct->ndim);
     
    if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s",  pIOstruct->Type) < 0)
        Perror("snprintf");
    if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s",  pIOstruct->Name) < 0)
        Perror("snprintf");
     
    TMPSTR.ndim = pIOstruct->ndim;
    TMPSTR.dim=NULL;
     
    if( (Dnode = m3l_read_Blist(TMPSTR, fp, Popts)) == NULL)
        Perror("ReadDirData - ReadDir");
     
    return Dnode;
}



/*
 * reads data after line identifying DIR
 */

node_t *m3l_read_Blist(tmpstruct_t TMPSTR, FILE *fp, opts_t *Popts)
{
	lmsize_t i;
	node_t *Dnode, *Tmpnode, *Pnode;
 /*
 * allocate node
 */	
	if( (Dnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
		Error("Allocate");
	}

	for(i=1;i<=TMPSTR.ndim; i++){

		Tmpnode=NULL;	

// 		if ( (Tmpnode = m3l_read_Bfile_data(fp, Popts)) == NULL)
			Error("ReadDirData: ReadData");
/*
 * add to node
 */
		if(i ==1){
			Pnode         = Tmpnode;
			Dnode->child  = Pnode;
			Pnode->parent = Dnode;
		}
		else
		{
			Pnode->next      = Tmpnode;
			Tmpnode->prev    = Pnode;
			Tmpnode->parent  = Dnode;
			 Pnode            = Tmpnode;
		}
	}
	return Dnode;
}
