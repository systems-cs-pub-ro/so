#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "utils.h"

#define NPAGES		(128 * 1024)

static unsigned long get_current_millis(void)
{
	struct timeval tv;
	int rc = gettimeofday(&tv, NULL);

	DIE(rc < 0, "gettimeofday");

	return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

int main(void)
{
	pid_t pid;
	int rc;
	size_t i;
	char c;
	unsigned long millis_start;
	unsigned long millis_end;
	int page_size = getpagesize();
	char *mem = malloc(NPAGES * page_size);

	DIE(!mem, "malloc");

	pid = fork();
	switch (pid)
	{
	case -1:	/* Error */
		DIE(1, "fork");
		break;
	
	case 0:		/* Child process */
		/* Measure the time spent reading. No COW. */
		millis_start = get_current_millis();

		for (i = 0; i < NPAGES; ++i)
			c = mem[i * page_size];

		millis_end = get_current_millis();

		printf("Time for reading %d pages: %lu ms\n", NPAGES,
			millis_end - millis_start);

		/* Measure the time spent writing. COW is performed. */
		millis_start = get_current_millis();

		for (i = 0; i < NPAGES; ++i)
			mem[i * page_size] = c + 1;

		millis_end = get_current_millis();

		printf("Time for writing to %d pages: %lu ms\n", NPAGES,
			millis_end - millis_start);

		break;

	default:	/* Parent process */
		rc = waitpid(pid, NULL, 0);
		DIE(rc < 0, "waitpid");
		break;
	}

	return 0;
}
