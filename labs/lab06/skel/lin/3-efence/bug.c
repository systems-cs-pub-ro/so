/**
 * SO
 * Lab #6, Virtual Memory
 *
 * Task #3, Linux
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
	int *v = calloc(10, sizeof(*v));
	int *w = calloc(10, sizeof(*w));

	for (i = 0; i < 10; i++)
		v[i] = i;

	for (i = 0; i > -10; i--)
		printf("w[%d]=%d\n", i, w[i]);

	free(w);
	free(v);

	return 0;
}
