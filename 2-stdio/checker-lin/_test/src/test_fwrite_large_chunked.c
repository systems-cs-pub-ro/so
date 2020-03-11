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
	int total;
	int chunk_size = 200;
	int to_write;

	install_hooks("libso_stdio.so", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/large_file", test_work_dir);


	/* --- BEGIN TEST --- */
	f = so_fopen(fpath, "w");
	FAIL_IF(!f, "Couldn't open file: %s\n", fpath);

	target_fd = so_fileno(f);

	num_sys_write = 0;

	total = 0;
	while (total < buf_len) {
		if (total + chunk_size >= buf_len)
			to_write = buf_len - total;
		else
			to_write = chunk_size;

		ret = so_fwrite(&buf[total], 1, to_write, f);
		FAIL_IF(ret != to_write, "Incorrect return value for so_write: got %d, expected %d\n", ret, to_write);

		total += ret;
	}

	// there should be 3 write syscalls. the file size is 16000 and the buffer size is 4096
	// first 4096 * 3 bytes should be flushed to the file, and the last 3712 should be still in the buffer
	FAIL_IF(num_sys_write != 3, "Incorrect number of write syscalls: got %d, expected %d\n", num_sys_write, 3);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	// the last 3712 bytes should be written to the file
	FAIL_IF(num_sys_write != 4, "Incorrect number of write syscalls: got %d, expected %d\n", num_sys_write, 4);

	FAIL_IF(!compare_file(fpath, buf, buf_len), "Incorrect data in file\n");

	return 0;
}
