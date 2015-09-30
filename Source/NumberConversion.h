/*
 *     Copyright (C) 
 */


/*
 *     Header file NumberConversion.h
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



#ifndef __NUMBERCONVERSION_H__
#define __NUMBERCONVERSION_H__

#define pack754_32(f) (pack754((f), 32, 8))
#define pack754_64(f) (pack754((f), 64, 11))
#define pack754_128(f) (pack754((f), 128, 15))

#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))
#define unpack754_128(i) (unpack754((i), 128, 15))

#ifdef __cplusplus 
#define CPP_C "C"
#else
#define CPP_C
#endif

// #define to_uint64(buffer,n) (*(uint64_t*)(buffer + n))

// uint64_t pack754(long double , unsigned , unsigned );
// long double unpack754(uint64_t , unsigned , unsigned );
// char * double_2_Hex(double);

extern CPP_C  Uint64_t pack754(lmlongdouble_t f, unsigned bits, unsigned expbits);
extern CPP_C  lmlongdouble_t unpack754(Uint64_t i, unsigned bits, unsigned expbits);
extern CPP_C  lmchar_t * double_2_Hex(lmdouble_t a);
extern CPP_C  lmlint_t hex2dec(lmchar_t *);


#endif
