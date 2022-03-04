/*
 * SO
 * Lab 10 - Advanced I/O Windows
 * Task #1
 */
#include "utils.h"
#include <windows.h>
#include <stdlib.h>

#define BUF_SIZE (64 * 1024) /* 64KB */

int main(int argc, char **argv)
{
	OVERLAPPED ov;
	HANDLE hFile;
	HANDLE hEvent;
	DWORD dwRet;
	DWORD dwErr;
	DWORD dwBytesRead;
	char *buffer = malloc(BUF_SIZE * sizeof(*buffer));
	int doWork = 1024;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s input_file\n", argv[0]);
		return -1;
	}

	/* Make sure overlapped structure is clean */
	ZeroMemory(&ov, sizeof(ov));
	memset(buffer, 0, BUF_SIZE);

	/* Create manual-reset event */
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	DIE(hEvent == INVALID_HANDLE_VALUE, "CreateEvent");

	ov.hEvent = hEvent;

	hFile = CreateFile(argv[1],
		GENERIC_READ,		/* Access mode */
		FILE_SHARE_READ,	/* Sharing option */
		NULL,			/* Security attributes */
		OPEN_EXISTING,		/* Open only if it exists */
		FILE_FLAG_OVERLAPPED,	/* File attributes */
		NULL);			/* No template */
	DIE(hFile == INVALID_HANDLE_VALUE, "CreateFile");

	dwRet = ReadFile(hFile, buffer, BUF_SIZE, &dwBytesRead, &ov);
	if (dwRet == FALSE) {
		dwErr = GetLastError();

		switch (dwErr) {
		case ERROR_HANDLE_EOF:
			/* Reached end of file */
			printf("End of File Reached\n");
			break;

		case ERROR_IO_PENDING:
			/* Async io not ready */
			printf("Async IO did not finished immediately\n");

			/* Do some other work  in the meantime*/
			printf("Waiting for IO to finish\n");
			while (doWork--)
				;

			/* Wait for it to finish */
			dwRet = GetOverlappedResult(ov.hEvent, &ov,
				&dwBytesRead, TRUE);
			printf("nRead = %d\n", dwBytesRead);
			break;

		default:
			/* ReadFile failed */
			PrintLastError("ReadFile");
			break;
		}
	} else {
		/**
		 * Operation finished right away
		 * No need to use GetOverlappedResult
		 */
		printf("Async IO finished immediately\n");
		printf("nRead = %d\n", dwBytesRead);
	}

	dwRet = CloseHandle(hFile);
	DIE(dwRet == FALSE, "CloseHandle");

	dwRet = CloseHandle(hEvent);
	DIE(dwRet == FALSE, "CloseHandle");

	return 0;
}
