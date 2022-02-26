/**
 * SO
 * Lab #2, Simple I/O operations
 *
 * Task #1, Windows
 *
 * Implementing cat
 */

/* Do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>

#include <utils.h>

#define BUFSIZE 8

int main(int argc, char *argv[])
{
	HANDLE hFile;
	CHAR buf[BUFSIZE];
	BOOL bRet;
	DWORD bytesRead;
	DWORD bytesWritten;
	DWORD totalWritten;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Open file with name pointed by argv[1] */
	hFile = CreateFile(argv[1],
					   GENERIC_READ,
					   0,
					   NULL,
					   OPEN_EXISTING,
					   FILE_ATTRIBUTE_NORMAL,
					   NULL);
	DIE(hFile == INVALID_HANDLE_VALUE, "CreateFile argv[0]");

	/* Read file pointed by argv[1] and send it to standard output */
	while (1) {
		bRet = ReadFile(hFile,
						buf,
						BUFSIZE,
						&bytesRead,
						NULL);
		DIE(bRet == FALSE, "ReadFile");

		if (bytesRead == 0)
			break;

		/* How much we've actually written */
		totalWritten = 0;
		do {
			bRet = WriteFile(GetStdHandle(STD_OUTPUT_HANDLE),
							 buf + totalWritten,
							 bytesRead,
							 &bytesWritten,
							 NULL);
			DIE(bRet == FALSE, "WriteFile");
			totalWritten += bytesWritten;
		} while (totalWritten < bytesRead);
	}

	bRet = CloseHandle(hFile);
	DIE(bRet == FALSE, "CloseHandle");

	return 0;
}
