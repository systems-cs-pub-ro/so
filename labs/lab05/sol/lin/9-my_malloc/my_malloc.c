/**
 * SO
 * Lab #5, Memory Management
 *
 * Task #9, Linux
 *
 * Dumb implementation of malloc that only allocates space
 */
#include <unistd.h>
#include <sys/types.h>

#include "my_malloc.h"

void *my_malloc(size_t size)
{
	void *current = NULL;

	/* TODO - user sbrk to alloc at least size bytes */
	current = sbrk(size);
	if (current == (void *)-1) {
		/* no more memory */
		return NULL;
	}

	/* return the start of the new allocated area */
	return current;
}
