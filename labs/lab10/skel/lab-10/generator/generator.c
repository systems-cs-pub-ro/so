/*
 * Generate a file of given size and fill it with random chars
 * @param1: file size
 * @param2: file name
 */

#include "utils.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
	DWORD noBytes, i;
	DWORD dwRet, dwBytesWritten;
	char *buffer;
	HANDLE hFile;

	if (argc != 3) {
		fprintf(stdout, "Usage: ./%s no_bytes file_name\n", argv[0]);
		return -1;
	}

	sscanf_s(argv[1], "%i", &noBytes, sizeof(int));
	buffer = calloc(noBytes + 1, sizeof(char));
	DIE(buffer == NULL, "calloc");

	srand((int)time(NULL) + noBytes);
	for (i = 0; i < noBytes; i++)
		buffer[i] = rand() % 256;

	hFile = CreateFile(argv[2],
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	DIE(hFile == INVALID_HANDLE_VALUE, "CreateFile");

	dwRet = WriteFile(hFile, buffer, noBytes, &dwBytesWritten, NULL);
	DIE(dwRet == FALSE, "WriteFile");

	return 0;
}
