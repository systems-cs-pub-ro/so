/**
  * SO
  * Lab #3
  *
  * Task #2, Windows
  *
  * FIFO client
  */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "common.h"


int main(void)
{
	HANDLE hPipe;
	char *message[BUFSIZE];
	DWORD dwBytesWritten, dwTotalBytesWritten = 0, dwMsgLen;
	BOOL bRet;

	/* Open Named Pipe as a regular file */
	hPipe = CreateFile(
		PIPE_NAME,      /* pipe name */
		GENERIC_WRITE,  /* write access */
		FILE_SHARE_READ,
		NULL,           /* default security attributes */
		OPEN_EXISTING,  /* opens existing pipe */
		0,              /* default attributes */
		NULL);
	DIE(hPipe == INVALID_HANDLE_VALUE, "CreateFile");

	/* Read mesage from user */
	printf("Message to send:");
	ZeroMemory(message, sizeof(message));
	scanf_s("%s", message, sizeof(message));
	dwMsgLen = strlen((const char *)message);

	/* Send the message */
	do {
		bRet = WriteFile(
			hPipe,
			message + dwTotalBytesWritten,
			dwMsgLen - dwTotalBytesWritten,
			&dwBytesWritten,
			NULL);
		DIE(bRet == FALSE, "WriteFile");

		dwTotalBytesWritten += dwBytesWritten;
	} while (dwTotalBytesWritten < dwMsgLen);

	bRet = FlushFileBuffers(hPipe);
	DIE(bRet == FALSE, "FlushFileBuffers");

	bRet = CloseHandle(hPipe);
	DIE(bRet == FALSE, "CloseHandle");

	return 0;
}
