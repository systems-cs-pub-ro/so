/**
  * SO
  * Lab #3
  *
  * Task #2, Windows
  *
  * FIFO server
  */
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
	
	hPipe = CreateNamedPipe(
				PIPE_NAME,
				PIPE_ACCESS_INBOUND,
				PIPE_TYPE_BYTE | PIPE_WAIT,
				PIPE_UNLIMITED_INSTANCES,
				BUFSIZE,
				BUFSIZE,
				0,
				NULL);
	DIE(hPipe == INVALID_HANDLE_VALUE, "CreateNamedPipe");

	bRet = ConnectNamedPipe(hPipe, NULL);
	DIE(bRet == FALSE, "ConnectNamedPipe");
	
	ZeroMemory(buff, sizeof(buff));

	do {
		bRet = ReadFile(hPipe,         /* handle to pipe */
				buff + dwTotalBytesRead, /* received data */
				sizeof(buff),  /* size of buffer */
				&dwBytesRead,  /* number of bytes read */
				NULL);
		
		if (bRet == FALSE && GetLastError() == ERROR_BROKEN_PIPE) {
			/* client finished */
			break;
		}
		DIE(bRet == FALSE, "ReadFile failed");
		
		dwTotalBytesRead += dwBytesRead;
	} while (dwBytesRead); /* till pipe is closed */

	printf("Message received: %s\n", buff);

	bRet = DisconnectNamedPipe(hPipe);
	DIE(bRet == FALSE, "DisconnectNamedPipe");

	bRet = CloseHandle(hPipe);
	DIE(bRet == FALSE, "CloseHandle");

	return result;
}
