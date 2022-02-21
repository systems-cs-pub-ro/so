/**
 * SO
 * Lab #3
 *
 * Task #4, Linux
 *
 * FIFO client
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
	int rc;
	int bytesWritten;
	int len;
	int total = 0;
	char message[BUFSIZE];

	/* TODO - Open named pipe for writing */
	fd = open(PIPE_NAME, O_WRONLY);
	DIE(fd < 0, "open pipe");

	/* Read message from user */
	memset(message, 0, sizeof(message));
	printf("Message to send:");
	scanf("%s", message);
	len = strlen(message);

	/* TODO - Write message to pipe */
	do {
		bytesWritten = write(fd, message + total, len - total);
		DIE(bytesWritten < 0, "write");

		total += bytesWritten;
	} while (total < len);

	/* Close pipe */
	rc = close(fd);
	DIE(rc < 0, "close");

	return 0;
}
