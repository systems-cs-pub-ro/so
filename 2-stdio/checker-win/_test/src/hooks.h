#ifndef HOOKS_H_
#define HOOKS_H_

struct func_hook {
        char *lib_name;
		char *func_name;
        unsigned long addr;
        unsigned long orig_addr;
};

__declspec(dllexport) int install_hooks(char *target_lib_name, struct func_hook *hooks, int num);

#endif //HOOKS_H_