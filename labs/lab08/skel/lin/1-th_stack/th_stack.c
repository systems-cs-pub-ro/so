/**
 * SO, 2019
 * Lab #8
 *
 * Task #1, lin
 *
 * Process address space when creating threads
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <utils.h>
#define N	8

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int a;

static void wait_for_input(const char *msg)
{
	char buf[32];

	printf(" * %s\n", msg);
	printf(" -- Press ENTER to continue ...\n"); fflush(stdout);
	fgets(buf, 32, stdin);
}


void *thread1_function(void *args)
{
	int thread_no = *(int *)args;

	printf(" thread %d starting...\n", thread_no);

	/* do stuff and never die */
	while (1)
		;

	return NULL;
}

int main(void)
{

	pthread_t th[N];
	int rc = 1, i, th_id[N];

	for (i = 0; i < N; i++) {
		wait_for_input("creating new thread");

		th_id[i] = i;
		rc = pthread_create(&th[i], NULL, thread1_function, &th_id[i]);
		DIE(rc != 0, "pthread_create");
	}

	for (i = 0; i < N; i++) {
		rc = pthread_join(th[i], NULL);
		DIE(rc != 0, "pthread_join");
	}

	return 0;
}
