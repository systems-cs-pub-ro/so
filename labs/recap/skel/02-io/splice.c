/**
 * SO
 * Recap
 * Task #2
 *
 * IO operations
 * Pagecache, truncate
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define MB	(1024 * 1024)
#define SIZE	(512 * MB)

int main(void)
{
	int fd = open("out", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	ssize_t len;
	size_t copied;

	if (fd < 0) {
		perror("open");
		exit(-1);
	}

	copied = 0;
	while (SIZE - copied) {
		len = splice(STDIN_FILENO, NULL, fd, NULL, SIZE - copied, 0);
		if (len < 0) {
			perror("splice");
			exit(-1);
		}
		copied += len;
	}

	close(fd);
	printf("%lu bytes copied\n", copied);

	return 0;
}
