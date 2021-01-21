/**
 * SO, 2019
 * Lab #8
 *
 * Task #10, lin
 *
 * Mutex vs Spinlock
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>	/* fork */
#include <sys/wait.h>	/* wait */
#include <pthread.h>

#include "utils.h"

#define NO_THREADS	2
#define MILION		1000000
#define NO_ITER		(200 * MILION)


#ifdef USE_SPINLOCK
	static pthread_spinlock_t lock;
#else
	static pthread_mutex_t lock;
#endif

static pthread_barrier_t barrier;
static int shared;

static inline void acquire_lock(void)
{
#ifdef USE_SPINLOCK
		pthread_spin_lock(&lock);
#else
		pthread_mutex_lock(&lock);
#endif
}

static inline void release_lock(void)
{
#ifdef USE_SPINLOCK
		pthread_spin_unlock(&lock);
#else
		pthread_mutex_unlock(&lock);
#endif
}

static void *thread_func(void *arg)
{
	int i;

	pthread_barrier_wait(&barrier);

	for (i = 0; i < NO_ITER; i++) {
		acquire_lock();
		shared++;
		release_lock();
	}
	return NULL;
}

int main(void)
{
	pthread_t threads[NO_THREADS];
	int i, rc;

#ifdef USE_SPINLOCK
	pthread_spin_init(&lock, PTHREAD_PROCESS_SHARED);
#else
	pthread_mutex_init(&lock, NULL);
#endif
	pthread_barrier_init(&barrier, NULL, NO_THREADS);

	for (i = 0; i < NO_THREADS; i++) {
		rc = pthread_create(&threads[i], NULL, thread_func, &i);
		DIE(rc == -1, "pthread_create");
	}

	for (i = 0; i < NO_THREADS; i++) {
		rc = pthread_join(threads[i], NULL);
		DIE(rc == -1, "pthread_join");
	}

#ifdef USE_SPINLOCK
	printf("Spinlock version - shared = %d\n", shared);
	pthread_spin_destroy(&lock);
#else
	printf("Mutex version - shared = %d\n", shared);
	pthread_mutex_destroy(&lock);
#endif
	pthread_barrier_destroy(&barrier);

	return 0;
}

