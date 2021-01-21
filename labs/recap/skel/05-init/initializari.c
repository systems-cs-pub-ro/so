#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE	(1024 * 1024)

/* init array to 2, 0, 0, 0 ... */
static int data1[ARRAY_SIZE] = {2, };
static void print_time(char *msg)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	printf("%s\t\t\t\tsecunde: %ld; microsecunde: %ld\n", msg, t.tv_sec, t.tv_usec);
}

static void init_array(int *a, size_t len)
{
	size_t i;

	for (i = 0; i < len; i += 1024)
		a[i] = 2010;
}

int main(void)
{
	int *data2 = malloc(ARRAY_SIZE * sizeof(int));
	print_time("before init data1");
	init_array(data1, ARRAY_SIZE);
	print_time("after  init data1");
	print_time("before init data2");
	init_array(data2, ARRAY_SIZE);
	print_time("after  init data2");
	return 0;
}

