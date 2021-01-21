#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
volatile int sched1 = 1;
volatile int sched2 = 0;

void* thread1_code(void*);
void* thread2_code(void*);

void* thread1_code(void *args)
{
	int i;

	while (1) {
		pthread_mutex_lock(&lock);
		while (!sched1) {
			pthread_cond_wait(&cond, &lock);
		}
		pthread_mutex_unlock(&lock);

		printf("thread 1 running\n");

		for (i = 0; i < 100000000000ULL; i++);

		sched1 = 0;
		pthread_mutex_lock(&lock);
		sched2 = 1;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock);

	}
}

void* thread2_code(void *args)
{
	int i;

	while (1) {
		pthread_mutex_lock(&lock);
		while (!sched2) {
			pthread_cond_wait(&cond, &lock);
		}
		pthread_mutex_unlock(&lock);

		printf("thread 2 running\n");

		for (i = 0; i < 100000000000ULL; i++);

		sched2 = 0;
		pthread_mutex_lock(&lock);
		sched1 = 1;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock);
	}
}

int main(int argc, char **argv)
{
	pthread_t t1, t2;

	pthread_create(&t1, NULL, thread1_code, NULL);
	pthread_create(&t2, NULL, thread2_code, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}
