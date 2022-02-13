/**
  * SO
  * Lab #4
  *
  * Task #5, lin
  *
  * Creating zombies
  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

#define TIMEOUT		20

int main(void)
{
	pid_t pid;

	pid = fork();
	switch (pid) {
	case -1:
		DIE(pid, "fork");
		break;

	/* child process */
	case 0:
		exit(EXIT_SUCCESS);
		break;

	/* parent process */
	default:
		break;
	}

	sleep(TIMEOUT);

	return 0;
}
