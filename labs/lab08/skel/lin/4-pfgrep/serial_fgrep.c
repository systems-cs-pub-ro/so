/**
 * SO, 2019
 * Lab #8
 *
 * Task #4, lin
 *
 * Serial implementation of  'fgrep -o <string> <file> | wc -l'
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "utils.h"

struct file_mapping {
	char *mem;
	size_t size;
};

static void map_file(const char *filename, struct file_mapping *fmap)
{
	int fd;
	struct stat statbuf;
	int rc;

	fd = open(filename, O_RDONLY);
	DIE(fd == -1, "open");

	rc = fstat(fd, &statbuf);
	DIE(rc == -1, "fstat");

	fmap->size = statbuf.st_size;

	fmap->mem = mmap(0, fmap->size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0);
	DIE(fmap->mem == MAP_FAILED, "mmap");

	close(fd);
}

static size_t count(char *what, const size_t what_size,
		    char *where, const size_t where_size)
{
	unsigned int i = 0, total = 0;

	while (i < where_size - what_size + 1)
		if (memcmp(where + i, what, what_size) == 0) {
			++total;
			i += what_size;
		} else
			++i;

	return total;
}

int main(int argc, char **argv)
{
	struct file_mapping fmap;

	if (argc != 3) {
		printf("Usage: %s <string> <file>\n", argv[0]);
		exit(1);
	}

	map_file(argv[2], &fmap);

	printf("total = %lu\n", count(argv[1], strlen(argv[1]),
		fmap.mem, fmap.size));

	munmap(fmap.mem, fmap.size);

	return 0;
}

