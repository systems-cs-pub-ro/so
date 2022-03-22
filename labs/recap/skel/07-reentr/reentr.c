/**
 * SO
 * Recap
 * Task #7
 *
 * Find out which functions are thread safe and which are reentrant.
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* thread1_code(void*);
void* thread2_code(void*);

int func1 (int x)
{
	return x + 1;
}

int func2 (int x)
{
	int var2 = 2;

	return x + var2;
}

int func3 (int x)
{
	static int var3 = 2;

	return x + var3;
}

int func4 (int x)
{
	static int var4 = 2;

	return x + var4++;
}

int var5 = 0;
void func5 (void)
{
	int i;

	for (i = 0; i < 10000; i++)
		var5++;
}

int var6 = 0;
void func6 (void)
{
	int i;

	pthread_mutex_lock(&lock);
	for (i = 0; i < 10000; i++)
		var6++;
	pthread_mutex_unlock(&lock);
}

void* thread1_code(void *args)
{
	//TODO call func*

	return NULL;
}

void* thread2_code(void *args)
{
	//TODO call func*

	return NULL;
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
