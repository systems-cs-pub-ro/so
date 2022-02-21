/*
 * SO
 * Lab 10 - Advanced I/O Windows
 *
 * I/O completion ports wrapper functions
 * Task #4 (I/O completion ports)
 */

#ifndef IOCP_H_
#define IOCP_H_ 1

#include <windows.h>

HANDLE iocp_init(void);
HANDLE iocp_add(HANDLE iocp, HANDLE hFile);
HANDLE iocp_add_key(HANDLE iocp, HANDLE hFile, ULONG_PTR key);
BOOL iocp_wait(HANDLE iocp, PDWORD bytes, PULONG_PTR key, LPOVERLAPPED *op);

#endif
