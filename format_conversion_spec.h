#ifndef __FORMAT_CONVERSION_SPEC_H__
#define __FORMAT_CONVERSION_SPEC_H__
/*
 * used in WriteData and Write2Socket
 */
#define FCS_W_LD(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%Lf%c", A, B)
#define FCS_W_D(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%lf%c", A, B)
#define FCS_W_F(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%f%c",  A, B)

#define FCS_W_ULLI(A,B) snprintf(buff, MAX_WORD_LENGTH,"%lld%c", A, B)
#define FCS_W_SLLI(A,B) snprintf(buff, MAX_WORD_LENGTH,"%lld%c", A, B)
#define FCS_W_LLI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%lld%c", A, B)
#define FCS_W_ULI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%ld%c",  A, B)
#define FCS_W_USI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)
#define FCS_W_SI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)
#define FCS_W_UI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)
#define FCS_W_LI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%ld%c",  A, B)
#define FCS_W_I(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)

#define FCS_W_ST(A,B)       snprintf(buff, MAX_WORD_LENGTH,"%ld%c", A, B)
#define FCS_W_PTRDF(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%ld%c", A, B)


/*
 * used in cat_data
 */

#define FCS_C_LD(A)   if( printf("%Lf ", A ) < 0) Perror("printf");
#define FCS_C_D(A)    if( printf("%lf ", A ) < 0) Perror("printf");
#define FCS_C_F(A)    if( printf("%f ",  A ) < 0) Perror("printf");

#define FCS_C_ULLI(A) if( printf("%lld ", A ) < 0) Perror("printf");
#define FCS_C_SLLI(A) if( printf("%lld ", A ) < 0) Perror("printf");
#define FCS_C_LLI(A) if( printf("%lld ", A ) < 0) Perror("printf");
#define FCS_C_ULI(A)  if( printf("%ld ",  A ) < 0) Perror("printf");
#define FCS_C_USI(A)  if( printf("%d ",   A ) < 0) Perror("printf");
#define FCS_C_SI(A)   if( printf("%d ",   A ) < 0) Perror("printf");
#define FCS_C_UI(A)   if( printf("%d ",   A ) < 0) Perror("printf");
#define FCS_C_LI(A)   if( printf("%ld ",  A ) < 0) Perror("printf");
#define FCS_C_I(A)    if( printf("%d ",   A ) < 0) Perror("printf");

#define FCS_C_ST(A)       if( printf("%ld ", A ) < 0) Perror("printf");
#define FCS_C_PTRDF(A)    if( printf("%ld ", A ) < 0) Perror("printf");


/*
 * used in ReadDescriptor and ReadSocket
 */
#define FCS_C2LD(A,B)  strtold(A,B);
#define FCS_C2D(A,B)   strtod(A,B);
#define FCS_C2F(A,B)   strtof(A,B);

#define FCS_C2LLI(A,B)  strtoll(A,B,0);
#define FCS_C2LI(A,B)   strtol(A,B,0);
#define FCS_C2I(A)      atoi(A);

#endif
