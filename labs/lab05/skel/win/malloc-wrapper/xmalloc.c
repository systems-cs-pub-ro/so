/**
 * SO
 * Lab #5, Memory Management
 *
 * Task #1, Windows
 *
 * malloc wrapper
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdlib.h>
#include "xmalloc.h"
#include "utils.h"

void *xmalloc(size_t size)
{
	HANDLE hProcessHeap;
	void *ptr;

	/* TODO */

	return ptr;
}
