/*
 * View virtual memory allocation
 */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "utils.h"

#define NUM_ROUNDS	10

int main(void)
{
	char *p;
	size_t cnt, i;

	puts("Initializing.");
	sleep(5);

	for (cnt = 0; cnt < NUM_ROUNDS; cnt++) {
		puts("Using malloc to allocate 1024 sets of 1024 bytes.");
		for (i = 0; i < 1024; i++) {
			p = malloc(1024);
			DIE(p == NULL, "malloc");
		}
		sleep(2);
	}

	for (cnt = 0; cnt < NUM_ROUNDS; cnt++) {
		puts("Using mmap to allocate 1MB.");
		p = mmap(NULL, 1024*1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		DIE(p == MAP_FAILED, "mmap");
		sleep(2);
	}

	return 0;
}
