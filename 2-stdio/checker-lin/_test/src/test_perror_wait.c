#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

pid_t hook_wait(int *wstatus);
pid_t hook_waitpid(pid_t pid, int *wstatus, int options);

struct func_hook hooks[] = {
	[0] = { .name = "wait", .addr = (unsigned long)hook_wait, .orig_addr = 0 },
	[1] = { .name = "waitpid", .addr = (unsigned long)hook_waitpid, .orig_addr = 0 },
};


//this will declare buf[] and buf_len
#include "large_file.h"


pid_t hook_wait(int *wstatus)
{
	FAIL_IF(1, "wait syscall waits for any child process, use waitpid!");
	return -1;
}

pid_t hook_waitpid(pid_t pid, int *wstatus, int options)
{
	if (pid <= 0)
		FAIL_IF(1, "use waitpid with the child's pid as first argument!");

	return -1;
}

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

	install_hooks("libso_stdio.so", hooks, 2);

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

	total = 0;
	while (!so_feof(f)) {
		ret = so_fread(&tmp[total], 1, chunk_size, f);

		total += ret;
	}

	FAIL_IF(memcmp(tmp, buf, buf_len), "Incorrect data\n");

	ret = so_pclose(f);
	FAIL_IF(ret != -1, "Incorrect return value for so_pclose: got %d, expected %d\n", ret, -1);

	free(tmp);

	return 0;
}
