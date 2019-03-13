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


//this will declare buf[] and buf_len
#include "huge_file.h"


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
	char *tmp;
	int ret;
	char *test_work_dir;
	char fpath[256];
	int total;
	int chunk_size = 2000;
	int to_read;

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

	num_ReadFile = 0;

	// read the rest of the file in chunks
	total = 0;
	while (total < buf_len) {
		if (total + chunk_size >= buf_len)
			to_read = buf_len - total;
		else
			to_read = chunk_size;

		ret = so_fread(&tmp[total], 1, to_read, f);

		FAIL_IF(ret != to_read, "Incorrect return value for so_fread: got %d, expected %d\n", ret, to_read);

		total += ret;
	}

	FAIL_IF(num_ReadFile != 49, "Incorrect number of reads: got %d, expected %d\n", num_ReadFile, 49);

	FAIL_IF(memcmp(tmp, buf, buf_len), "Incorrect data\n");

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	free(tmp);

	return 0;
}
