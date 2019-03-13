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
	char *tmp;
	int c;
	int i;
	int ret;
	char *test_work_dir;
	char fpath[256];

	tmp = malloc(buf_len);
	FAIL_IF(!tmp, "malloc failed\n");

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

	for (i = 0; i < buf_len; i++) {
		c = so_fputc((int)buf[i], f);

		FAIL_IF((char)c != buf[i], "Incorrect character as position %d: got %x, expected %x\n", i, (unsigned char)c, buf[i]);
	}

	// there should be 3 write syscalls. the file size is 16000 and the buffer size is 4096
	// first 4096 * 3 bytes should be flushed to the file, and the last 3712 should be still in the buffer
	FAIL_IF(num_WriteFile != 3, "Incorrect number of read syscalls: got %d, expected %d\n", num_WriteFile, 3);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	// the last 3712 bytes should be written to the file
	FAIL_IF(num_WriteFile != 4, "Incorrect number of read syscalls: got %d, expected %d\n", num_WriteFile, 4);

	FAIL_IF(!compare_file(fpath, buf, buf_len), "Incorrect data in file\n");

	free(tmp);

	return 0;
}
