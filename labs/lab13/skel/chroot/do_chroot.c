#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "utils.h"

#define CHROOT_FOLDER	"/home/student/chroot/"
#define CHROOTED_FILE	"/etc/hosts"

int main(void)
{
	pid_t pid;
	int fd;
	int rc;
	int status;

	pid = fork();
	switch (pid) {
	case -1:
		perror("fork");
		exit(EXIT_FAILURE);
		break;

	case 0:
		/* child process chroots */
		rc = chroot(CHROOT_FOLDER);
		DIE(rc < 0, "chroot");

		fd = open(CHROOTED_FILE, O_RDONLY);
		DIE(fd < 0, "open");

		exit(EXIT_SUCCESS);
		break;
	
	default:
		/* parent process */
		break;
	}

	fd = open(CHROOTED_FILE, O_RDONLY);
	DIE(fd < 0, "open");

	rc = wait(&status);
	DIE(rc < 0, "wait");

	return 0;
}
