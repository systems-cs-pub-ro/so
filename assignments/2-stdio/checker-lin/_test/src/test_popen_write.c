#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

int num_sys_write;
int target_fd;

ssize_t hook_write(int fd, void *buf, size_t len);

struct func_hook hooks[] = {
	[0] = { .name = "write", .addr = (unsigned long)hook_write, .orig_addr = 0 },
};


//this will declare buf[] and buf_len
#include "large_file.h"


ssize_t hook_write(int fd, void *buf, size_t len)
{
	ssize_t (*orig_write)(int, void *, size_t);

	orig_write = (ssize_t (*)(int, void *, size_t))hooks[0].orig_addr;

	if (fd == target_fd)
		num_sys_write++;

	return orig_write(fd, buf, len);
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

	install_hooks("libso_stdio.so", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/popen_out", test_work_dir);

	sprintf(cmd, "wc -c > %s", fpath);


	/* BEGIN TEST */
	f = so_popen(cmd, "w");
	FAIL_IF(!f, "popen failed\n");

	target_fd = so_fileno(f);

	num_sys_write = 0;

	ret = so_fwrite(buf, 1, buf_len, f);

	FAIL_IF(num_sys_write != 3, "Incorrect number of write syscalls: got %d, expected %d\n", num_sys_write, 3);

	ret = so_pclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	FAIL_IF(num_sys_write != 4, "Incorrect number of write syscalls: got %d, expected %d\n", num_sys_write, 4);

	expected_fsize = sprintf(expected_fdata, "%d\n", buf_len);
	FAIL_IF(!compare_file(fpath, (unsigned char *)expected_fdata, expected_fsize), "Incorrect data in file\n");

	return 0;
}
