#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

int num_WriteFile;
HANDLE target_handle;

BOOL WINAPI hook_WriteFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

struct func_hook hooks[] = {
	{ "kernel32.dll", "WriteFile", (unsigned long)hook_WriteFile, 0 },
};


//this will declare buf[] and buf_len
#include "large_file.h"


BOOL WINAPI hook_WriteFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	BOOL (WINAPI *orig_WriteFile)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED) =
		(BOOL (WINAPI *)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED))hooks[0].orig_addr;

	if (hFile == target_handle)
		num_WriteFile++;

	/* simulate WriteFile failure after 2 calls */
	if ((hFile == target_handle) && (num_WriteFile == 3)) {
		if (lpNumberOfBytesWritten)
			*lpNumberOfBytesWritten = 0;
		return FALSE;
	}

	return orig_WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}


int main(int argc, char *argv[])
{
	SO_FILE *f;
	int ret;
	char *test_work_dir;
	char fpath[256];

	install_hooks("so_stdio.dll", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/large_file", test_work_dir);


	/* --- BEGIN TEST --- */
	f = so_fopen(fpath, "w");
	FAIL_IF(!f, "Couldn't open file: %s\n", fpath);

	target_handle = so_fileno(f);

	num_WriteFile = 0;

	ret = so_fwrite(buf, 1, buf_len, f);

	ret = so_ferror(f);
	FAIL_IF(ret == 0, "Incorrect return value for so_ferror: got %d, expected != 0\n", ret);

	so_fclose(f);


	return 0;
}
