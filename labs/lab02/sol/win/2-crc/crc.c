/**
 * SO
 * Lab #2, Simple I/O operations
 *
 * Task #2, Windows
 *
 * Implementing grep
 */

/* Do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include <utils.h>
#include <crc32.h>

#define BUFSIZE 512
#define CHUNKSIZE 32

static void WriteCrc(int crc, HANDLE hWrite)
{
	BOOL bRet;
	DWORD dwBytesWritten;
	DWORD dwBytesToWrite = sizeof(crc);

	/* TODO 1 - Write the CRC to the file. Use a loop! */
	do {
		bRet = WriteFile(hWrite,
						 &crc,
						 dwBytesToWrite,
						 &dwBytesWritten,
						 NULL);
		DIE(bRet == FALSE, "WriteFile");

		dwBytesToWrite -= dwBytesWritten;
	} while (dwBytesToWrite);
}

static void GenerateCrc(CHAR *sourceFile, CHAR *destFile)
{
	HANDLE hRead;
	HANDLE hWrite;
	CHAR buf[BUFSIZE];
	BOOL bRet;
	DWORD bytesRead;
	int crc = 0;

	/* TODO 1 - Open source file for reading */
	hRead = CreateFile(sourceFile,
					   GENERIC_READ,
					   0,
					   NULL,
					   OPEN_EXISTING,
					   FILE_ATTRIBUTE_NORMAL,
					   NULL);
	DIE(hRead == INVALID_HANDLE_VALUE, "CreateFile");

	/* TODO 1 - Create destination file for writing */
	hWrite = CreateFile(destFile,
						GENERIC_WRITE,
						0,
						NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
	DIE(hWrite == INVALID_HANDLE_VALUE, "CreateFile");

	/* Read from file  */
	while (1) {
		ZeroMemory(buf, sizeof(buf));

		/* TODO 1 - Read from source file into buf BUFSIZE bytes */
		bRet = ReadFile(hRead,
						buf,
						BUFSIZE,
						&bytesRead,
						NULL);
		DIE(bRet == FALSE, "ReadFile");

		/* TODO 1 - Test for end of file */
		if (bytesRead == 0)
			break;

		/* Calculate crc for buf */
		crc = update_crc(crc, (unsigned char *)buf, bytesRead);
	}

	/* Write crc to destination file */
	WriteCrc(crc, hWrite);

	/* TODO 1 - Close files */
	bRet = CloseHandle(hRead);
	DIE(bRet == FALSE, "CloseHandle");

	bRet = CloseHandle(hWrite);
	DIE(bRet == FALSE, "CloseHandle");
}

static DWORD GetSize(HANDLE file)
{
	DWORD dwSize;
	DWORD dwRet;

	/* TODO 2 - Calculate and return file size using SetFilePointer */
	dwSize = SetFilePointer(file, 0, NULL, FILE_END);
	DIE(dwSize < 0, "SetFilePointer");

	dwRet = SetFilePointer(file, 0, NULL, FILE_BEGIN);
	DIE(dwRet < 0, "SetFilePointer");

	return dwSize;
}

static DWORD ReadChunk(CHAR *chunk, HANDLE hFile)
{
	BOOL bRet;
	DWORD dwBytesRead;
	DWORD dwBytesToRead = CHUNKSIZE;
	DWORD dwTotalRead = 0;

	/**
	 * TODO 3
	 * Read at most CHUNKSIZE bytes from file into the buffer. Use a loop!
	 * Return the number of read bytes.
	 */
	do {
		bRet = ReadFile(hFile,
						chunk,
						dwBytesToRead,
						&dwBytesRead,
						NULL);
		DIE(bRet == FALSE, "ReadFile");

		if (dwBytesRead == 0)
			break;

		dwBytesToRead -= dwBytesRead;
		dwTotalRead += dwBytesRead;
	} while (dwBytesToRead);

	return dwTotalRead;
}

static BOOL CompareFiles(CHAR *file1, CHAR *file2)
{
	DWORD bytesRead1;
	DWORD bytesRead2;
	HANDLE hFile1;
	HANDLE hFile2;
	CHAR chunk1[CHUNKSIZE];
	CHAR chunk2[CHUNKSIZE];
	BOOL result = FALSE;
	BOOL bRet;

	/* TODO 4 - Open file handles */
	hFile1 = CreateFile(file1,
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
	DIE(hFile1 == INVALID_HANDLE_VALUE, "CreateFile");

	hFile2 = CreateFile(file2,
						GENERIC_READ,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
	DIE(hFile2 == INVALID_HANDLE_VALUE, "CreateFile");

	/* TODO 4 - Compare file size */
	if (GetSize(hFile1) != GetSize(hFile2)) {
		printf("Files are of different sizes\n");
		result = FALSE;
		goto exit;
	}

	/* TODO 4 - Compare the CRC files, chunk by chunk */
	while (1) {
		ZeroMemory(chunk1, sizeof(chunk1));
		ZeroMemory(chunk2, sizeof(chunk2));

		bytesRead1 = ReadChunk(chunk1, hFile1);
		bytesRead2 = ReadChunk(chunk2, hFile2);

		/* TODO 4 - Test for the end of the files */
		if (bytesRead1 == 0) {
			result = TRUE;
			break;
		}

		/* TODO 4 - Compare the previously read chunks */
		if (memcmp(chunk1, chunk2, sizeof(chunk1)) != 0) {
			result = FALSE;
			goto exit;
		}
	}

exit:
	/* TODO 4 - Close files */
	bRet = CloseHandle(hFile1);
	DIE(bRet == FALSE, "CloseHandle");

	bRet = CloseHandle(hFile2);
	DIE(bRet == FALSE, "CloseHandle");

	return result;
}

int main(int argc, char *argv[])
{
	BOOL equal;

	if (argc != 4) {
		fprintf(stderr, "Usage:\n"
						"\t crc.exe -g <input_file> <output_file> - "
						"generate crc\n"
						"\t crc.exe -c <file1> <file2>            - "
						"compare files\n");
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[1], "-g") == 0)
		GenerateCrc(argv[2], argv[3]);

	if (strcmp(argv[1], "-c") == 0) {
		equal = CompareFiles(argv[2], argv[3]);

		if (equal)
			printf("Files are equal\n");
		else
			printf("Files differ\n");
	}

	return 0;
}
