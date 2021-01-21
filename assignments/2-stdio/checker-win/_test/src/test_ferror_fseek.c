#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

int num_SetFilePointer;
HANDLE target_handle;

DWORD WINAPI hook_SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);

struct func_hook hooks[] = {
	{ "kernel32.dll", "SetFilePointer", (unsigned long)hook_SetFilePointer, 0 },
};


//this will declare buf[] and buf_len
#include "huge_file.h"


DWORD WINAPI hook_SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{

	DWORD (WINAPI *orig_SetFilePointer)(HANDLE, LONG, PLONG, DWORD) = 
		(DWORD (WINAPI *)(HANDLE, LONG, PLONG, DWORD))hooks[0].orig_addr;

	if (hFile == target_handle)
		num_SetFilePointer++;

	if (num_SetFilePointer == 2)
		return ERROR_NEGATIVE_SEEK;

	return orig_SetFilePointer(hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
}


int main(int argc, char *argv[])
{
	SO_FILE *f;
	char *tmp;
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

	sprintf(fpath, "%s/huge_file", test_work_dir);

	ret = create_file_with_contents(fpath, buf, buf_len);
	FAIL_IF(ret != 0, "Couldn't create file: %s\n", fpath);


	/* --- BEGIN TEST --- */
	f = so_fopen(fpath, "r");
	FAIL_IF(!f, "Couldn't open file: %s\n", fpath);

	target_handle = so_fileno(f);

	num_SetFilePointer = 0;

	ret = so_fread(tmp, 1, buf_len, f);

	FAIL_IF(ret != buf_len, "Incorrect return value for so_fread: got %d, expected %d\n", ret, buf_len);

	FAIL_IF(memcmp(tmp, buf, buf_len), "Incorrect data\n");


	ret = so_fseek(f, 0, SEEK_SET);
	FAIL_IF(ret != 0, "Incorrect offset: got %d, expected %d\n", ret, 0);

	ret = so_fseek(f, 0, SEEK_END);
	FAIL_IF(ret == buf_len, "Incorrect offset: got %d, expected %d\n", ret, -1);

	ret = so_fseek(f, 42, SEEK_SET);
	FAIL_IF(ret != 0, "Incorrect offset: got %d, expected %d\n", ret, 0);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	free(tmp);

	return 0;
}
