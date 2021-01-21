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

	/* TODO - check if we must increment counter or finish */

}

int main(void)
{
	HANDLE timer_queue;
	HANDLE timer;
	BOOL bRet;
	DWORD dwRet;

	/* create a TimerQueue */

	/* create a semaphore/event */

	/* create a timer and associate it with the timer queue */

	/* wait for the semaphore/event to be set, so we can free resources */

	/*  delete the timer queue and its timers */

	return 0;
}
