/**
 * SO, 2017
 * Lab #6
 *
 * Task #2, lin
 *
 * write vs mmap
 */
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "utils.h"

#define N 100000

int main(void)
{
	char msg[] = "Testing testing 123...\n";
	int rc, i;
	int fd;

	fd = open("test_write", O_CREAT | O_TRUNC | O_RDWR, 0644);
	DIE(fd == -1, "open");

	for (i = 0; i < N; i++) {
		rc = write(fd, msg, sizeof(msg));
		DIE(rc < 0, "write");
	}

	rc = close(fd);
	DIE(rc == -1, "close");

	return 0;
}
