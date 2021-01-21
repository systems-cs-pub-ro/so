/*
 * SO, 2016 - Lab 10 (Advanced I/O Windows
 * I/O completion ports wrapper functions
 * Task #4 (I/O completion ports)
 */

#include <windows.h>

#include "iocp.h"

/*
 * create I/O completion port
 */

HANDLE iocp_init(void)
{
	/* TODO */
}

/*
 * add handle to completion port; use handle as key
 */

HANDLE iocp_add(HANDLE iocp, HANDLE hFile)
{
	/* TODO */
}

/*
 * add handle to completion port; use key as key
 */

HANDLE iocp_add_key(HANDLE iocp, HANDLE hFile, ULONG_PTR key)
{
	/* TODO */
}

/*
 * wait for event notification on completion port
 */

BOOL iocp_wait(HANDLE iocp, PDWORD bytes, PULONG_PTR key, LPOVERLAPPED *op)
{
	/* TODO */
}
