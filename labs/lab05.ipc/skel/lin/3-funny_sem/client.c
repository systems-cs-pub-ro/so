/**
  * SO, 2011
  * Lab #5
  *
  * Task #3, lin
  *
  * Client want's to share a magic number
  */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>

#include "utils.h"

#define SEM_NAME	"/my_sem"
#define	MAGIC_NUM	42

int main(void)
{
	sem_t* sem;
	int rc;


	printf("Client is sending magic number %d\n", MAGIC_NUM);

	/* TODO - Use one semaphore to send MAGIC_NUM to server */



	return 0;
}
