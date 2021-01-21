/**
 * SO, 2019
 * Lab #8
 *
 * Task #5, lin
 *
 * Deadlock
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct thd_data_t {
	pthread_mutex_t mutex;
	int found;
	int val;
	int start, stop;
};

/* Thread x data */
struct thd_data_t data_x = {
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.found = 0,
	.val   = -1,
	.start = 10000,
	.stop  = 20000,
};

/* Thread y data */
struct thd_data_t data_y = {
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.found = 0,
	.val   = -1,
	.start = 20000,
	.stop  = 30000,
};

static int factorial(int n)
{
	int i;
	int p = 1;

	for (i = 0; i < n; i++)
		p *= i;
	return p;
}

/**
 * Computational intensive function
 * @return: 0 - n is not a 'magic number'
 */
static int is_super_number(int n)
{
	return (factorial(n) % 123123 == 12345);
}

void *thread_routine_y(void *arg)
{
	int i;
	int finished = 0;

	for (i = data_y.start; i < data_y.stop && !finished; i++) {

		/* Show progress - usefull in case threads block */
		if (i % 100 == 0)
			printf("thread_Y: %d\n", i);

		/* Calling is_super_number() outside critical section.
		 * This way we don't hold the mutex very much
		 */
		int local_found = is_super_number(i);

		pthread_mutex_lock(&data_y.mutex);
		if (local_found) {
			finished = 1;
			data_y.found = 1;
			data_y.val = i;
		} else {
			/* sleep to force deadlock */
			if ((i + 1) % 5000 == 0)
				sleep(1);

			pthread_mutex_lock(&data_x.mutex);
			if (data_x.found)
				finished = 1;
			pthread_mutex_unlock(&data_x.mutex);
		}
		pthread_mutex_unlock(&data_y.mutex);
	}
	return NULL;
}

void *thread_routine_x(void *arg)
{
	int finished = 0;
	int i;

	for (i = data_x.start; i < data_x.stop && !finished; i++) {
		/* Show progress - usefull in case threads block */
		if (i % 100 == 0)
			printf("thread_X: %d\n", i);

		/* Calling is_super_number() outside critical section.
		 * This way we don't hold the mutex very much
		 */
		int local_found = is_super_number(i);

		pthread_mutex_lock(&data_x.mutex);
		if (local_found) {
			finished = 1;
			data_x.found = 1;
			data_x.val = i;
		} else {
			pthread_mutex_lock(&data_y.mutex);
			if (data_y.found)
				finished = 1;
			pthread_mutex_unlock(&data_y.mutex);
		}
		pthread_mutex_unlock(&data_x.mutex);
	}
	return NULL;
}

int main(void)
{
	pthread_t tid_x;
	pthread_t tid_y;

	/* both threads run the 'thread_routine' */
	pthread_create(&tid_x, NULL, thread_routine_x, (void *) 0);
	pthread_create(&tid_y, NULL, thread_routine_y, (void *) 1);

	/* wait for all threads to finish */
	pthread_join(tid_y, NULL);
	pthread_join(tid_x, NULL);

	if (data_x.found)
		printf("Threadul X found the magic value %d\n", data_x.val);
	else if (data_y.found)
		printf("Threadul Y found the magic value %d\n", data_y.val);
	else
		printf("No thread found the magic value\n");

	return 0;
}
