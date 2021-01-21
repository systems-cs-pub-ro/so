#include <stdio.h>
#include <stdlib.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

pid_t hook_fork(void);

struct func_hook hooks[] = {
	[0] = { .name = "fork", .addr = (unsigned long)hook_fork, .orig_addr = 0 },
};

pid_t hook_fork(void)
{
	return -1;
}

int main(int argc, char *argv[])
{
	SO_FILE *f;

	install_hooks("libso_stdio.so", hooks, 1);

	/* --- BEGIN TEST --- */

	f = so_popen("ls", "r");
	FAIL_IF(f != NULL, "popen should have failed: got %p, expected %p\n", f, NULL);

	return 0;
}
