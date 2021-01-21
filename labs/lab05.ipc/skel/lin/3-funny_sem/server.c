/**
  * SO, 2011
  * Lab #5
  *
  * Task #3, lin
  *
  * Server tring to find the magic number
  */
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>

#include "utils.h"

#define SEM_NAME	"/my_sem"

int main(void)
{
	sem_t *sem;
	int magic_num, rc;

	/* TODO - Use one semaphore to receive magic_num from client */

	printf("Client sent us this magic number: %d\n", magic_num);

	return 0;
}


