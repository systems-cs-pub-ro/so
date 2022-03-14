/**
 * SO, 2017
 * Lab #2, Operatii I/O simple
 *
 * Task #2, Windows
 *
 * Implementing simple crc method
 */

/* Do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <crc32.h>
#include <stdio.h>
#include <utils.h>
#include <windows.h>

#define BUFSIZE 512
#define CHUNKSIZE 32

static void WriteCrc(int crc, HANDLE hWrite)
{
	BOOL bRet;
	DWORD dwBytesWritten;
	DWORD dwBytesToWrite;
	DWORD dwTotalWritten;
	/* TODO 1 - Write the CRC to the file. Use a loop! */
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

	/* TODO 1 - Create destination file for writing */

	/* Read from file  */
	while (1) {
		ZeroMemory(buf, sizeof(buf));

		/* TODO 1 - Read from source file into buf BUFSIZE bytes */

		/* TODO 1 - Test for end of file */

		/* Calculate crc for buf */
		crc = update_crc(crc, (unsigned char *)buf, bytesRead);
	}

	/* Write crc to destination file */
	WriteCrc(crc, hWrite);

	/* TODO 1 - Close files */
}

static DWORD GetSize(HANDLE file)
{
	DWORD dwSize;

	/* TODO 2 - Calculate and return file size using SetFilePointer */

	return dwSize;
}

static DWORD ReadChunk(CHAR *chunk, HANDLE hFile)
{
	BOOL bRet;
	DWORD dwBytesRead;
	DWORD dwBytesToRead;
	DWORD dwTotalRead = 0;

	/**
	 * TODO 3
	 * Read at most CHUNKSIZE bytes from file into the buffer. Use a loop!
	 * Return the number of read bytes.
	 */

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

	/* TODO 4 - Compare file size */

	/* TODO 4 - Compare the files, chunk by chunk */
	while (1) {
		ZeroMemory(chunk1, sizeof(chunk1));
		ZeroMemory(chunk2, sizeof(chunk2));

		bytesRead1 = ReadChunk(chunk1, hFile1);
		bytesRead2 = ReadChunk(chunk2, hFile2);

		/* TODO 4 - Test for the end of the files */

		/* TODO 4 - Compare the previously read chunks */
	}

exit:
	/* TODO 4 - Close files */

	return result;
}

int main(int argc, char *argv[])
{
	BOOL equal;

	if (argc != 4) {
		fprintf(stderr,
				"Usage:\n"
				"\tcrc.exe -g <input_file> <output_file> - generate crc\n"
				"\tcrc.exe -c <file1> <file2>            - compare files\n");
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
