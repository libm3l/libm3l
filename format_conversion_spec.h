#ifndef __FORMAT_CONVERSION_SPEC_H__
#define __FORMAT_CONVERSION_SPEC_H__

#define FCS_W_LD(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%Lf%c", A, B)
#define FCS_W_D(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%lf%c", A, B)
#define FCS_W_F(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%f%c",  A, B)

#define FCS_W_ULLI(A,B) snprintf(buff, MAX_WORD_LENGTH,"%lld%c", A, B)
#define FCS_W_SLLI(A,B) snprintf(buff, MAX_WORD_LENGTH,"%lld%c", A, B)
#define FCS_W_ULI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%ld%c",  A, B)
#define FCS_W_USI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)
#define FCS_W_SI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)
#define FCS_W_UI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)
#define FCS_W_LI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%ld%c",  A, B)
#define FCS_W_I(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)

#define FCS_W_ST(A,B)       snprintf(buff, MAX_WORD_LENGTH,"%ld%c", A, B)
#define FCS_W_PTRDF(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%ld%c", A, B)

#endif
