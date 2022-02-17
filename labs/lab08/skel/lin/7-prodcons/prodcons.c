/**
 * SO
 * Lab #8
 *
 * Task #7, Linux
 *
 */
#include <pthread.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>

#define BUFFER_SIZE	3
#define NR_ITERATIONS	10
#define RAND_DELAY	-1

typedef struct {
	int buff[BUFFER_SIZE];
	int count;
} buffer_t;

void init_buffer(buffer_t *b)
{
	memset(b->buff, 0, sizeof(int) * BUFFER_SIZE);
	b->count = 0;
}

void insert_item(buffer_t *b, int item)
{
	b->buff[b->count] = item;
	b->count++;
}

int remove_item(buffer_t *b)
{
	b->count--;
	return b->buff[b->count];
}

int is_buffer_full(buffer_t *b)
{
	return b->count == BUFFER_SIZE;
}

int is_buffer_empty(buffer_t *b)
{
	return b->count == 0;
}

/* the buffer where the producer will place items
 * and from which the consumer will take items
 */
buffer_t common_area;

pthread_cond_t buffer_not_full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_not_empty = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer_fn(void *arg)
{
	int item_to_insert;
	int i;
	int rc;
	int delay = *(int *) arg;

	for (i = 0; i < NR_ITERATIONS; i++) {
		item_to_insert = i;

		/* TODO - lock mutex */

		/* TODO - if common area is full
		 *		then wait until the common area is not full
		 */

		/* TODO - insert item into common area */

		printf("Inserted item %d\n", item_to_insert);

		/* TODO - if we have one element in common area
		 *		then signal that the buffer is not empty
		 */

		/* TODO - unlock mutex */

		if (delay == RAND_DELAY)
			sleep(rand() % 3);
		else
			sleep(delay);
	}

	return NULL;
}

void *consumer_fn(void *arg)
{
	int item_consumed;
	int i;
	int rc;
	int delay = *(int *) arg;

	for (i = 0; i < NR_ITERATIONS; i++) {
		/* TODO - lock mutex */

		/* TODO - if common area is empty
		 *		then wait until the common area is not empty
		 */

		/* TODO - remove item from common area */

		printf("\t\tConsumed item %d\n", item_consumed);

		/* TODO - if we have BUFFER_SIZE - 1 elements in common area
		 *		then signal that the buffer is not full
		 */

		/* TODO - unlock mutex */

		if (delay == RAND_DELAY)
			sleep(rand() % 3);
		else
			sleep(delay);
	}

	return NULL;
}

void run_threads(int producer_delay, int consumer_delay)
{
	int rc;
	pthread_t producer_th;
	pthread_t consumer_th;

	/* initialization */
	init_buffer(&common_area);

	/* create the threads */
	rc = pthread_create(&producer_th, NULL, producer_fn, &producer_delay);
	DIE(rc != 0, "pthread_create");
	rc = pthread_create(&consumer_th, NULL, consumer_fn, &consumer_delay);
	DIE(rc != 0, "pthread_create");

	/* wait for the threads to finish execution */
	rc = pthread_join(producer_th, NULL);
	DIE(rc != 0, "pthread_join");
	rc = pthread_join(consumer_th, NULL);
	DIE(rc != 0, "pthread_join");
}

int main(void)
{
	srand(time(NULL));

	/* run fast producer and slow consumer */
	printf("fast producer - slow consumer:\n");
	run_threads(0, 1);
	/* run slow producer and fast consumer */
	printf("slow producer - fast consumer:\n");
	run_threads(1, 0);
	/* run rand producer and rand consumer */
	printf("rand producer - rand consumer:\n");
	run_threads(RAND_DELAY, RAND_DELAY);

	return 0;
}
