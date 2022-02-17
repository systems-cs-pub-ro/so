/**
 * SO
 * Lab #4
 *
 * Task #1, Windows
 *
 * Timers usage.
 */

#define _WIN32_WINNT	0x0500

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <windows.h>

#include "utils.h"

#define _SECOND		10000000
#define TIMEOUT		(-2 * _SECOND)


/**
 * Timer APC routine
 */
static VOID CALLBACK TimerAPCProc(LPVOID lpArgToCompletionRoutine,
		DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	/* TODO: Use ctime and time to print current time. */
	time_t currentTime;

	currentTime = time(NULL);

	printf("time is: %s", ctime(&currentTime));
}

/**
 * Initialise and configure timer
 */
static void InitSetTimer(void)
{
	/**
	 * TODO
	 * First, create your timer using CreateWaitableTimer.
	 * Then set its timeout and its routine using SetWaitableTimer.
	 * No data needs to be passeed to the completion routine.
	 * Don't forget to check those calls for errors.
	 */
	BOOL bRet;
	HANDLE hTimer;
	LARGE_INTEGER dueTime;

	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	DIE(hTimer == NULL, "CreateWaitableTimer");

	dueTime.QuadPart = TIMEOUT;
	bRet = SetWaitableTimer(
		hTimer,
		&dueTime,
		2000,
		TimerAPCProc,
		NULL,
		FALSE);
	DIE(bRet == FALSE, "SetWaitableTimer");
}

int main(void)
{
	InitSetTimer(); /* configures the timer to 2 seconds */

	while (1) {
		/* TODO: Wait for timer (use SleepEx function). */
		SleepEx(INFINITE, TRUE);
	}

	return 0;
}
