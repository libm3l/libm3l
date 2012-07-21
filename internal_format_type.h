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
 *     Header file internal_format_type.h
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




#ifndef  __TMPSTRUCT_T__
#define  __TMPSTRUCT_T__


#define lm3lTCPIPHeader "-libm3l_TCPIP_Header-"              /* end of buffer TCP/IP */


typedef struct tmpstruct{
	 char Type[MAX_TYPE_LENGTH], Name_Of_List[MAX_TYPE_LENGTH]; 
 	size_t ndim, *dim;
}tmpstruct_t;

typedef struct opts{
	char opt_a, opt_A;
	char opt_b, opt_B;	
	char opt_c, opt_C;
	char opt_d, opt_D;
	char opt_e, opt_E;
	char opt_f, opt_F;
	char opt_i, opt_I;
	char opt_k, opt_K;
	char opt_l, opt_L;	
	char opt_m, opt_M;
	char opt_n, opt_N;
	char opt_p, opt_P;
	char opt_r, opt_R;
	char opt_s, opt_S;
	char opt_u, opt_U;

	char opt_linkscleanemptrefs, opt_linkscleanemptlinks;
	char opt_nomalloc;
	char opt_tcpencoding;
	char opt_tcpheader;

} opts_t;


/*
 * parsed_path
 */
typedef struct path{
	char **path;
	char abspath;
	size_t seg_count;
} path_t;

/*
 * get_arguments
 */
typedef struct get_arg{
	char first;  			/* S,s or '\0' if S or s, indicates that the argument specifies sub-set */
	char arg;    			/* value of argument V(value), T(Type), .... */
	char args[MAX_NAME_LENGTH];	/* argument - compared according to arg */
	char s_name[MAX_NAME_LENGTH];   /* if sub-set - specifies name of the subset */
	int retval;
} get_arg_t;


#endif