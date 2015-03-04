/**
 * SO, 2011
 * Lab #2, Operatii I/O simple
 *
 * Task #2, Windows
 *
 * Implementing simple crc method
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>

#include <utils.h>
#include <crc32.h>

#define BUFSIZE 512
#define CHUNKSIZE 32

static void GenerateCrc(CHAR *sourceFile, CHAR *destFile)
{
	HANDLE hRead, hWrite;
	CHAR buf[BUFSIZE];
	BOOL bRet;
	DWORD bytesRead, bytesWritten;
	int crc;

	/* TODO 1 - Open source file for reading */
	
	/* TODO 1 - Create destination file for writing */

	/* read from file  */
	while (1) {

		ZeroMemory(buf, sizeof(buf));

		/* TODO 1 - Read from into buf BUFSIZE bytes */
		
		/* TODO 1 - Test for end of file */

		/* calculate crc for buf */
		crc = update_crc(0, (unsigned char*) buf, bytesRead);

		/* TODO 1 - Write crc to destination file */
	}

	/* TODO 1 - Close files */

	return;
}


static DWORD GetSize(HANDLE file)
{
	DWORD dwSize;

	/*
	 * TODO 2 - Calculate and return file 
	 * size using SetFilePointer 
	 */

	return dwSize;
}

static BOOL CompareFiles(CHAR *file1, CHAR *file2)
{
	DWORD  bytesRead;
	HANDLE hFile1, hFile2;
	CHAR chunk1[CHUNKSIZE], chunk2[CHUNKSIZE];
	BOOL result = FALSE, bRet;

	/* TODO 3 - Open file handles */
	
	/* TODO 3 - Compare file size */

	/* TODO 3 - Compare the actual files, chunk by chunk */
	/*
	while (1) {
		ZeroMemory(chunk1, sizeof(chunk1));
		ZeroMemory(chunk2, sizeof(chunk2));

	}
	*/

exit:
	/* TODO 3 - Close files */

	return result;
}

int main(int argc, char *argv[])
{
	BOOL equal;

	if (argc != 4) {
		fprintf(stderr,"Usage:\n"
				"\t crc.exe -g <input_file> <output_file> - generate crc\n"
				"\t crc.exe -c <file1> <file2>            - compare files\n");
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[1], "-g") == 0){
		GenerateCrc(argv[2], argv[3]);
	}

	if (strcmp(argv[1], "-c") == 0){
		equal = CompareFiles(argv[2], argv[3]);

		if (equal)
			printf("Files are equal\n");
		else
			printf("Files differ\n");
	}

	return 0;
}