#include <stdio.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

int target_fd;

int hook_close(int fd);

struct func_hook hooks[] = {
	[0] = { .name = "close", .addr = (unsigned long)hook_close, .orig_addr = 0 },
};


//this will declare buf[] and buf_len
#include "huge_file.h"


int hook_close(int fd)
{
	int (*orig_close)(int);

	orig_close = (int (*)(int))hooks[0].orig_addr;

	if (fd == target_fd)
		return -1;

	return orig_close(fd);
}


int main(int argc, char *argv[])
{
	SO_FILE *f1, *f2, *f3;
	int ret;
	char *test_work_dir;
	char fpath[256];

	install_hooks("libso_stdio.so", hooks, 1);

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(fpath, "%s/huge_file", test_work_dir);

	ret = create_file_with_contents(fpath, buf, buf_len);
	FAIL_IF(ret != 0, "Couldn't create file: %s\n", fpath);


	/* --- BEGIN TEST --- */
	f1 = so_fopen(fpath, "r");
	FAIL_IF(!f1, "Couldn't open file: %s\n", fpath);

	f2 = so_fopen(fpath, "w");
	FAIL_IF(!f2, "Couldn't open file: %s\n", fpath);

	f3 = so_fopen(fpath, "r");
	FAIL_IF(!f3, "Couldn't open file: %s\n", fpath);

	target_fd = so_fileno(f2);

	ret = so_fclose(f3);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	ret = so_fclose(f2);
	FAIL_IF(ret == 0, "Incorrect return value for so_fclose: got %d, expected !%d\n", ret, -1);

	ret = so_fclose(f1);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	return 0;
}
