/**
 * SO, 2014
 * Lab #6, Memoria virtuala
 *
 * Task #3, Windows
 *
 * Implementing copy using mapping
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <windows.h>

#include "utils.h"

static int pageSize = 0x1000;

HANDLE open(const char *filename, DWORD flag)
{
	HANDLE hFile;

	hFile = CreateFile(
			filename,
			FILE_READ_DATA | FILE_WRITE_DATA,
			FILE_SHARE_READ,
			NULL,
			flag,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	DIE(hFile == INVALID_HANDLE_VALUE, "CreateFile");
	return hFile;
}

LPVOID map(HANDLE fd, DWORD size)
{
	HANDLE hFileMap;
	LPVOID p;

	/* TODO map file */

	return p;
}

int main(int argc, char **argv)
{
	HANDLE hSrc, hDst;
	void *pin, *pout;
	WIN32_FILE_ATTRIBUTE_DATA fileAttr;
	DWORD size, rc;

	DIE(argc != 3, "Usage:\n\tcopy <from_file> <to_file>");
	rc = GetFileAttributesEx(argv[1], GetFileExInfoStandard, &fileAttr);
	DIE(rc == FALSE, "GetFileAttributesEx");
	size = fileAttr.nFileSizeLow;

	/* opening files */
	hSrc = open(argv[1], OPEN_EXISTING);
	hDst = open(argv[2], CREATE_ALWAYS);

	/* TODO - truncate the output file to the input file size */

	/* TODO - map files and copy content */

	/* TODO - cleanup */

	return 0;
}
