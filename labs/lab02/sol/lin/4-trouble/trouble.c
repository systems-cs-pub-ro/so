/**
 * SO
 * Lab #2, Simple I/O operations
 *
 * Task #4, Linux
 *
 * Troubleshooting open and write functions
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

int main(void)
{
	int fd;
	int rc;
	size_t bytes_written = 0;
	size_t count;
	char *msg = "Ana are mere!\n";

	/* Open files */
	fd = open("tmp1.txt", O_CREAT | O_RDWR, 0644);
	DIE(fd < 0, "open tmp1.txt");

	count = strlen(msg);
	while (bytes_written < count) {
		ssize_t bytes_written_now = write(fd, msg + bytes_written,
										  count - bytes_written);

		if (bytes_written_now <= 0) /* I/O error */
			return -1;

		bytes_written += bytes_written_now;
	}

	rc = close(fd);
	DIE(rc < 0, "close fd2");

	return 0;
}
