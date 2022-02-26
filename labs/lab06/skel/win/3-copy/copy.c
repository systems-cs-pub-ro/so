/**
 * SO
 * Lab #6, Virtual Memory
 *
 * Task #3, Windows
 *
 * Implementing copy using mapping
 */

/* Do not use UNICODE */
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

	/* TODO - Map file */

	return p;
}

int main(int argc, char **argv)
{
	HANDLE hSrc;
	HANDLE hDst;
	void *pin;
	void *pout;
	WIN32_FILE_ATTRIBUTE_DATA fileAttr;
	DWORD size;
	DWORD rc;

	DIE(argc != 3, "Usage:\n\tcopy <from_file> <to_file>");
	rc = GetFileAttributesEx(argv[1], GetFileExInfoStandard, &fileAttr);
	DIE(rc == FALSE, "GetFileAttributesEx");
	size = fileAttr.nFileSizeLow;

	/* Open files */
	hSrc = open(argv[1], OPEN_EXISTING);
	hDst = open(argv[2], CREATE_ALWAYS);

	/* TODO - Truncate the output file to the input file size */

	/* TODO - Map files and copy content */

	/* TODO - Cleanup */

	return 0;
}
