/**
 * SO, 2017
 * Lab #2, Operatii I/O simple
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

#define BUFSIZE		32

int main(int argc, char **argv)
{
	int fd_src, fd_dst, rc, bytesRead;
	char buffer[BUFSIZE];

	if (argc < 2 || argc > 3) {
		printf("Usage:\n\t%s source_file [destination_file]\n",
			argv[0]);
		return 0;
	}

	/* TODO 1 - open source file for reading */


	if (argc == 3) {
		/* TODO 2 - redirect stdout to destination file */

	}

	/* TODO 1 - read from file and print to stdout
	 * use _only_ read and write functions
     * for writing to output use write(STDOUT_FILENO, buffer, bytesRead);
	 */

    /* TODO 3 - Change the I/O strategy and implement xread/xwrite. These
     * functions attempt to read _exactly_ the size provided as parameter.
     */

	/* TODO 1 - close file */

	return 0;
}
