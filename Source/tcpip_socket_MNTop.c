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
 *     Function tcpip_socket_MNTop.c
 *
 *     Date: 2012-09-21
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
#include "FunctionsPrt.h"

#include "tcpip_socket_IOop.h"
#include "tcpip_socket_op.h"
#include "Write2Socket.h"
#include "ReadSocket.h"
#include "udf_rm.h"
#include "Mklist.h"

node_t *m3l_ACKN(lmchar_t *answer)
{
	node_t *Gnode, *TmpNode;
	lmsize_t *dim;
	
	if(  (Gnode = m3l_Mklist("ackn", "DIR", 0, 0, (node_t **)NULL, (const lmchar_t *)NULL, (const lmchar_t *)NULL, (lmchar_t *)NULL)) == 0)
		Perror("m3l_Mklist");
	
	dim = (lmsize_t *) malloc( 1* sizeof(lmsize_t));
	dim[0] = strlen(answer)+1;
	
	if(  (TmpNode = m3l_Mklist("ANSWER", "C", 1, dim, &Gnode, "/ackn", "./", (lmchar_t *)NULL)) == 0)
		Error("m3l_Mklist");
	TmpNode->data.c = answer;
	
	free(dim);
	
 	return Gnode;
}



node_t *MkTCPIPHeader(opts_t *Popts)
{
	node_t *Tmpnode, *AddNode;
	tmpstruct_t TMPSTR;
	
	opts_t *Poptadd, opts;
/*
 * options for add and remove list
 */
	opts.opt_i = '\0'; opts.opt_d = '\0'; opts.opt_f = '\0'; opts.opt_r = 'r'; opts.opt_I = '\0'; opts.opt_k = '\0'; opts.opt_b = '\0';opts.opt_l = '\0';
/*
 * options for allocate list
 */	
	opts.opt_n = '\0'; opts.opt_b = '\0'; opts.opt_nomalloc = '\0';
	Poptadd = &opts;
/*
 * make header node
 */
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List), "%s", lm3lTCPIPHeader) < 0)
		   Perror("snprintf");
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", "DIR") < 0)
		   Perror("snprintf");
	
	TMPSTR.ndim = 0;
	TMPSTR.dim=NULL;
 /*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
	if( (Tmpnode = m3l_AllocateNode(TMPSTR, Popts)) == NULL){
		Error("Allocate");
		return (node_t *) NULL;
	}
/*
 * add info about coding
 */
	if( snprintf(TMPSTR.Name_Of_List, sizeof(TMPSTR.Name_Of_List),"%s", "TCPIP_encoding") < 0)
		   Perror("snprintf");
	if( snprintf(TMPSTR.Type, sizeof(TMPSTR.Type),"%s", "C") < 0)
		   Perror("snprintf");
	
	TMPSTR.ndim = 1;
	if( (TMPSTR.dim=(lmsize_t *)malloc(TMPSTR.ndim * sizeof(lmsize_t))) == NULL)
		Perror("malloc");
	TMPSTR.dim[0]=2;
 /*
 * two ways of allocating pointer - through reference pointer or as a function returning pointer
 */	
	if( (AddNode = m3l_AllocateNode(TMPSTR, Poptadd)) == NULL){
		Error("Allocate");
		free(TMPSTR.dim);
		if( m3l_rm_list(1, &Tmpnode, Popts) < 0)
			Warning("problem in rm_list");
		return (node_t *) NULL;
	}
	
	free(TMPSTR.dim);
	
	AddNode->data.c[0] = Popts->opt_tcpencoding;
	AddNode->data.c[1] = '\0';
	
	if( m3l_add_list(&AddNode, &Tmpnode,  Poptadd) < 0){
		Warning("problem in add_list");
		if( m3l_rm_list(1, &Tmpnode, Poptadd) < 0)
			Warning("problem in rm_list");
		if( m3l_rm_list(1, &AddNode, Poptadd) < 0)
			Warning("problem in rm_list");
		return (node_t *)NULL;
	}
	
	return Tmpnode;
}