/*
 * Copyright
 * 
 * the pack754 and unpack754 functions are transcript of Brian Hall code from the Beej's Guide to Network Programming
 * http://beej.us/guide/bgnet/output/html/multipage/advanced.html
 * 
 * Copyright - see extract from the Beej's Guide to Network Programming web page: ( http://beej.us/guide/bgnet/output/html/multipage/intro.html#copyright )
 *
 * ---------------------------------------------------------------------------------------------------------------------------------
 * 1.9. Copyright and Distribution
 *
 * Beej's Guide to Network Programming is Copyright © 2012 Brian "Beej Jorgensen" Hall.
 * 
 * With specific exceptions for source code and translations, below, this work is licensed under the Creative Commons Attribution- Noncommercial- No 
 * Derivative Works 3.0 License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/ or send a letter to Creative Commons,
 * 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 * 
 * One specific exception to the "No Derivative Works" portion of the license is as follows: this guide may be freely translated into any language, provided the
 * translation is accurate, and the guide is reprinted in its entirety. The same license restrictions apply to the translation as to the original guide. The 
 * translation may also include the name and contact information for the translator.
 * 
 * The C source code presented in this document is hereby granted to the public domain, and is completely free of any license restriction.
 * 
 * Educators are freely encouraged to recommend or supply copies of this guide to their students.
 * 
 * Contact beej@beej.us for more information.
 * ---------------------------------------------------------------------------------------------------------------------------------
 */

#include "Header.h"
#include "NumberConversion.h"


uint64_t pack754(long double f, unsigned bits, unsigned expbits)
{
	long double fnorm;
	int shift;
	long long sign, exp, significand;
	unsigned significandbits = bits - expbits - 1; // mantisa = total - exponent -1 for sign
/*
 * if number is zero return 0
 */
	if (f == 0.0) return 0;
/*
 * check sign and begin normalization
 */
	if (f < 0) { 
		sign = 1; fnorm = -f; 
	}
	else { 
		sign = 0; fnorm = f; 
	}
/*
 * get the normalized form of f and track the exponent
 */
	shift = 0;
	while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
	while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
	fnorm = fnorm - 1.0;
/*
 * calculate the binary form (non-float) of the significand data
 */
	significand = fnorm * ((1LL<<significandbits) + 0.5f);
/*
 * get the biased exponent
 */
	exp = shift + ((1<<(expbits-1)) - 1); // shift + bias
/*
 * return the final answer
 */
	return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
}

long double unpack754(uint64_t i, unsigned bits, unsigned expbits)
{
	long double result;
	long long shift;
	unsigned bias;
	unsigned significandbits = bits - expbits - 1; // mantisa = total - exponent -1 for sign
/*
 * if number is zero return 0
 */
	if (i == 0) return 0.0;
/*
 * pull the significand
 *	mask
 *	convert back to float
 *	add the one back on
 */
	result = (i&((1LL<<significandbits)-1));
	result /= (1LL<<significandbits);
	result += 1.0f;
/*
 * deal with the exponent
 */
	bias = (1<<(expbits-1)) - 1;
	shift = ((i>>significandbits)&((1LL<<expbits)-1)) - bias;
	while(shift > 0) { result *= 2.0; shift--; }
	while(shift < 0) { result /= 2.0; shift++; }
/*
 * sign it
 */
	result *= (i>>(bits-1))&1? -1.0: 1.0;
	
	return result;
}
