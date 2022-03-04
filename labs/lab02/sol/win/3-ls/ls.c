/**
 * SO
 * Lab #2, Simple I/O operations
 *
 * Task #3, Windows
 *
 * Implementing "ls" tool with options
	 -a - print attributes of the file
	 -R - recursive
 */

/* Do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>

#include <utils.h>

#define MAXPATH 256

/* Enough to fit something like "1-ls.vcproj.CHOOCH.Administrator.user" */
#define MAX_FILE_NAME 48

/* Prints count white spaces */
static void PrintPadding(DWORD count);

/* Prints human readable date and time out of a FILETIME structure */
static void PrintFileTime(FILETIME fTime);

/* Print program parameters and usage */
static void PrintUsage(char *program);

/**
 * List files from directory reprezented by path
 * @path		- path to dir we want to list
 * @bRecursive	- if "ls -R" was issued
 * @bAll		- if "ls -a" was issued
 */
static void ListFiles(TCHAR *path, BOOL bRecursive, BOOL bAll)
{
	WIN32_FIND_DATA ffd;
	FILETIME ftWrite;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwRet;
	TCHAR extendedPath[MAXPATH];
	TCHAR newPath[MAXPATH];

	int i = 0;

	printf("\n === %s ===\n\n", path);
	/**
	 * Prepare path to FindFirst File -we must add "\*"
	 * extendedPath = path + "\*"
	 */
	sprintf_s(extendedPath, MAXPATH, "%s\\*", path);

	/**
	 * TODO 1 - Find the first file in the directory
	 * Use extendedPath as argument
	 */
	hFind = FindFirstFile(extendedPath, &ffd);
	DIE(hFind == INVALID_HANDLE_VALUE, "FindFirstFile");

	/* TODO 1 - List all the files in the directory */
	do {
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			/**
			 * TODO 1 - Print file name
			 * For printing spaces use the PrintPadding(no_spaces) function
			 */
			printf("<DIR>\t%s", ffd.cFileName);
			PrintPadding(MAX_FILE_NAME - strlen(ffd.cFileName));
			printf("\n");

			if (bRecursive == TRUE) {
				/**
				 * TODO 3 - Recursive
				 * Remember that . and .. are also members of the current
				 * directory. Do not include them in your recursion, or you
				 * might loop infinitely
				 */
				if ((strncmp(ffd.cFileName, ".", 1) != 0) &&
					(strncmp(ffd.cFileName,
							 "..", 2) != 0)) {
					sprintf_s(newPath, MAXPATH, "%s\\%s",
							  path, ffd.cFileName);
					ListFiles(newPath, bRecursive, bAll);
				}
			}
		} else {
			/**
			 * TODO 1 - Print file name
			 * For printing spaces use the PrintPadding(no_spaces) function
			 */
			printf("     \t%s", ffd.cFileName);
			PrintPadding(MAX_FILE_NAME - strlen(ffd.cFileName));

			if (bAll == TRUE) {
				/**
				 * TODO 2 - Print the time of the last access
				 * Use the PrintFileTime function
				 */
				ftWrite = ffd.ftLastWriteTime;
				PrintFileTime(ftWrite);

				/* TODO 2 - Get File Size */
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				printf("%lld bytes", filesize.QuadPart);
			}
			printf("\n");
		}
	} while (FindNextFile(hFind, &ffd) == TRUE);

	/* TODO 1 - Cleanup */
	dwRet = FindClose(hFind);
	DIE(dwRet == FALSE, "FindClose");
}

int main(int argc, char *argv[])
{
	TCHAR path[MAXPATH];
	BOOL bRecursive = FALSE, bAll = FALSE, gotPath = FALSE;
	int i;

	if (argc < 2 || argc > 4) {
		PrintUsage(argv[0]);
		return (-1);
	}

	/* Parse input line */
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-a") == 0) {
			bAll = TRUE;
			continue;
		}
		if (strcmp(argv[i], "-R") == 0) {
			bRecursive = TRUE;
			continue;
		}

		/* Save path */
		strcpy_s(path, MAXPATH, argv[i]);
		gotPath = TRUE;
	}

	if (gotPath)
		ListFiles(path, bRecursive, bAll);
	else
		PrintUsage(argv[0]);

	return 0;
}

static void PrintFileTime(FILETIME fTime)
{
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	/* Converting the last-write time to local time */
	dwRet = FileTimeToSystemTime(&fTime, &stUTC);
	DIE(dwRet == FALSE, "FileTimeToSystemTime");

	dwRet = SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	DIE(dwRet == FALSE, "SystemTimeToTzSpecificLocalTime");

	/* Printing the date and time */
	printf("%02d/%02d/%d  %02d:%02d  ",
		   stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		   stLocal.wHour, stLocal.wMinute);
}

static void PrintPadding(DWORD count)
{
	DWORD i;

	for (i = 0; i < count; i++)
		printf(" ");
}

static void PrintUsage(char *program)
{
	printf("\nUsage: %s <directory name>"
		   "\n\t-R -recursive\n\t-a all atributes\n",
		   program);
}
