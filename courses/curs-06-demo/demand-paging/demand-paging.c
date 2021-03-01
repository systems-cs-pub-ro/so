/*
 * See how demand paging works.
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

#define CHUNK_SIZE	(1024*1024)
#define PAGE_SIZE	getpagesize()
#define NUM_PAGES	(CHUNK_SIZE / PAGE_SIZE)

#define NUM_ROUNDS	10

int main(void)
{
	char *p[NUM_ROUNDS];	/* array of start addresses */
	int cnt, i;

	puts("Initializing ...");
	sleep(5);

	for (cnt = 0; cnt < NUM_ROUNDS; cnt++) {
		printf("Using mmap to allocate %dMB (%d chunks of %d bytes).\n",
				CHUNK_SIZE, NUM_PAGES, PAGE_SIZE);
		p[cnt] = mmap(NULL, CHUNK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		DIE(p == MAP_FAILED, "malloc");
		sleep(2);
	}

	puts("Going for demand paging ...");
	for (cnt = 0; cnt < NUM_ROUNDS; cnt++) {
		printf("Going through all pages in chunk %d.\n", cnt);
		for (i = 0; i < NUM_PAGES; i++)
			p[cnt][i*PAGE_SIZE] = 'a';
		sleep(2);
	}

	return 0;
}
