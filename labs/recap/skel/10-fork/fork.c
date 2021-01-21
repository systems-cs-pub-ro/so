#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(void)
{
	int ii = 0;
	
	if (fork() >  0) ii++;
	wait(NULL);

	if (fork() == 0) ii++;
	wait(NULL);

	if (fork() < 0) ii++;
	wait(NULL);
	printf("Result %d %p\n", ii, &ii);

	return 0;
}


	 
