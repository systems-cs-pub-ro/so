#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "utils.h"

int main(void)
{
	pid_t pid;
	int ret;

	pid = fork();
	switch (pid) {
	case -1:	/* error */
		DIE(pid == -1, "fork");

	case 0:		/* child process */
		printf("  [child] Terminate in 5 seconds. No zombie processes yet\n");
		sleep(5);
		exit(EXIT_SUCCESS);
		break;

	default:	/* parent process */
		break;
	}

	/* Sleep to sync with child process then wait for it. */
	sleep(5);
	printf("  [parent] Child is dead. Check zombie processes.\n");

	sleep(10);
	ret = waitpid(pid, NULL, 0);
	DIE(ret < 0, "waitpid");
	printf("  [parent] Waited for child. Zombie process is gone.\n");
	sleep(5);

	return 0;
}
