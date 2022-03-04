/**
 * SO
 * Lab #6, Virtual Memory
 *
 * Task #4, Windows
 *
 * Changing access right to pages
 */

/* Do not use UNICODE */
#undef _UNICODE
#undef UNICODE

/* #define _WIN32_WINNT 0x0500 */

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <windows.h>

#include <utils.h>

static LPVOID access_violation_handler;
static int pageSize = 0x1000;
static LPBYTE p;
static int how[3] = {PAGE_NOACCESS, PAGE_READONLY, PAGE_READWRITE};

/**
 * SIGSEGV handler
 */
static LONG CALLBACK access_violation(PEXCEPTION_POINTERS ExceptionInfo)
{
	DWORD old, rc;
	LPBYTE addr;
	int pageNo;

	/* TODO 2 - Get the memory location which caused the page fault */
	addr = (LPBYTE)ExceptionInfo->ExceptionRecord->ExceptionInformation[1];

	/* TODO 2 - Get the page number which caused the page fault */
	pageNo = (int)(addr - p) / pageSize;

	printf("Exception on page %d\n", pageNo);

	/* TODO 2 - Test if page is one of our own */
	if (!(pageNo >= 0 && pageNo < 3)) {
		printf("wrong page number %d\n", pageNo);
		return EXCEPTION_CONTINUE_SEARCH;
	}

	/* TODO 2 - Increase protection for that page */
	if (how[pageNo] == PAGE_NOACCESS) {
		how[pageNo] = PAGE_READONLY;

		rc = VirtualProtect(p + pageNo * pageSize, pageSize,
							PAGE_READONLY, &old);
		DIE(rc == FALSE, "VirtualProtect");

		printf("giving PAGE_READONLY to page %d\n", pageNo);
	} else {
		how[pageNo] = PAGE_READWRITE;

		rc = VirtualProtect(p + pageNo * pageSize, pageSize,
							PAGE_READWRITE, &old);
		DIE(rc == FALSE, "VirtualProtect");

		printf("giving PAGE_READWRITE to page %d\n", pageNo);
	}

	return EXCEPTION_CONTINUE_EXECUTION;
}

/**
 * Sets SIGSEGV handler
 */
static void set_signal(void)
{
	access_violation_handler = AddVectoredExceptionHandler(1,
														   access_violation);
	DIE(access_violation_handler == NULL, "AddVctoredExceptionHandler");
}

/**
 * Restores SIGSEGV handler
 */
static void restore_signal(void)
{
	int rc = RemoveVectoredExceptionHandler(access_violation_handler);

	DIE(rc == FALSE, "RemoveVectoredExceptionHandler");
}

int main(void)
{
	BYTE ch;
	DWORD old;
	DWORD rc;

	p = VirtualAlloc(NULL, 3 * pageSize, MEM_COMMIT, PAGE_NOACCESS);
	DIE(p == NULL, "VirtualAlloc");

	rc = VirtualProtect(p + 0 * pageSize, pageSize, PAGE_NOACCESS, &old);
	DIE(rc == FALSE, "VirtualProtect");

	rc = VirtualProtect(p + 1 * pageSize, pageSize, PAGE_READONLY, &old);
	DIE(rc == FALSE, "VirtualProtect");

	rc = VirtualProtect(p + 2 * pageSize, pageSize, PAGE_READWRITE, &old);
	DIE(rc == FALSE, "VirtualProtect");

	set_signal();

	ch = p[0 * pageSize];
	p[0 * pageSize] = 'a';
	ch = p[1 * pageSize];
	p[1 * pageSize] = 'a';
	ch = p[2 * pageSize];
	p[2 * pageSize] = 'a';

	restore_signal();

	rc = VirtualFree(p, 3 * pageSize, MEM_DECOMMIT);
	DIE(rc == FALSE, "VirtualFree");

	return 0;
}
