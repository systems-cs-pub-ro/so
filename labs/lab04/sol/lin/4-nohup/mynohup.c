/**
  * SO
  * Lab #4
  *
  * Task #4, lin
  *
  * Simulate nohup command
  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"

#define NOHUP_OUT_FILE		"nohup.out"

/* configure handlers */
static void set_signals(void)
{
	struct sigaction sa;
	int rc;

	/* ignore SIGHUP */
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;

	rc = sigaction(SIGHUP, &sa, NULL);
	DIE(rc == -1, "sigaction");
}

/* execute a new program */
static void exec_func(int argc, char **argv)
{
	char **exec_args;
	int i, rc;

	/* ignore SIGHUP */
	set_signals();

	/* if stdout is a terminal, redirect output to NOHUP_OUT_FILE */
	if (isatty(STDOUT_FILENO)) {
		int fd;

		fd = open(NOHUP_OUT_FILE, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		DIE(fd == -1, "open");

		rc = dup2(fd, STDOUT_FILENO);
		DIE(rc == -1, "dup2");

		rc = close(fd);
		DIE(rc == -1, "close");
	}

	/* exec a new process */
	exec_args = malloc(argc * sizeof(*exec_args));
	DIE(exec_args == NULL, "malloc");

	for (i = 0; i < argc-1; i++)
		exec_args[i] = argv[i+1];
	exec_args[argc-1] = NULL;

	execvp(exec_args[0], exec_args);
	DIE(1, "execvp");
}

int main(int argc, char **argv)
{
	if (argc <= 1) {
		fprintf(stderr, "Usage: %s command_and_arguments\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	exec_func(argc, argv);

	return 0;
}
