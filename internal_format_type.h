/*
 * Header file with declarations
 * copyright � 2012 Adam Jirasek
 */

#ifndef  __TMPSTRUCT_T__
#define  __TMPSTRUCT_T__

typedef struct tmpstruct{
	 char Type[MAX_TYPE_LENGTH], Name_Of_List[MAX_TYPE_LENGTH]; 
 	size_t ndim, *dim;
}tmpstruct_t;
#endif


#ifndef  __OPT_T__
#define  __OPT_T__

typedef struct opts{
	char opt_a, opt_A;
	char opt_d, opt_D;
	char opt_f, opt_F;
	char opt_i, opt_I;
	char opt_l, opt_L;
	char opt_p, opt_P;
	char opt_r, opt_R;
	char opt_s, opt_S;
	char opt_u, opt_U;
} opts_t;

#endif