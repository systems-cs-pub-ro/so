/**
 * SO
 * Recap
 * Task #10
 *
 * What is the process tree created by the program below?
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	int ii = 0;

	if (fork() >  0)
		ii++;
	wait(NULL);

	if (fork() == 0)
		ii++;
	wait(NULL);

	if (fork() < 0)
		ii++;
	wait(NULL);

	printf("Result %d %p\n", ii, &ii);

	return 0;
}
