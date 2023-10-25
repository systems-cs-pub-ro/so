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
#include "huge_file.h"


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
	int total;
	int chunk_size = 2000;
	int to_write;
	int size_member;
	int total_members;

	install_hooks("so_stdio.dll", hooks, 1);

        test_work_dir = "_test";
        size_member = 1;

        if (argc >= 2)
                test_work_dir = argv[1];

        if (argc == 3)
                size_member = atoi(argv[2]);

	sprintf(fpath, "%s/huge_file", test_work_dir);


	/* --- BEGIN TEST --- */
	f = so_fopen(fpath, "w");
	FAIL_IF(!f, "Couldn't open file: %s\n", fpath);

	target_handle = so_fileno(f);

	num_WriteFile = 0;

	total = 0;
	while (total < buf_len) {
		if (total + chunk_size >= buf_len)
			to_write = buf_len - total;
		else
			to_write = chunk_size;

		total_members = to_write / size_member;
		ret = so_fwrite(&buf[total], size_member, total_members, f);
		FAIL_IF(ret != total_members, "Incorrect return value for so_write: got %d, expected %d\n", ret, total_members);

		total += total_members * size_member;
	}

	FAIL_IF(num_WriteFile != 48, "Incorrect number of write syscalls: got %d, expected %d\n", num_WriteFile, 48);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	FAIL_IF(num_WriteFile != 49, "Incorrect number of write syscalls: got %d, expected %d\n", num_WriteFile, 49);

	FAIL_IF(!compare_file(fpath, buf, buf_len), "Incorrect data in file\n");

	return 0;
}
