/**
 * SO
 * Lab #5, Memory Management
 *
 * Task #8, Linux
 *
 * Endianess
 */
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

int main(void)
{
	int i;
	unsigned int n = 0xDEADBEEF;
	unsigned char *w = (unsigned char *)&n;

	/* TODO - use w to show all bytes of n in order */
	for (i = 0; i < 4; i++)
		printf("%x ", w[i]);

	printf("\n");

	return 0;
}
