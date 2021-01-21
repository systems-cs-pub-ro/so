/**
 * SO, 2014
 * Lab #6, Memoria virtuala
 *
 * Task #4, Windows
 *
 * Changing access right to pages
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

//#define _WIN32_WINNT 0x0500

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <windows.h>

#include <utils.h>

static LPVOID access_violation_handler;
static int pageSize = 0x1000;
static LPBYTE p;
static int how[3] = { PAGE_NOACCESS, PAGE_READONLY, PAGE_READWRITE };

/*
 * SIGSEGV handler
 */
static LONG CALLBACK access_violation(PEXCEPTION_POINTERS ExceptionInfo)
{
	DWORD old, rc;
	LPBYTE addr;
	int pageNo;

	/* TODO - get the memory location which caused the page fault */

	/* TODO - get the page number which caused the page fault */


	/* TODO - test if page is one of our own */


	/* TODO - increase protection for that page */


	return EXCEPTION_CONTINUE_EXECUTION;
}

/*
 * sets SIGSEGV handler
 */
static void set_signal(void)
{
	/* TODO add VectoredHandler */
}

/*
 * restores SIGSEGV handler
 */
static void restore_signal(void)
{
	/* TODO remove VectoredHandler */
}

int main(void)
{
	BYTE ch;
	DWORD old, rc;

	/* TODO 1 - Map 3 pages with the desired memory protection
	 * Use global 'p' variable to keep the address return by VirtualAlloc
	 * Use VirtualProtect to set memory protection based on global 'how'
	 * array; 'how' array keeps protection level for each page
	 */

	set_signal();

	ch = p[0*pageSize]; p[0*pageSize] = 'a';
	ch = p[1*pageSize]; p[1*pageSize] = 'a';
	ch = p[2*pageSize]; p[2*pageSize] = 'a';

	restore_signal();

	/* TODO 1 - cleanup */

	return 0;
}
