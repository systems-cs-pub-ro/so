/**
 * SO
 * Lab #5, Memory Management
 *
 * Task #1, Linux
 *
 * Process memory zones
 */
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/**
 * TODO - inc() function must behave like a counter
 *      The function returns the next value in order and
 *      it does not receive paramters
 *
 *	You are not allowed to use global variables
 */
int inc(void)
{
	static int counter = 1;

	counter++;
	return counter;
}

int main(void)
{
	int i;

	for (i = 0; i < 10; i++)
		printf("%d\n", inc());

	return 0;
}
