/**
  * SO, 2017
  * Lab #3
  *
  * Task #4, lin
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
	int fd, bytesRead, rc, offset = 0;
	char buff[BUFSIZE];

	/* TODO - create named pipe */

	/* TODO - open named pipe */

	/* TODO - read in buff from pipe while not EOF */
	memset(buff, 0, sizeof(buff));

	printf("Message received:%s\n", buff);

	/* Close and delete pipe */
	rc = close(fd);
	DIE(rc < 0, "close");

	rc = unlink(PIPE_NAME);
	DIE(rc < 0, "unlink");

	return 0;
}
