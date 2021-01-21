/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>

#include "utils.h"

#define NO_THREADS 50

/* function executed by the threads */
DWORD WINAPI ThreadFunc(LPVOID lpParameter)
{
	printf("thread %d starting\n", lpParameter);

	/* do some work */
	while (1) {
		/* do some work */
		Sleep(1000);
	}

	return 0;
}

DWORD main(VOID)
{
	DWORD IdThread, dwReturn;
	HANDLE hThread[NO_THREADS];
	int i;

	/* create threads */
	for (i = 0; i < NO_THREADS; i++) {
		hThread[i] = CreateThread(
				NULL, /* default security attributes */
				0,    /* default stack size */
				(LPTHREAD_START_ROUTINE) ThreadFunc,
				(LPVOID) i, /* no thread parameter */
				0,          /* immediately run the thread */
				&IdThread); /* thread id */
		DIE(hThread[i] == NULL, "CreateThread");
	}

	/* wait for threads completion */
	for (i = 0; i < NO_THREADS; i++) {
		dwReturn = WaitForSingleObject(hThread[i], INFINITE);
		DIE(dwReturn == WAIT_FAILED, "WaitForSingleObject");
	}

	return 0;
}
