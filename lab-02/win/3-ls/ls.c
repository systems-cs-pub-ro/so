/**
 * SO, 2011
 * Lab #2, Operatii I/O simple
 *
 * Task #3, Windows
 *
 * Implementing "ls" tool with options
	 -a - print attributes of the file
	 -r - recursive
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>

#include <utils.h>

#define MAXPATH			256

/* enough to fit something like "1-ls.vcproj.CHOOCH.Administrator.user" */
#define MAX_FILE_NAME	48  

/* Prints count white spaces */
static void PrintPadding(DWORD count);

/* Prints human readable date and time out of a FILETIME structure */
static void PrintFileTime(FILETIME fTime);

/* 
 * List files from directory reprezented by path
 * @path		- path to dir we want to list
 * @bRecursive	- if "ls -r" was issued
 * @bAll		- if "ls -a" was issued
 */
static void ListFiles(TCHAR *path, BOOL bRecursive, BOOL bAll)
{
	WIN32_FIND_DATA ffd;
	FILETIME ftWrite;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwRet;
	TCHAR extendedPath[MAXPATH], newPath[MAXPATH];
	int i = 0;
	

	printf("\n === %s === \n\n", path);
	/* 
	 * Prepare path to FindFirst File -we must add "\*"
	 * extendedPath = path + "\*" 
	 */	
	sprintf_s(extendedPath, MAXPATH, "%s\\*",path);

	/* 
	 * TODO 1 -  Find the first file in the directory 
	 * Use extendePath as argument 
	 */

	/* TODO 1 - List all the files in the directory */
	do{

		if ( /* TODO 2 - Test if file is directory */ 0 ) {

			/* 
			 * TODO 2 - print file name 
			 * For printing spaces use function printPadding(no_spaces) 
			 */		    

			if (bRecursive == TRUE){

				/* TODO 3 - Recursive */	
			}
		} 
		else 
		{
			/* 
			 * TODO 2 - print file name 
			 * For printing spaces use function printPadding(no_spaces) 
			 */		    

			if (bAll == TRUE){

				/* 
				 * TODO 2 - print the time of the last access
				 * Use the PrintFileTime function
				 */

				/* TODO 2 - get File Size */
			}
			printf("\n");
		}
	} while (/* TODO 1 - interate if there are still files in list */ 0 );

	/* TODO 1 - cleanup */
}

int main(int argc, char *argv[])
{
	TCHAR path[MAXPATH];
	BOOL bRecursive = FALSE, bAll = FALSE;
	int i;

	if(argc < 2 || argc > 4) {
		printf("\nUsage: %s <directory name>"
			   "\n\t-R -recursive\n\t-a all atributes\n", argv[0]);
		return (-1);
	}

	/* Parse input line */
	for (i = 1; i < argc; i++){
		if (strcmp(argv[i], "-a") == 0) {
			bAll = TRUE;
			continue;
		}
		if (strcmp(argv[i], "-R") == 0) {
			bRecursive = TRUE;
			continue;
		}

		/* save path */
		strcpy_s(path, MAXPATH, argv[i]);
	}
	
	ListFiles(path, bRecursive, bAll);

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
    printf( "%02d/%02d/%d  %02d:%02d  ",
			stLocal.wMonth, stLocal.wDay, stLocal.wYear,
			stLocal.wHour, stLocal.wMinute);
}

static void PrintPadding(DWORD count)
{
	DWORD i;
	for (i = 0; i < count; i++)
		printf(" ");
}