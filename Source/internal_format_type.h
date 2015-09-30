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

#ifdef __cplusplus 
#define CPP_C "C"
#else
#define CPP_C
#endif


typedef struct tmpstruct{
	 lmchar_t Type[MAX_TYPE_LENGTH], Name_Of_List[MAX_TYPE_LENGTH]; 
 	lmsize_t ndim, *dim;
}tmpstruct_t;

typedef struct opts{
	lmchar_t opt_a, opt_A;
	lmchar_t opt_b, opt_B;	
	lmchar_t opt_c, opt_C;
	lmchar_t opt_d, opt_D;
	lmchar_t opt_e, opt_E;
	lmchar_t opt_f, opt_F;
	lmchar_t opt_i, opt_I;
	lmchar_t opt_k, opt_K;
	lmchar_t opt_l, opt_L;	
	lmchar_t opt_m, opt_M;
	lmchar_t opt_n, opt_N;
	lmchar_t opt_p, opt_P;
	lmchar_t opt_r, opt_R;
	lmchar_t opt_s, opt_S;
	lmchar_t opt_u, opt_U;

	lmchar_t opt_linkscleanemptrefs, opt_linkscleanemptlinks;
	lmchar_t opt_nomalloc;
	lmchar_t opt_tcpencoding;
	lmchar_t opt_tcpheader;
	lmchar_t opt_shutdown;
	lmchar_t opt_add;
	lmchar_t opt_MEMCP;
	lmchar_t opt_EOBseq;
	lmchar_t opt_REOBseq;
} opts_t;


/*
 * parsed_path
 */
typedef struct path{
	lmchar_t **path;     /*    array of chars, paths is segmented by / sign and each segment is saved in path[] */
	lmchar_t abspath;  /*     if A path is absolute (ie. all the way up to the master head node), if R, path is relative (ie. starting from node which was passed to Locate) */
	lmsize_t seg_count;  /* numer of segments of path */
} path_t;

/*
 * get_arguments
 */
typedef struct get_arg{
	lmchar_t first;  			/* S,s or '\0' if S or s, indicates that the argument specifies sub-set */
	lmchar_t arg;    			/* value of argument V(value), T(Type), .... */
	lmchar_t args[MAX_NAME_LENGTH];	/* argument - compared according to arg */
	lmchar_t s_name[MAX_NAME_LENGTH];   /* if sub-set - specifies name of the subset */
	lmint_t retval;
} get_arg_t;


#endif