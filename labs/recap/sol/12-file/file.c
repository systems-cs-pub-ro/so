/**
 * SO
 * Recap
 * Task #12
 *
 * File descriptor manipulation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
	
int main(void)
{
	int fd;

	fd = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	
	fputs("alfa", stderr);
	fputs("beta", stdout);

	return 0;
}
