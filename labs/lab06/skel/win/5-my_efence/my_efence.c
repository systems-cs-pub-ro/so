/**
 * SO, 2014
 * Lab #6, Memoria virtuala
 *
 * Task #5, Windows
 *
 * Locking memory in order to avoid swaping
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <windows.h>

#include "utils.h"


static LPVOID MyMalloc(DWORD dwSize)
{
	SYSTEM_INFO siSysInfo;
	DWORD dwPageSize, dwNoPages, dwOffest, rc, oldProt;
	char *lpMem = NULL;

	GetSystemInfo(&siSysInfo);
	dwPageSize = siSysInfo.dwPageSize;

	/* Number of pages used to allocate a buffer of size dwSize */
	dwNoPages = dwSize / dwPageSize + 1;

	/* TODO - alocate dwNoPages + 1 pages*/


	/* TODO - change accces for the last page (guard page)
	 * to PAGE_NOACCESS
	 */


	/* TODO - return a pointer to an offest in the allocated memory
	 * so that memory area has dwSize bytes and ends before the guard page
	 */

	return NULL;
}


int main(void)
{
	int i;

	int *v = malloc(10 * sizeof(int));
	//int *v = MyMalloc(10 * sizeof(int));

	for (i = 0; i <= 10; i++) {
		__try {
			// Write to memory.
			v[i] = i;
		}
		__except(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
			EXCEPTION_EXECUTE_HANDLER :
			EXCEPTION_CONTINUE_SEARCH) {
			printf("i = %d - Exiting process\n", i);
			ExitProcess(GetLastError());
		}
	}

	for (i = 0; i <= 10; i++)
		printf("%d\n", v[i]);

	return 0;
}
