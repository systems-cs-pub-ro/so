#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	int i = 0;
		
	for (i = 0; i < 10; i++) { 
		getpid();
		write(1, "A", 1);
	}

	return 0;
}
