/**
 * SO
 * Lab #9
 *
 * Task #5, Windows
 *
 * Timer Queue
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#define _WIN32_WINNT    0x500
#include <windows.h>
#include <stdio.h>

#include "utils.h"

#define PERIOD       1000
#define TIMES        3

HANDLE finished;

VOID CALLBACK TimerFunction(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	static int count;
	BOOL bRet;

	printf("'TimerFunction' has been called and count is %d\n", count);

	/* TODO - Check if we must increment counter or finish */

}

int main(void)
{
	HANDLE timer_queue;
	HANDLE timer;
	BOOL bRet;
	DWORD dwRet;

	setbuf(stdout, NULL);

	/* TODO - Create a TimerQueue */

	/* TODO - Create a semaphore/event */

	/* TODO - Create a timer and associate it with the timer queue */

	/* TODO - Wait for the semaphore/event to be set so we can free resources */

	/* TODO - Delete the timer queue and its timers */

	return 0;
}
