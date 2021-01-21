/**
 * SO, 2016
 * Lab #1, Introduction
 *
 * Task #3, Linux
 *
 * Multiple source files compiling
 */

#include <stdio.h>

#include "add.h"
#include "mul.h"

#define A_NUMBER	2
#define B_NUMBER	A_NUMBER + 8

int main(void)
{
	printf(" ** Ops using defined functions **\n");
	printf("%d + %d = %d\n", A_NUMBER, B_NUMBER, add(A_NUMBER, B_NUMBER));
	printf("%d * %d = %d\n", A_NUMBER, B_NUMBER, mul(A_NUMBER, B_NUMBER));

	printf(" ** Ops using predefined operators **\n");
	printf("%d + %d = %d\n", A_NUMBER, B_NUMBER, A_NUMBER + B_NUMBER);
	printf("%d * %d = %d\n", A_NUMBER, B_NUMBER, A_NUMBER * B_NUMBER);


#ifdef HAVE_MATH
	int x = 2, y = 3;

	printf("%d ^ %d = %.1f\n", x, y, pow(x, y));
#endif
	return 0;
}
