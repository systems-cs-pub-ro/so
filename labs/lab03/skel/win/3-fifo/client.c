/**
 * SO, 2017
 * Lab #3, Procese
 *
 * Task #3, Windows
 *
 * Named Pipes - Client side
 */

/* do not use UNICODE */
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

	/* TODO - Open Named Pipe as a regular file */

	/* Read mesage from user */
	printf("Message to send:");
	ZeroMemory(message, sizeof(message));
	scanf_s("%s", message, sizeof(message));
	dwMsgLen = strlen((const char *)message);

	/* TODO - Send the message */

	/* Closing our end of the pipe */
	bRet = FlushFileBuffers(hPipe);
	DIE(bRet == FALSE, "FlushFileBuffers");

	bRet = CloseHandle(hPipe);
	DIE(bRet == FALSE, "CloseHandle");

	return 0;
}
