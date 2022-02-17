/**
 * SO
 * Lab #8
 *
 * Task #4, Linux
 *
 * Parallel implementation of  'fgrep -o <string> <file> | wc -l'
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "utils.h"

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define NUM_THREADS	4

struct file_mapping {
	char *mem;
	size_t size;
};

struct params {
	char *chunk_mem;
	size_t chunk_size;
};

static struct params th_params[NUM_THREADS];

static char *string;
static size_t string_size;

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
	unsigned int i = 0;
	unsigned int total = 0;

	while (i < where_size - what_size + 1)
		if (memcmp(where + i, what, what_size) == 0) {
			++total;
			i += what_size;
		} else
			++i;

	return total;
}

void *thread_code(void *args)
{
	/* get parameters from args */
	struct params *p = (struct params *) args;

	/* how many times string appeared in this thread's chunk */
	size_t *result = malloc(sizeof(*result));

	*result = count(string, string_size, p->chunk_mem, p->chunk_size);

	return result;
}

static void split_file_mapping(struct file_mapping *fmap)
{
	/* split file mapping into chunks for each thread
	 * initialize th_params structures for each thread
	 */
	size_t chunk_size = (fmap->size + NUM_THREADS - 1) / NUM_THREADS;
	int i;

	for (i = 0; i < NUM_THREADS - 1; i++) {
		th_params[i].chunk_mem = fmap->mem + i * chunk_size;
		th_params[i].chunk_size = chunk_size + string_size - 1;
	}

	th_params[i].chunk_mem = fmap->mem + i * chunk_size;
	th_params[i].chunk_size = MIN(chunk_size, fmap->size - i * chunk_size);

	if (string_size - 1 > th_params[i].chunk_size)
		th_params[i - 1].chunk_size =
			chunk_size + th_params[i].chunk_size;
}

int main(int argc, char **argv)
{
	struct file_mapping fmap;
	int i;
	int rc;
	pthread_t th_id[NUM_THREADS];
	void *results[NUM_THREADS];
	size_t total = 0;

	if (argc != 3) {
		printf("Usage: %s <string> <file>\n", argv[0]);
		exit(1);
	}

	string = argv[1];
	string_size = strlen(string);

	map_file(argv[2], &fmap);

	split_file_mapping(&fmap);

	/* TODO - start threads
	 *	- thread i will run 'thread_code' function
	 *		passing th_params[i] as parameter
	 */

	/* TODO - wait for threads to finish and collect results */

	/* TODO - compute final sum */

	printf("total = %lu\n", total);

	for (i = 0; i < NUM_THREADS; ++i)
		free(results[i]);

	munmap(fmap.mem, fmap.size);

	return 0;
}
