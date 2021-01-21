/**
 * SO, 2010 - Lab #2, Simple I/O
 * Task #4, Linux
 *
 * Standard output/error redirection
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
	
	
int main(void)
{
	int fd;

	/*...*/
	
	fputs("alfa", stderr);
	fputs("beta", stdout);

	return 0;
}

	
