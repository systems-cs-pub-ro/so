/**
 * SO
 * Lab #5, Memory Management
 *
 * Task #9, Linux
 *
 * Dump malloc implementation that only allocates space
 */
#include <unistd.h>
#include <sys/types.h>

#include "my_malloc.h"

void *my_malloc(size_t size)
{
	void *current = NULL;

	/* TODO - user sbrk to alloc at least size bytes */

	/* return the start of the new allocated area */
	return current;
}
