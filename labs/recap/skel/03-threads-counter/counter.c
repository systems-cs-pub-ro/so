/**
 * SO
 * Recap
 * Task #3
 *
 * Threads counter - analyze what happends if we use fork() into the thread code.
 * The usage of this program is:
 *	./counter nthreads should_fork, with default values 100 and 1
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>

#include "utils.h"

static sem_t sem;
static char buffer[128];
static int should_fork = 1;

static void *thread_code(void *args)
{
	pid_t pid;
	int status;
	int rc;

	if (!args && should_fork) {
		switch((pid = fork())) {
		case -1:
			DIE(1, "fork");

		case 0:
			system("./counter 10 0");
			sleep(5);
			exit(-1);

		default:
			waitpid(pid, &status, 0);
		}
	}

	rc = sem_wait(&sem);
	DIE(rc < 0, "sem_wait");
	return NULL;
}

static int get_threads_number(char *cmd)
{
	int pipes[2];
	pid_t pid;
	int status;
	int rc;

	rc = pipe(pipes);
	DIE(rc, "pipe");

	switch((pid = fork())) {
	case -1:
		DIE(1, "fork");

	case 0:		/* Child process */
		/* Write to pipe. */
		rc = close(pipes[0]);
		DIE(rc < 0, "close");

		rc = dup2(pipes[1], STDOUT_FILENO);
		DIE(rc < 0, "close");

		const char *argv[] = {"/bin/bash", "-c", cmd, NULL};
		execv("/bin/bash", (char *const *)argv);
		DIE(1, "execv");

	default:	/* Parent process */
		/* Wait for child process to end. */
		rc = close(pipes[1]);
		DIE(rc < 0, "close");

		rc = waitpid(pid, &status, 0);
		DIE(rc < 0, "waitpid");

		/* Read data from pipe. */
		memset(buffer, 0, 128);

		rc = read(pipes[0], buffer, 128);
		DIE(rc < 0, "read");

		rc = close(pipes[0]);
		DIE(rc < 0, "close");

		break;
	}

	/* Subtract 3 because the result contains . .. and total x */
	return atoi(buffer) - 3;
}

int main(int argc, char **argv)
{
	int nthreads;
	int rc;
	size_t i;
	size_t N = 100;
	pthread_t *threads;

	if (argc > 1)
		N = atoi(argv[1]);

	if (argc > 2)
		should_fork = atoi(argv[2]);

	rc = sem_init(&sem, 0, 0);
	DIE(rc < 0, "sem_init");

	threads = malloc(N * sizeof(*threads));
	DIE(!threads, "malloc");

	for (i = 0 ; i < N ; i++) {
		rc = pthread_create(&threads[i], NULL, thread_code, (void*)i);
		DIE(rc, "pthread_create");
	}

	snprintf(buffer, 128, "ls -la /proc/%d/task | wc -l", (int)getpid());

	nthreads = get_threads_number(buffer);
	printf("Number of tasks is %d\n", nthreads);

	for (i = 0 ; i < N ; i++) {
		rc = sem_post(&sem);
		DIE(rc < 0, "sem_post");
	}

	sleep(3);

	for (i = 0 ; i < N ; i++) {
		rc = pthread_join(threads[i], NULL);
		DIE(rc, "pthread_join");
	}

	free(threads);

	rc = sem_destroy(&sem);
	DIE(rc, "sem_destroy");

	return 0;
}
