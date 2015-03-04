/**
 * SO, 2011
 * Lab #2, Procese
 *
 * Task #2, Linux
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

#define BUFSIZE		32

int main(int argc, char **argv)
{
	int fd_src, fd_dst, rc, bytesRead;
	char buffer[BUFSIZE];

	if (argc < 2 || argc > 3){
		printf("Usage:\n\t%s source_file [destination_file]\n", argv[0]);
		return 0;
	}

	/* TODO 1 - open source file for reading */


	if (argc == 3) {
		/* TODO 2 - redirect stdout to destination file */

	}

	/* TODO 1 - read from file and print to stdout
		use _only_ read and write functions */

	/* TODO 1 - close file */

	return 0;
}
