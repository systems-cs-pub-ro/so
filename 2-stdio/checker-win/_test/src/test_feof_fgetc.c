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

	if (hFile == target_handle)
		num_ReadFile++;

	return orig_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}


int main(int argc, char *argv[])
{
	SO_FILE *f;
	int c;
	int ret;
	char *test_work_dir;
	char fpath[256];
	int i;

	install_hooks("so_stdio.dll", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/small_file", test_work_dir);

	ret = create_file_with_contents(fpath, (unsigned char *)buf, strlen(buf));
	FAIL_IF(ret != 0, "Couldn't create file: %s\n", fpath);


	/* --- BEGIN TEST --- */
	f = so_fopen(fpath, "r");
	FAIL_IF(!f, "Couldn't open file: %s\n", fpath);

	target_handle = so_fileno(f);

	num_ReadFile = 0;

	for (i = 0; i < buf_len; i++) {
		c = so_fgetc(f);
		FAIL_IF((char)c != buf[i], "Incorrect character: got %x, expected %x\n", (unsigned char)c, buf[i]);

		ret = so_feof(f);
		FAIL_IF(ret != 0, "Incorrect return value for so_feof: got %d, expected %d\n", ret, 0);
	}

	c = so_fgetc(f);
	FAIL_IF(c != SO_EOF, "Incorrect return value for so_fgetc: got %d, expected %d\n", c, SO_EOF);

	ret = so_feof(f);
	FAIL_IF(ret == 0, "Incorrect return value for so_feof: got %d, expected != 0\n", ret);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	return 0;
}
