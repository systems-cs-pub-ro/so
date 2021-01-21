/**
  * SO, 2016
  * Lab #4
  *
  * Task #6, lin
  *
  * Timers usage
  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <signal.h>
#include <sys/time.h>

#include "utils.h"

#define TIMEOUT		2


/*
 * SIGALRM handler - called when the timer expires
 */
static void timer_handler(int signo)
{
	time_t curr_time;

	/* TODO - print time */
}

/*
 * set signal handlers
 */
static void set_signals(void)
{
	struct sigaction sa;
	int rc;

	memset(&sa, 0, sizeof(sa));

	/* TODO - set handler for SIGALRM */
}

/*
 * configure timer to generate SIGALRM when fired
 * the timer expires at every TIMEOUT seconds
 */
static void set_timer(void)
{
	timer_t timerid;
	struct sigevent sev;
	struct itimerspec its;
	int rc;

	/* TODO - Create the timer */

	/* TODO - Start the timer */
}

/*
 * use sigsuspend for waiting signals
 */
static void wait_for_signal(void)
{
	sigset_t old_mask;
	int rc;

	/* TODO - wait for signal using sigsuspend */
}

int main(void)
{
	set_signals();	/* configure handler for  SIGALRM */
	set_timer();   /* create and start timer */

	while (1)
		wait_for_signal();

	return 0;
}
