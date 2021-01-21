/**
 * SO, 2017
 * Lab #6
 *
 * Task #2, lin
 *
 * write vs mmap
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

#define N 100000

int main(void)
{
	char msg[] = "Testing testing 123...\n";
	int rc, i;
	int fd;
	char *mem;

	fd = open("test_mmap", O_CREAT | O_TRUNC | O_RDWR, 0644);
	DIE(fd == -1, "open");

	rc = ftruncate(fd, N * sizeof(msg));
	DIE(rc == -1, "ftruncate");

	mem = mmap(0, N * sizeof(msg), PROT_READ | PROT_WRITE,
		   MAP_SHARED, fd, 0);
	DIE(mem == MAP_FAILED, "mmap");

	for (i = 0; i < N; i++)
		memcpy(mem + i * sizeof(msg), msg, sizeof(msg));

	rc = munmap(mem, N * sizeof(msg));
	DIE(rc == -1, "munmap");

	rc = close(fd);
	DIE(rc == -1, "close");

	return 0;
}


