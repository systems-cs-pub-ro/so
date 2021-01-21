#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

int num_sys_lseek;
int target_fd;

long hook_lseek(int fd, long offset, int whence);

struct func_hook hooks[] = {
	[0] = { .name = "lseek", .addr = (unsigned long)hook_lseek, .orig_addr = 0 }
};


//this will declare buf[] and buf_len
#include "huge_file.h"


long hook_lseek(int fd, long offset, int whence)
{
	long (*orig_lseek)(int, long, int);

	orig_lseek = (long (*)(int, long, int))hooks[0].orig_addr;

	if (fd == target_fd)
		num_sys_lseek++;

	if (num_sys_lseek == 2)
		return -1;

	return orig_lseek(fd, offset, whence);
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

	install_hooks("libso_stdio.so", hooks, 1);

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

	target_fd = so_fileno(f);

	num_sys_lseek = 0;

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
