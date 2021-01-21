/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <stdio.h>
#include <windows.h>
#include "utils.h"

#define NO_THREADS 6

#define ERR_A	1
#define ERR_B	2
#define ERR_C	3

DWORD myErrno;

BOOL function_A(VOID);
BOOL function_B(VOID);
BOOL function_C(VOID);

VOID myPerror(char *description)
{
	DWORD err = 0;

	/* TODO - get error id from TLS */

	switch (err) {
	case ERR_A:
		printf("Got 'Somenthing is wrong!' error: %s\n",
				description);
		break;
	case ERR_B:
		printf("Got 'This is not my fault!' error: %s\n",
				description);
		break;
	case ERR_C:
		printf("Got 'Weird error!' error: %s\n",
				description);
		break;
	default:
		printf("Unknowned error\n");
	}
}

/* function executed by the threads */
DWORD WINAPI ThreadFunc(LPVOID lpParameter)
{
	DWORD lpvData = 1;
	DWORD dwReturn;

	DWORD choice = (rand() + GetCurrentThreadId())  % 3;

	switch (choice) {
	case 0:
		dwReturn = function_A();
		if (dwReturn == FALSE)
			myPerror("function A failed\n");
		break;

	case 1:
		dwReturn = function_B();
		if (dwReturn == FALSE)
			myPerror("function B failed\n");
		break;

	case 2:
		dwReturn = function_C();
		if (dwReturn == FALSE)
			myPerror("function C failed\n");
		break;
	}

	return 0;
}

DWORD main(VOID)
{
	DWORD IDThread, dwReturn;
	HANDLE hThread[NO_THREADS];
	int i;

	/* TODO - allocate TLS index */

	/* create threads */
	for (i = 0; i < NO_THREADS; i++) {
		hThread[i] = CreateThread(
				NULL, /* default security attributes */
				0,    /* default stack size */
				(LPTHREAD_START_ROUTINE) ThreadFunc,
				NULL, /* no thread parameter */
				0,    /* immediately run the thread */
				&IDThread); /* thread id */
			DIE(hThread[i] == NULL, "CreateThread");
	}

	/* wait for threads completion */
	for (i = 0; i < NO_THREADS; i++) {
		dwReturn = WaitForSingleObject(hThread[i], INFINITE);
		DIE(dwReturn == WAIT_FAILED, "WaitForSingleObject");
	}

	/* TODO - free TLS index */

	return 0;
}


BOOL function_A(VOID)
{
	DWORD dwRet;

	/* function encounters ERR_A error and sets myErrno */
	dwRet = TlsSetValue(myErrno, (LPVOID)ERR_A);
	DIE(dwRet == FALSE, "TlsSet failed");

	return FALSE;
}

BOOL function_B(VOID)
{
	DWORD dwRet;

	/* function encounters ERR_B error and sets myErrno */
	dwRet = TlsSetValue(myErrno, (LPVOID)ERR_C);
	DIE(dwRet == FALSE, "TlsSet failed");

	return FALSE;
}

BOOL function_C(VOID)
{
	DWORD dwRet;

	/* function encounters ERR_C error and sets myErrno */
	dwRet = TlsSetValue(myErrno, (LPVOID)ERR_B);
	DIE(dwRet == FALSE, "TlsSet failed");

	return FALSE;
}
