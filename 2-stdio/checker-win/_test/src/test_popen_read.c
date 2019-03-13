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
#include "large_file.h"


BOOL WINAPI hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	BOOL (WINAPI *orig_ReadFile)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED) =
		(BOOL (WINAPI *)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED))hooks[0].orig_addr;

	if (hFile == target_handle)
		num_ReadFile++;

	return orig_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}


#define DEFAULT_BUFSIZE 4096

int main(int argc, char *argv[])
{
	SO_FILE *f;
	unsigned char *tmp;
	int ret;
	int expected_sys_read;
	char *test_work_dir;
	char fpath[256];
	char cmd[128];
	int chunk_size = 200;
	int total;
	int n;
	int i;

	tmp = malloc(buf_len);
	FAIL_IF(!tmp, "malloc failed\n");

	install_hooks("so_stdio.dll", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	n = sprintf(fpath, "%s/large_file", test_work_dir);
	// the type command cannot handle forward slashes
	for (i = 0; i < n; i++)
		if (fpath[i] == '/')
			fpath[i] = '\\';

	ret = create_file_with_contents(fpath, buf, buf_len);
	FAIL_IF(ret != 0, "Couldn't create file: %s\n", fpath);

	expected_sys_read = ((buf_len + DEFAULT_BUFSIZE - 1) / DEFAULT_BUFSIZE) + 1;


	/* --- BEGIN TEST --- */
	sprintf(cmd, "type %s", fpath);

	f = so_popen(cmd, "r");
	FAIL_IF(!f, "popen failed\n");

	// make sure the child process wrote all the data
	Sleep(2000);

	target_handle = so_fileno(f);

	num_ReadFile = 0;

	total = 0;
	while (!so_feof(f)) {
		ret = so_fread(&tmp[total], 1, chunk_size, f);

		total += ret;
	}

	FAIL_IF(num_ReadFile != expected_sys_read, "Incorrect number of reads: got %d, expected %d\n", num_ReadFile, expected_sys_read);

	FAIL_IF(memcmp(tmp, buf, buf_len), "Incorrect data\n");

	ret = so_pclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_pclose: got %d, expected %d\n", ret, 0);

	free(tmp);

	return 0;
}
