/**
 * SO
 * Lab #5
 *
 * Task #3, Linux
 *
 * Server tring to find the magic number
 */
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>

#include "utils.h"

#define SEM_NAME "/my_sem"

int main(void)
{
	sem_t *sem;
	int magic_num = 0;
	int rc;

	/* TODO - Use one semaphore to receive magic_num from client */
	sem = sem_open(SEM_NAME, 0);
	DIE(sem == SEM_FAILED, "sem_open");

	rc = sem_getvalue(sem, &magic_num);
	DIE(rc < 0, "sem_getvalue");

	printf("Client sent us this magic number: %d\n", magic_num);

	rc = sem_close(sem);
	DIE(rc < 0, "close");

	rc = sem_unlink(SEM_NAME);
	DIE(rc < 0, "unlink");

	return 0;
}
