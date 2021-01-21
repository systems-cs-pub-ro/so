#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

int num_sys_read;
int target_fd;

ssize_t hook_read(int fd, void *buf, size_t len);

struct func_hook hooks[] = {
	[0] = { .name = "read", .addr = (unsigned long)hook_read, .orig_addr = 0 },
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


#define DEFAULT_BUFSIZE 4096

int main(int argc, char *argv[])
{
	SO_FILE *f;
	unsigned char *tmp;
	int ret;
	char *test_work_dir;
	char fpath[256];
	char cmd[128];
	int chunk_size = 200;
	int total;

	tmp = malloc(buf_len);
	FAIL_IF(!tmp, "malloc failed\n");

	install_hooks("libso_stdio.so", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/large_file", test_work_dir);

	ret = create_file_with_contents(fpath, buf, buf_len);
	FAIL_IF(ret != 0, "Couldn't create file: %s\n", fpath);



	/* --- BEGIN TEST --- */
	sprintf(cmd, "cat %s", fpath);

	f = so_popen(cmd, "r");
	FAIL_IF(!f, "popen failed\n");

	target_fd = so_fileno(f);

	num_sys_read = 0;

	total = 0;
	while (!so_feof(f)) {
		ret = so_fread(&tmp[total], 1, chunk_size, f);

		total += ret;
	}

	FAIL_IF(memcmp(tmp, buf, buf_len), "Incorrect data\n");

	ret = so_pclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_pclose: got %d, expected %d\n", ret, 0);

	free(tmp);

	return 0;
}
