#ifndef HOOKS_H_
#define HOOKS_H_

struct func_hook {
	char *name;
	unsigned long addr;
	unsigned long orig_addr;
};

int install_hooks(char *target_lib_name, struct func_hook *hooks, int num);

#endif //HOOKS_H_
