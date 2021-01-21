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
#include "huge_file.h"


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
	int chunk_size = 2000;
	int to_write;
	int size_member;
	int total_members;

	install_hooks("libso_stdio.so", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	if (argc == 3)
		size_member = atoi(argv[2]);
	else
		size_member = 1;

	sprintf(fpath, "%s/huge_file", test_work_dir);


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

		total_members = to_write / size_member;
		ret = so_fwrite(&buf[total], size_member, total_members, f);
		FAIL_IF(ret != total_members, "Incorrect return value for so_write: got %d, expected %d\n", ret, total_members);

		total += total_members * size_member;
	}

	FAIL_IF(num_sys_write != 48, "Incorrect number of write syscalls: got %d, expected %d\n", num_sys_write, 48);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	FAIL_IF(num_sys_write != 49, "Incorrect number of write syscalls: got %d, expected %d\n", num_sys_write, 49);

	FAIL_IF(!compare_file(fpath, buf, buf_len), "Incorrect data in file\n");

	return 0;
}
