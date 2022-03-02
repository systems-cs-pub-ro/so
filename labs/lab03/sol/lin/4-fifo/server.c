/**
 * SO
 * Lab #3
 *
 * Task #4, Linux
 *
 * FIFO server
 */
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "utils.h"
#include "common.h"	/* PIPE_NAME, BUFSIZE */

int main(void)
{
	int fd;
	int bytesRead;
	int rc;
	int offset = 0;
	char buff[BUFSIZE];

	/* TODO - Create named pipe */
	rc = mkfifo(PIPE_NAME, 0644);
	DIE(rc < 0, "mkfifo");

	/* TODO - Open named pipe */
	fd = open(PIPE_NAME, O_RDONLY);
	DIE(fd < 0, "open fifo");

	/* TODO - Read in buff from pipe while not EOF */
	memset(buff, 0, sizeof(buff));
	do {
		bytesRead = read(fd, buff + offset, BUFSIZE);
		DIE(bytesRead < 0, "read");
		offset += bytesRead;
	} while (bytesRead > 0);

	printf("Message received:%s\n", buff);

	/* TODO - Close and delete pipe */
	rc = close(fd);
	DIE(rc < 0, "close");

	rc = unlink(PIPE_NAME);
	DIE(rc < 0, "unlink");

	return 0;
}
