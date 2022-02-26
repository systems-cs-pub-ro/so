/*
 * SO
 * Lab 10 - Advanced I/O Windows
 *
 * I/O completion ports wrapper functions
 * Task #4 (I/O completion ports)
 */
#include <windows.h>

#include "iocp.h"

/* Create I/O completion port */
HANDLE iocp_init(void)
{
	return CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		(ULONG_PTR)NULL,
		0);
}

/* Add handle to completion port; use handle as key */
HANDLE iocp_add(HANDLE iocp, HANDLE hFile)
{
	return CreateIoCompletionPort(
		hFile,
		iocp,
		(ULONG_PTR)hFile,
		0);
}

/* Add handle to completion port; use key as key */
HANDLE iocp_add_key(HANDLE iocp, HANDLE hFile, ULONG_PTR key)
{
	return CreateIoCompletionPort(
		hFile,
		iocp,
		key,
		0);
}

/* Wait for event notification on completion port */
BOOL iocp_wait(HANDLE iocp, PDWORD bytes, PULONG_PTR key, LPOVERLAPPED *op)
{
	return GetQueuedCompletionStatus(
		iocp,
		bytes,
		key,
		op,
		INFINITE);
}
