#include <stdio.h>
#include <syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <dlfcn.h>

/*
 * Laboratorul 13 SO (http://elf.cs.pub.ro/so/wiki/)
 * Wrapping pthread_create() and start_routine
 */
#define MESSAGE  "I cracked your computer\n"

static void *(*user_start_routine)(void*) = NULL;
static void *lh = NULL;
static int (*real_fnc)(void *thread,
        const void *attr,
        void *(*start_routine)(void*), void *arg) = NULL;

int pthread_create(void *thread,
        const void *attr,
        void *(*start_routine)(void*), void *arg);

void init(void)
{
    static int init = 0;
    if (init)
        return;

    lh = dlopen("libpthread.so.0", RTLD_LAZY | RTLD_LOCAL);
    printf("lh = %p\n", lh);
    if (!lh)
        return;

    real_fnc = (int (*)(void *thread,
                const void *attr,
                void *(*)(void*), void *arg)) dlsym(lh, "pthread_create");
    if (!real_fnc)
    {
        dlclose(lh);
        return;
    }
    init = 1;
    printf("[init] real_pthread_create %p, my_pthread_create %p, library handle %p\n", real_fnc, pthread_create, lh);
}

void *my_start_routine(void *arg)
{
    printf("[my start routine] dispatching the user's start routine, {%d}\n", pthread_self());
    if (user_start_routine)
        user_start_routine(arg);
}

int pthread_create(void *thread,
        const void *attr,
        void *(*start_routine)(void*), void *arg)
{
    int result;

    init();

    if (!real_fnc)
        return -1;

    result = real_fnc(thread, attr, my_start_routine, arg);
    printf("[my pthread_create] Spawned new thread {%d}\n", pthread_self());
    return result;
}

