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

	return orig_WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}


int main(int argc, char *argv[])
{
	SO_FILE *f;
	int ret;
	char *test_work_dir;
	char fpath[256];
	char cmd[1024];
	char expected_fdata[128];
	int expected_fsize;

	install_hooks("so_stdio.dll", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/popen_out", test_work_dir);

	sprintf(cmd, ".\\_test\\bin\\my_wc.exe -c > %s", fpath);

	/* BEGIN TEST */
	f = so_popen(cmd, "w");
	FAIL_IF(!f, "popen failed\n");

	target_handle = so_fileno(f);

	num_WriteFile = 0;

	ret = so_fwrite(buf, 1, buf_len, f);

	FAIL_IF(num_WriteFile != 3, "Incorrect number of write syscalls: got %d, expected %d\n", num_WriteFile, 3);

	ret = so_pclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	FAIL_IF(num_WriteFile != 4, "Incorrect number of write syscalls: got %d, expected %d\n", num_WriteFile, 4);

	expected_fsize = sprintf(expected_fdata, "%d\r\n", buf_len);
	FAIL_IF(!compare_file(fpath, (unsigned char *)expected_fdata, expected_fsize), "Incorrect data in file\n");

	return 0;
}
