#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NR_PAGES 1024 * 128
#define TEST_ACCESS 1


int main() {
	unsigned long  page_size;
	char *addr;
	int i;

	page_size = getpagesize();
	addr = malloc(NR_PAGES * page_size);

#ifdef TEST_ACCESS
	for (i = 0; i < NR_PAGES; i++) {
		addr[i + page_size] = 'c';
	}
#endif

	return 0;
}
