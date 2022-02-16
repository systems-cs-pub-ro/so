/**
 * SO
 * Lab #3
 *
 * Task #5, Linux
 *
 * Use your magic to make this program print 'Hello World'
 */
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	if (fork())
		printf(" Hello ");
	else
		printf(" World ");

	return 0;
}
