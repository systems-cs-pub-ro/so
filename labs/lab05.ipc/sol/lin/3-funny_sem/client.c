/**
 * SO
 * Lab #5
 *
 * Task #3, Linux
 *
 * Client want's to share a magic number
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>

#include "utils.h"

#define SEM_NAME "/my_sem"
#define	MAGIC_NUM 42

int main(void)
{
	sem_t *sem;
	int rc;

	printf("Client is sending magic number %d\n", MAGIC_NUM);

	sem = sem_open(SEM_NAME, O_CREAT, 0644, MAGIC_NUM);
	DIE(sem == SEM_FAILED, "sem_open");

	rc = sem_close(sem);
	DIE(rc < 0, "close");

	return 0;
}
