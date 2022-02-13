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

#define xfree(ptr)						\
	do {							\
		BOOL bRet;					\
		HANDLE hProcessHeap = GetProcessHeap();         \
		DIE(hProcessHeap == NULL, "GetProcessHeap");	\
								\
		bRet = HeapFree(hProcessHeap, 0, ptr);		\
		DIE(bRet == FALSE, "HeapFree");			\
								\
		ptr = NULL;					\
	} while (0)

#endif
