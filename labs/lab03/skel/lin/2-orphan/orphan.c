/**
  * SO, 2017
  * Lab #3
  *
  * Task #2, lin
  *
  * What happens if a parent process does not wait for its child process?
  */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>	/* fork */
#include <sys/wait.h>	/* wait */

#include "utils.h"

int main(void)
{
	pid_t child_pid = fork();

	switch (child_pid) {
	case -1:
		DIE(child_pid, "fork");
	case 0:
		/* child process */
		sleep(20);
		break;
	default:
		sleep(5);
		/* parent process does not wait for the child process */
		break;
	}

	return 0;
}
