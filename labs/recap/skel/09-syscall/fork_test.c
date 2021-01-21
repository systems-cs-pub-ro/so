#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

#define SYS_NATIVE 0
#define SYS_GLIBC  1


#define SYS_TYPE SYS_GLIBC
int main(void)
{
	printf("pid: %d\n", getpid());
#if SYS_TYPE == SYS_NATIVE
	fork();
#endif

#if SYS_TYPE == SYS_GLIBC
	syscall(SYS_fork); 
#endif
	printf("pid: %d\n", getpid());

	return 0;
}
