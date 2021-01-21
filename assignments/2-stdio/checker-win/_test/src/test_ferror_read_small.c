#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

int num_ReadFile;
HANDLE target_handle;

BOOL WINAPI hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);

struct func_hook hooks[] = {
	{ "kernel32.dll", "ReadFile", (unsigned long)hook_ReadFile, 0 },
};


char buf[] = "Hello, World!\n";
int buf_len = sizeof(buf) - 1;


BOOL WINAPI hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	BOOL (WINAPI *orig_ReadFile)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED) =
		(BOOL (WINAPI *)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED))hooks[0].orig_addr;

	/* simulate ReadFile failure */
	if (hFile == target_handle) {
		if (lpNumberOfBytesRead)
			*lpNumberOfBytesRead = 0;
		return FALSE;
	}

	return orig_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}


int main(int argc, char *argv[])
{
	SO_FILE *f;
	char tmp[128];
	int ret;
	char *test_work_dir;
	char fpath[256];

	install_hooks("so_stdio.dll", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/small_file", test_work_dir);

	ret = create_file_with_contents(fpath, (unsigned char *)buf, buf_len);
	FAIL_IF(ret != 0, "Couldn't create file: %s\n", fpath);


	/* --- BEGIN TEST --- */
	f = so_fopen(fpath, "r");
	FAIL_IF(!f, "Couldn't open file: %s\n", fpath);

	target_handle = so_fileno(f);

	num_ReadFile = 0;

	/* this should fail */
	ret = so_fread(&tmp, 1, buf_len, f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fread: got %d, expected %d\n", ret, 0);

	ret = so_ferror(f);
	FAIL_IF(ret == 0, "Incorrect return value for so_ferror: got %d, expected != 0\n", ret);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	return 0;
}
