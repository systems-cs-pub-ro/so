/**
 * SO, 2019
 * Lab #8
 *
 * Task #8, lin
 *
 * fork() vs pthread_create()
 */
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>

#include <utils.h>

void *thread_function(void *args)
{
	sleep(2);
	printf("thread here\n");
	return NULL;
}

int main(void)
{
	pid_t pid;
	int status;
	int rc;
	pthread_t a_thread;

	rc = pthread_create(&a_thread, NULL, thread_function, NULL);
	DIE(rc != 0, "pthread_create");

	pid = fork();
	switch (pid) {
	case -1:
		printf("error");
		exit(-1);
	case 0: /* child */

		printf("child here\n");

		sleep(3);
		break;
	default: /* parent */

		printf("parent here\n");

		break;
	}
	wait(&status);

	return 0;
}
