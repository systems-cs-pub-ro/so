/**
  * SO, 2011
  * Lab #5
  *
  * Task #1, lin
  *
  * Using named semaphores
  */
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

static int wait_for_input(const char *msg)
{
	char buf[32];

	printf("-- %s --\n", msg);
	fflush(stdout);
	fgets(buf, 32, stdin);
	if (strncasecmp(buf, "e", 1) == 0) {
		return 0;
	}
	return 1;
}

int main(void)
{
	sem_t *sem;
	int rc;

	sem = sem_open("/my_sem", O_CREAT, 0644, 0);
	DIE(sem == SEM_FAILED, "sem_open");

	for (;;) {
		rc = sem_post(sem);
		DIE(rc < 0, "sem_post");

		if (!wait_for_input("Press any key to continue (E/e exit)"))
			break;
	}

	rc = sem_close(sem);
	DIE(rc < 0, "sem_close");

	rc = sem_unlink("/my_sem");
	DIE(rc < 0, "sem_unlink");

	return 0;
}
