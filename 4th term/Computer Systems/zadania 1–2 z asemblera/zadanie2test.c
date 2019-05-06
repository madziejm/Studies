/*
 * =====================================================================================
 *
 *       Filename:  zadanie2test.c
 *
 *    Description:  Count leading zeros.
 *
 *        Version:  1.0
 *        Created:  24/04/19 13:07:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Maciej M
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>

unsigned leading_zeros(long x)
{
    if(x==0)
	    return 64;
    unsigned n = 0;
    if (x <= 0x00000000ffffffff) n += 32, x <<= 32;
    if (x <= 0x0000ffffffffffff) n += 16, x <<= 16;
    if (x <= 0x00ffffffffffffff) n +=  8, x <<= 8;
    if (x <= 0x0fffffffffffffff) n +=  4, x <<= 4;
    if (x <= 0x3fffffffffffffff) n +=  2, x <<= 2;
    if (x <= 0x7fffffffffffffff) n ++;
    return n;
}


int main()
{
	printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
			leading_zeros(0xFFFFFFFFFFFFFFFF),
			leading_zeros(0xffffffffffffff),
			leading_zeros(0xFFFFFFFF),
			leading_zeros(0x0),
			leading_zeros(0x01),
			leading_zeros(0x0F),
			leading_zeros(0x002)
	      );
	return 0;
}
