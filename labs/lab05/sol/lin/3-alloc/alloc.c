/**
 * SO
 * Lab #5, Memory Management
 *
 * Task #3, Linux
 *
 * Valgrind usage
 */
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#define MAX		100
#define CHUNK		10
#define SEED		23
#define STOP		17

int main(void)
{
	int nr, count = 0;
	int *buffer;

	/* alloc buffer with initial size CHUNK */
	buffer = malloc(CHUNK * sizeof(int));
	DIE(buffer == NULL, "malloc");

	srand(SEED);
	do {
		nr = 1 + rand() % MAX;
		buffer[count] = nr;
		count++;

		if (count % CHUNK == 0) {
			printf("Resize buffer to fit %d elements\n", count);

			/* resize buffer to fit CHUNK elements */
			buffer =  malloc((count + CHUNK) * sizeof(int));
			DIE(buffer == NULL, "malloc");
		}

		printf("buffer[%d]=%d\n", count - 1, buffer[count-1]);
	} while (buffer[count - 1] != STOP);

	/* free memory */
	free(buffer);

	return 0;
}
