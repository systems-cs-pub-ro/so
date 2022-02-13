/**
  * SO
  * Lab #4
  *
  * Task #5, lin
  *
  * Avoid creating zombies using signals
  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

#define TIMEOUT		20

/*
 * configure signal handler
 */
static void set_signals(void)
{
	struct sigaction sa;
	int rc;

	/* ignore SIGCHLD */
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	rc = sigaction(SIGCHLD, &sa, NULL);
	DIE(rc == -1, "sigaction");
}

int main(void)
{
	pid_t pid;

	set_signals();

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
