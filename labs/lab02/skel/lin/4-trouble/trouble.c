/**
 * SO, 2017
 * Lab #2, Operatii I/O simple
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
	int fd, rc;
	char *msg = "Ana are mere!\n";

	/* opening files */
	fd = open("tmp1.txt", O_CREAT | O_WRONLY);
	DIE(fd < 0, "open failed");

	rc = write(fd, msg, strlen(msg));
	DIE(rc < 0, "write failed");

	rc = close(fd);
	DIE(rc < 0, "close failed");

	return 0;
}

