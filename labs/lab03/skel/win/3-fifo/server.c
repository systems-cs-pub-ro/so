/**
 * SO, 2017
 * Lab #3, Procese
 *
 * Task #3, Windows
 *
 * Named Pipes - Server side
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>

#include "utils.h"
#include "common.h"


int main(void)
{
	HANDLE hPipe;
	char buff[BUFSIZE];
	int result = EXIT_SUCCESS;
	BOOL bRet;
	DWORD dwBytesRead, dwTotalBytesRead = 0;

	/* TODO - Create named pipe - assure inbound access */

	/* TODO - connect to pipe */

	/* TODO - read data from pipe */
	ZeroMemory(buff, sizeof(buff));

	printf("Message received: %s\n", buff);

	/* Disconnect and close pipe */
	bRet = DisconnectNamedPipe(hPipe);
	DIE(bRet == FALSE, "DisconnectNamedPipe");

	bRet = CloseHandle(hPipe);
	DIE(bRet == FALSE, "CloseHandle");

	return result;
}
