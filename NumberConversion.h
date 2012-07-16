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
#define unpack754_32(i) (unpack754((i), 32, 8))
#define unpack754_64(i) (unpack754((i), 64, 11))

uint64_t pack754(long double , unsigned , unsigned );
long double unpack754(uint64_t , unsigned , unsigned );

#endif
