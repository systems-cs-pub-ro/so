#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

HANDLE target_handle;

BOOL WINAPI hook_CloseHandle(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);

struct func_hook hooks[] = {
	{ "kernel32.dll", "CloseHandle", (unsigned long)hook_CloseHandle, 0 },
};


//this will declare buf[] and buf_len
#include "huge_file.h"


BOOL WINAPI hook_CloseHandle(HANDLE hFile)
{
	BOOL (WINAPI *orig_CloseHandle)(HANDLE) = (BOOL (WINAPI *)(HANDLE))hooks[0].orig_addr;

	if (hFile == target_handle)
		return FALSE;

	return orig_CloseHandle(hFile);
}


int main(int argc, char *argv[])
{
	SO_FILE *f1, *f2, *f3;
	char *tmp;
	int ret;
	char *test_work_dir;
	char fpath[256];

	install_hooks("so_stdio.dll", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/huge_file", test_work_dir);

	ret = create_file_with_contents(fpath, buf, buf_len);
	FAIL_IF(ret != 0, "Couldn't create file: %s\n", fpath);


	/* --- BEGIN TEST --- */
	f1 = so_fopen(fpath, "r");
	FAIL_IF(!f1, "Couldn't open file: %s\n", fpath);

	f2 = so_fopen(fpath, "w");
	FAIL_IF(!f2, "Couldn't open file: %s\n", fpath);

	f3 = so_fopen(fpath, "r");
	FAIL_IF(!f3, "Couldn't open file: %s\n", fpath);

	target_handle = so_fileno(f2);

	ret = so_fclose(f3);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	ret = so_fclose(f2);
	FAIL_IF(ret == 0, "Incorrect return value for so_fclose: got %d, expected !%d\n", ret, -1);

	ret = so_fclose(f1);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	return 0;
}
