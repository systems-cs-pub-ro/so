/**
 * SO
 * Lab #1, Introduction
 *
 * Task #2, Windows
 *
 * Multiple source files compiling
 */

#include <stdio.h>

#include "add.h"

#define A_NUMBER	2
#define B_NUMBER	(A_NUMBER + 8)

int main(void)
{
	int res;

	Dprintf("Before add function call\n");

	res = add(A_NUMBER, B_NUMBER);

	Dprintf("After add function call\n");
	return 0;
}
