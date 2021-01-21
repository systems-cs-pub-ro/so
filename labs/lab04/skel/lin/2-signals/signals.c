/**
  * SO, 2016
  * Lab #4
  *
  * Task #2, lin
  *
  * Catching signals
  */
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "utils.h"

int normal_count;
int real_count;
int done;

static void sig_handler(int signum)
{
	switch (signum) {
	case SIGINT:
		usleep(100000);
		normal_count++;
		break;

	case 34:
		usleep(100000);
		real_count++;
		break;

	case SIGQUIT:
		done = 1;
		break;
	}
}

int main(void)
{
	struct sigaction signals;
	sigset_t mask;
	int rc;

	sigfillset(&mask);

	memset(&signals, 0, sizeof(struct sigaction));
	signals.sa_mask = mask;

	signals.sa_handler = sig_handler;
	rc = sigaction(SIGINT, &signals, NULL);
	DIE(rc == -1, "sigaction");

	rc = sigaction(SIGQUIT, &signals, NULL);
	DIE(rc == -1, "sigaction");

	rc = sigaction(SIGRTMIN, &signals, NULL);
	DIE(rc == -1, "sigaction");


	while (!done)
		sleep(1);

	printf("Got %d normal signals\n", normal_count);
	printf("Got %d real-time signals\n", real_count);

	return 0;
}
