/**
 * SO
 * Lab #2, Simple I/O operations
 *
 * Task #3, Linux
 *
 * cat/cp applications
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"
#include "xfile.h"

#define BUFSIZE 32

int main(int argc, char **argv)
{
	int fd; 
	int fd2;
	int rc;
	int bytesRead;
	char buffer[BUFSIZE];

	if (argc < 1 || argc > 3) {
		printf("Usage:\n ./cat source_file [destination_file]\n");
		return 0;
	}

	/* TODO 1 - Open source file for reading */
	fd = open(argv[1], O_RDONLY);
	DIE(fd < 0, "open source");

	if (argc == 3) {
		/* TODO 2 - Redirect stdout to destination file */

		fd2 = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, 0644);
		DIE(fd2 < 0, "open destination");

		rc = dup2(fd2, STDOUT_FILENO);
		DIE(rc < 0, "dup2");

		rc = close(fd2);
		DIE(rc < 0, "close fd2");
	}

	/* TODO 1 - Read from file and print to stdout
	 * use _only_ read and write functions
	 */
	while ((bytesRead = xread(fd, buffer, BUFSIZE)) != 0) {
		DIE(bytesRead < 0, "read failed");

		buffer[bytesRead] = '\0';

		rc = xwrite(STDOUT_FILENO, buffer, bytesRead);
		/* write(STDOUT_FILENO, buffer, bytesRead); */

		DIE(rc < 0, "write failed");
	}

	/* TODO 1 - Close file */
	rc = close(fd);
	DIE(rc < 0, "close fd");

	return 0;
}
