#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

int num_sys_read;
int num_sys_write;
int target_fd;

ssize_t hook_read(int fd, void *buf, size_t len);
ssize_t hook_write(int fd, void *buf, size_t len);

struct func_hook hooks[] = {
	[0] = { .name = "read", .addr = (unsigned long)hook_read, .orig_addr = 0 },
	[1] = { .name = "write", .addr = (unsigned long)hook_write, .orig_addr = 0 },
};


//this will declare buf[] and buf_len
#include "large_file.h"


ssize_t hook_read(int fd, void *buf, size_t len)
{
	ssize_t (*orig_read)(int, void *, size_t);

	orig_read = (ssize_t (*)(int, void *, size_t))hooks[0].orig_addr;

	if (fd == target_fd)
		num_sys_read++;

	return orig_read(fd, buf, len);
}


ssize_t hook_write(int fd, void *buf, size_t len)
{
	ssize_t (*orig_write)(int, void *, size_t);

	orig_write = (ssize_t (*)(int, void *, size_t))hooks[1].orig_addr;

	if (fd == target_fd)
		num_sys_write++;

	return orig_write(fd, buf, len);
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

	install_hooks("libso_stdio.so", hooks, 2);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/large_file", test_work_dir);

	ret = create_file_with_contents(fpath, buf, buf_len);
	FAIL_IF(ret != 0, "Couldn't create file: %s\n", fpath);


	/* --- BEGIN TEST --- */
	f = so_fopen(fpath, "r+");
	FAIL_IF(!f, "Couldn't open file: %s\n", fpath);

	target_fd = so_fileno(f);

	num_sys_read = 0;
	num_sys_write = 0;

	ret = so_fread(tmp, 1, 5000, f);
	FAIL_IF(ret != 5000, "Incorrect return value for so_fread: got %d, expected %d\n", ret, 5000);
	FAIL_IF(num_sys_read != 2, "Incorrect number of reads: got %d, expected %d\n", num_sys_read, 2);
	FAIL_IF(memcmp(tmp, buf, 5000), "Incorrect data\n");

	// there should be a fseek between fread and fwrite
	ret = so_fseek(f, 3000, SEEK_SET);
	FAIL_IF(ret != 0, "Incorrect return value for so_fseek: got %d, expected %d\n", ret, 0);

	ret = so_fwrite(tmp, 1, 5000, f);
	FAIL_IF(num_sys_write != 1, "Incorrect number of writes: got %d, expected %d\n", num_sys_write, 1);
	FAIL_IF(ret != 5000, "Incorrect return value for so_fwrite: got %d, expected %d\n", ret, 5000);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	FAIL_IF(num_sys_write != 2, "Incorrect number of writes: got %d, expected %d\n", num_sys_write, 2);

	// replicate the operations on buf
	memmove(&buf[3000], tmp, 5000);

	FAIL_IF(!compare_file(fpath, buf, buf_len), "Incorrect data in file\n");

	free(tmp);

	return 0;
}
