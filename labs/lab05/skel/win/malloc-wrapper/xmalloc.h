/**
 * SO
 * Lab #5, Memory Management
 *
 * Task #1, Windows
 *
 * malloc wrapper
 */

#ifndef XMALLOC_H__
#define XMALLOC_H__ 1

#include <windows.h>
#include <stdlib.h>

void *xmalloc(size_t size);

/* TODO */

#define xfree(ptr)			\
	do {				\
		BOOL bRet;		\
		HANDLE hProcessHeap;	\
	} while (0)

#endif
