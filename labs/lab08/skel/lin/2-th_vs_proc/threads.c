/**
 * SO, 2019
 * Lab #8
 *
 * Task #2, lin
 *
 * Threads vs Processes
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <utils.h>

#define NUM_TASKS	10
#define NUM_JOBS	20

void *do_task(void *arg)
{
	int thread_no = *(int *) arg;
	int i;

	printf("Thread %d doing task\n", thread_no);

	/* do some work */
	for (i = 0; i < NUM_JOBS; i++)
		sleep(1); /* lazy :-) */
	return NULL;
}

void *do_bad_task(void *arg)
{
	int thread_no = *(int *) arg;

	printf("Thread %d doing BAD task\n", thread_no);

	exit(-1);
}

int main(void)
{
	pthread_t thread[NUM_TASKS];
	int th_id[NUM_TASKS];
	int i, rc;

	for (i = 0; i < NUM_TASKS; i++) {
		th_id[i] = i;

		/* Create a thread and assign do_task as its starting function */
		/* TODO - call do_bad_task() for each 4th thread */
		rc = pthread_create(&thread[i], NULL, do_task, &th_id[i]);
		DIE(rc == -1, "pthread_create");
	}

	/* wait for all */
	for (i = 0; i < NUM_TASKS; i++) {
		rc = pthread_join(thread[i], NULL);
		DIE(rc == -1, "pthread_join");
	}

	return 0;
}
