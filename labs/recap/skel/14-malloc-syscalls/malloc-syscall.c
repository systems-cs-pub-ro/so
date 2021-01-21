#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_ALLOC	1000
#define DO_FREE		0

static char *ptr[NUM_ALLOC];

int main(void)
{
	size_t i;
	int page_size = getpagesize();

	for (i = 0; i < 1000; i++)
		ptr[i] = malloc(page_size);

#if DO_FREE == 1
	for (i = 0; i < 1000; i++)
		free(ptr[i]);
#endif

	return 0;
}
