/**
 * SO
 * Lab #2, Simple I/O operations
 *
 * Task #4, Windows
 *
 * Troubleshoot
 */

/* Do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>

#include <utils.h>

#define BUFSIZE 32

int main(int argc, char *argv[])
{
	HANDLE hFile;
	CHAR buf[BUFSIZE];
	BOOL bRet;
	DWORD bytesWritten;

	/**
	 * File name should not contain *
	 * File attributes should be GENERIC_WRITE
	 */
	hFile = CreateFile("so_rulz.txt",
					   GENERIC_WRITE,
					   FILE_SHARE_WRITE,
					   NULL,
					   OPEN_ALWAYS,
					   FILE_ATTRIBUTE_NORMAL,
					   NULL);
	DIE(hFile == INVALID_HANDLE_VALUE, "CreateFile ");

	ZeroMemory(buf, BUFSIZE);
	memcpy(buf, "Testing 123", strlen("Testing 123"));

	bRet = WriteFile(hFile,
					 buf,
					 strlen(buf),
					 &bytesWritten,
					 NULL);
	DIE(bRet == FALSE, "WriteFile");

	bRet = CloseHandle(hFile);
	DIE(bRet == FALSE, "CloseHandle");

	return 0;
}
