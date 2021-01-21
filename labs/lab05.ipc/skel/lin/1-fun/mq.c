/**
  * SO, 2011
  * Lab #5
  *
  * Task #1, lin
  *
  * Using a message queue
  */
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
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
	mqd_t mq;
	int rc;

	mq = mq_open("/my_mqueue", O_CREAT | O_RDWR, 0644, NULL);
	DIE(mq == (mqd_t)-1, "mq_open");

	for (;;) {
		rc = mq_send(mq, "item", 5, 1);
		DIE(rc < 0, "mq_send");

		if (!wait_for_input("Press any key to continue (E/e exit)"))
			break;
	}

	rc = mq_close(mq);
	DIE(rc < 0, "mq_close");

	rc = mq_unlink("/my_mqueue");
	DIE(rc < 0, "mq_unlink");

	return 0;
}
