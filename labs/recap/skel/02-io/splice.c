/**
 * SO, 2010 - Lab #13, Recap
 * Task #2, Linux
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
#define SIZE	(128 * MB)

int
main(void)
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
		if ((len = splice(STDIN_FILENO, NULL, fd, NULL,
						SIZE - copied, 0)) < 0) {
			perror("splice");
			exit(-1);
		}
		copied += len;
	}
	close(fd);
	printf("%lu bytes copyed\n", copied);
	return 0;
}
