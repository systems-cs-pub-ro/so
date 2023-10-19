#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utils.h"

#define NUM_ROUNDS		100
#define NUM_PROCESSES		100

static void show(void)
{
	printf("This is process %d\n", getpid());
}

int main(void)
{
	int status;
	size_t i, j;

	for (i = 0; i < NUM_ROUNDS; i++) {
		for (j = 0; j < NUM_PROCESSES; j++) {
			pid_t pid = fork();
			switch (pid) {
			case -1:
				DIE(pid, "fork");
			case 0:
				/* child process */
				show();
				exit(EXIT_SUCCESS);
				break;
			default:
				/* parent process */
				break;
			}
		}

		for (j = 0; j < NUM_PROCESSES; j++)
			wait(&status);
	}

	return 0;
}
