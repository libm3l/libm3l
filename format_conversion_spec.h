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
 *     Header file format_conversion_spec.h
 *
 *     Date: 2012-05-18
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

#ifndef __FORMAT_CONVERSION_SPEC_H__
#define __FORMAT_CONVERSION_SPEC_H__



#if ABSTRACT_DECLAR_UINT==1
/*
 * used in WriteData and Write2Socket
 */
	#define FCS_W_LD(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%.16Lf%c", A, B)
	#define FCS_W_D(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%.16lf%c", A, B)
	#define FCS_W_F(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%.8f%c",  A, B)

	#define FCS_W_ULLI(A,B) snprintf(buff, MAX_WORD_LENGTH,"%"PRId64 "%c", A, B)
	#define FCS_W_SLLI(A,B) snprintf(buff, MAX_WORD_LENGTH,"%"PRId64 "%c", A, B)
	#define FCS_W_LLI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%"PRId64 "%c", A, B)
	#define FCS_W_ULI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%"PRIu32 "%c",  A, B)
	#define FCS_W_USI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%"PRIu16 "%c",   A, B)
	#define FCS_W_SI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%"PRId16 "%c",   A, B)
	#define FCS_W_UI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%"PRIu32 "%c",   A, B)
	#define FCS_W_LI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%"PRId64 "%c",  A, B)
	#define FCS_W_I(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%"PRId32 "%c",   A, B)

	#define FCS_W_ST(A,B)       snprintf(buff, MAX_WORD_LENGTH,"%" PRId64 "%c", A, B)
	#define FCS_W_PTRDF(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%" PRId64 "%c", A, B)

/*
 * used in cat_data
 */

	#define FCS_C_LD(A)   if( printf("%.16Lf ", A ) < 0) Perror("printf");
	#define FCS_C_D(A)    if( printf("%.16lf ", A ) < 0) Perror("printf");
	#define FCS_C_F(A)    if( printf("%.8f ",  A ) < 0) Perror("printf");

	#define FCS_C_ULLI(A) if( printf("% "PRId64, A ) < 0) Perror("printf");
	#define FCS_C_SLLI(A) if( printf("% "PRId64, A ) < 0) Perror("printf");
	#define FCS_C_LLI(A)  if( printf("% "PRId64, A ) < 0) Perror("printf");
	#define FCS_C_ULI(A)  if( printf("%"PRIu32, A ) < 0) Perror("printf");
	#define FCS_C_USI(A)  if( printf("%"PRIu16, A ) < 0) Perror("printf");
	#define FCS_C_SI(A)   if( printf("% "PRId16, A ) < 0) Perror("printf");
	#define FCS_C_UI(A)   if( printf("%"PRIu32, A ) < 0) Perror("printf");
	#define FCS_C_LI(A)   if( printf("% "PRId64, A ) < 0) Perror("printf");
	#define FCS_C_I(A)    if( printf("% "PRId32, A ) < 0) Perror("printf");

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
#else
/*
 * used in WriteData and Write2Socket
 */
	#define FCS_W_LD(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%.16Lf%c", A, B)
	#define FCS_W_D(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%.16lf%c", A, B)
	#define FCS_W_F(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%.8f%c",  A, B)

	#define FCS_W_ULLI(A,B) snprintf(buff, MAX_WORD_LENGTH,"%lld%c", A, B)
	#define FCS_W_SLLI(A,B) snprintf(buff, MAX_WORD_LENGTH,"%lld%c", A, B)
	#define FCS_W_LLI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%lld%c", A, B)
	#define FCS_W_ULI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%lu%c",  A, B)
	#define FCS_W_USI(A,B)  snprintf(buff, MAX_WORD_LENGTH,"%u%c",   A, B)
	#define FCS_W_SI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)
	#define FCS_W_UI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%u%c",   A, B)
	#define FCS_W_LI(A,B)   snprintf(buff, MAX_WORD_LENGTH,"%ld%c",  A, B)
	#define FCS_W_I(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%d%c",   A, B)

	#define FCS_W_ST(A,B)       snprintf(buff, MAX_WORD_LENGTH,"%ld%c", A, B)
	#define FCS_W_PTRDF(A,B)    snprintf(buff, MAX_WORD_LENGTH,"%ld%c", A, B)

/*
 * used in cat_data
 */

	#define FCS_C_LD(A)   if( printf("%.16Lf ", A ) < 0) Perror("printf");
	#define FCS_C_D(A)    if( printf("%.16lf ", A ) < 0) Perror("printf");
	#define FCS_C_F(A)    if( printf("%.8f ",  A ) < 0) Perror("printf");

	#define FCS_C_ULLI(A) if( printf("%lld ", A ) < 0) Perror("printf");
	#define FCS_C_SLLI(A) if( printf("%lld ", A ) < 0) Perror("printf");
	#define FCS_C_LLI(A) if( printf("%lld ", A ) < 0) Perror("printf");
	#define FCS_C_ULI(A)  if( printf("%lu ",  A ) < 0) Perror("printf");
	#define FCS_C_USI(A)  if( printf("%u ",   A ) < 0) Perror("printf");
	#define FCS_C_SI(A)   if( printf("%d ",   A ) < 0) Perror("printf");
	#define FCS_C_UI(A)   if( printf("%u ",   A ) < 0) Perror("printf");
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




#endif
