/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

#define NO_THREADS 8

LPDWORD pmap; /* File mapping */
HANDLE threads[NO_THREADS];

HANDLE init_setup(LPSTR filename);
LPVOID MapFile(HANDLE hFile, DWORD size);

DWORD N;

typedef struct _chunk {
	DWORD dwLen;	/* number of elements in the chunk */
	LPDWORD lpMem;   /* start of chunk */
} CHUNK;
typedef CHUNK * LPCHUNK;

typedef struct _thread_data {
	DWORD threadId;
	LPCHUNK lpChunk;
} THREAD_DATA;
THREAD_DATA thData[NO_THREADS];

static int CompareFunc(const void *a, const void *b)
{
	if (*(DWORD *)a < *(DWORD *)b)
		return -1;
	if (*(DWORD *)a > *(DWORD *)b)
		return 1;
	return 0;	
}

static void print_mem(LPDWORD mem, DWORD size)
{
	DWORD i;

	for (i = 0; i < size; i++)
		printf("%d ", mem[i]);
	printf("\n");
}

/* Merge adjacent chunks */
LPCHUNK MergeChunks(LPCHUNK lpSrc, LPCHUNK lpDst)
{
	DWORD lenSrc = lpSrc->dwLen;
	DWORD lenDst = lpDst->dwLen;
	DWORD i1 = 0, i2 = 0, im = 0;

	LPDWORD srcArray = lpSrc->lpMem;
	LPDWORD dstArray = lpDst->lpMem;

	LPCHUNK merged = malloc(sizeof(CHUNK));
	LPDWORD mergeArray = malloc((lenSrc + lenDst) * sizeof(DWORD));

	while (i1 < lenSrc && i2 < lenDst) {
		if (CompareFunc(&(srcArray[i1]), &(dstArray[i2])) < 0) {
			mergeArray[im] = srcArray[i1];
			i1++;
			im++;
		} else {
			mergeArray[im] = dstArray[i2];
			i2++;
			im++;
		}
	}

	if (i1 < lenSrc)
		memcpy(&(mergeArray[im]), &(srcArray[i1]),
				(lenSrc - i1) * sizeof(DWORD));
	else if (i2 < lenDst)
		memcpy(&(mergeArray[im]), &(dstArray[i2]),
				(lenDst - i2) * sizeof(DWORD));

	merged->lpMem = mergeArray; /* srcArray; */
	merged->dwLen = lenSrc + lenDst;


	return merged;
}


DWORD WINAPI ThreadFunc(LPVOID lpParameter)
{
	DWORD dwRet, pow2;
	THREAD_DATA td = *(THREAD_DATA *) lpParameter;
	LPCHUNK chunk = td.lpChunk;

	printf("thread %d starting..\n", td.threadId);

	/* sort the local chunk */
	qsort(chunk->lpMem, chunk->dwLen, sizeof(DWORD), CompareFunc);


	pow2 = 1;

	while ((td.threadId % (2 * pow2) == 0) &&
			(td.threadId + pow2 < NO_THREADS)) {

		/* TODO - merge chunks */
	}

	printf("thread %d done\n", td.threadId);

	return 0;
}

HANDLE init_setup(LPSTR filename)
{
	DWORD dwFileSize = 0, dwRet, i, chunkSize;	
	HANDLE hFile;
	LPCHUNK chunk;

	/* Open file */
	hFile = CreateFile(filename, 
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	DIE(hFile == INVALID_HANDLE_VALUE, "CreateFile");

	/* Get file size */
	dwFileSize = GetFileSize(hFile, NULL);
	DIE(dwFileSize == INVALID_FILE_SIZE, "GetFileSize");

	/* Get number of integers in file */
	N = dwFileSize / sizeof(DWORD);
	chunkSize = (N - 1 + NO_THREADS) / NO_THREADS;

	/* Map file */
	pmap = MapFile(hFile, dwFileSize);
	print_mem(pmap, N);

	/* TODO - Create threads and assign chunks */
	for (i = 0; i < NO_THREADS; i++) {
		chunk = malloc(sizeof(CHUNK));

		/* TODO - init chunk */

		/* TODO - create thread */
	}

	return hFile;
}

int main(int argc, LPSTR argv[])
{
	DWORD dwRet, i;
	HANDLE hFile;

	if (argc != 2) {
		printf("Usage: %s file_with_numbers\n", argv[0]);
		ExitProcess(0);
	}

	hFile = init_setup(argv[1]);
	
	/* Wait for the merging to finish */
	dwRet = WaitForSingleObject(threads[0], INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject main");

	printf("Threading part done\n");

	for (i = 0; i < NO_THREADS; i++) {
		dwRet = CloseHandle(threads[i]);
		DIE(dwRet == FALSE, "CloseHandle");
	}

	/* Check if numbers are sorted */
	print_mem(thData[0].lpChunk->lpMem, thData[0].lpChunk->dwLen);
	for (i = 0; i < N-1; i++) {
		if (thData[0].lpChunk->lpMem[i] >
				thData[0].lpChunk->lpMem[i+1])
			printf("Check failed - found %d before %d\n",
					thData[0].lpChunk->lpMem[i],
					thData[0].lpChunk->lpMem[i+1]);
	}

	printf("All done. Cleaning...\n");

	dwRet = UnmapViewOfFile(pmap);
	DIE(dwRet == FALSE, "UnmapViewOfFile");

	dwRet = CloseHandle(hFile);
	DIE(dwRet == FALSE, "CloseHandle");

	ExitProcess(0);
}


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



