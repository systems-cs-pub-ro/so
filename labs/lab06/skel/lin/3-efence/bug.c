/**
 * SO, 2017
 * Lab #6
 *
 * Task #3, lin
 *
 * Debugging with 'ElectricFence'
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

int main(void)
{
	int i;
	int *v = calloc(10, sizeof(int));
	int *w = calloc(10, sizeof(int));

	for (i = 0; i < 10; i++)
		v[i] = i;

	for (i = 0; i > -10; i--)
		printf("w[%d]=%d\n", i, w[i]);

	free(w);
	free(v);

	return 0;
}
