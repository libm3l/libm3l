
#include "Header.h"
#include "format_type.h"
#include "HexOP.h"

/*
 * converts double to HEX
 */
char * double_2_Hex(double a)
{
//    char *buf = new char[17]; // double is 8-byte long, so we have 2*8 + terminating \0

	char buf[17];
	char *d2c;
	char *n = buf;
	int i;

	d2c = (char *) &a;
	
	for(i = 0; i < 8; i++){
		sprintf(n, "%02X", *d2c++);
		n += 2;
	} 
	*(n) = '\0';
}