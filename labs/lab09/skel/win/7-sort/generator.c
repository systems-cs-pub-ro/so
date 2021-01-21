/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

LPVOID MapFile(HANDLE hFile, DWORD size)
{
	HANDLE hFileMap;
	LPVOID p;

	hFileMap = CreateFileMapping(
			hFile,
			NULL,
			PAGE_READWRITE,
			0,
			size,
			NULL);
	DIE(hFileMap == NULL, "CreateFileMapping");

	p = MapViewOfFile(
			hFileMap,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			0);
	DIE(p == NULL, "MapViewOfFile");

	return p;
}

int main(int argc, LPSTR argv[])
{
	HANDLE hFile;
	LPSTR filename;
	DWORD dwRet, i, N;
	LPDWORD pmap;

	if (argc != 3) {
		printf("Usage: %s filename N_numbers\n", argv[0]);
		return 0;
	}
	
	filename = argv[1];
	N = atoi(argv[2]);

	/* create file */
	hFile = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE,
					0, NULL, CREATE_ALWAYS, 0, NULL);
	DIE(hFile == INVALID_HANDLE_VALUE, "CreateFile");

	/* map file */
	pmap = MapFile(hFile, N * sizeof(DWORD));

	for (i = 0; i < N; i++)
		pmap[i] = rand();

	dwRet = UnmapViewOfFile(pmap);
	DIE(dwRet == FALSE, "UnmapViewOfFile");

	CloseHandle(hFile);

	return 0;
}
