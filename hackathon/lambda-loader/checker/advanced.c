#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void run()
{
	printf("hello from advanced\n");
}

void clumsy() {
	int *seg = NULL;
	int fault = 42;

	printf("How do pointers work?\n");

	*seg = fault;
}

void going_to_sleep() {
	printf("I don't feel so good, I'm going to sleep for good.\n");
	exit(1);
}

void sleepy(void)
{
	printf("Will go to sleep\n");
	usleep(500000);
	printf("I love the smell of napalm in the morning\n");
}
