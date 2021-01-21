/**
 * SO, 2017
 * Lab #2, Operatii I/O simple
 *
 * Task #4, Windows
 *
 * Troubleshoot
 */

/* do not use UNICODE */
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
	DWORD  bytesWritten;

	hFile = CreateFile("so*rulz.txt",
			GENERIC_READ,
			FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

	ZeroMemory(buf, BUFSIZE);
	memcpy(buf, "Testing 123", strlen("Testing 123"));

	bRet = WriteFile(hFile,
			buf,
			strlen(buf),
			&bytesWritten,
			NULL);

	bRet = CloseHandle(hFile);

	return 0;
}
