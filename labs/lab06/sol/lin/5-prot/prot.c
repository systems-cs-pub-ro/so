/**
 * SO
 * Lab #7, Virtual Memory
 *
 * Task #5, Linux
 *
 * Changing page acces protection
 */
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "utils.h"

static int pageSize;
static struct sigaction old_action;
char *p;
int how[3] = {PROT_NONE, PROT_READ, PROT_WRITE};

static void segv_handler(int signum, siginfo_t *info, void *context)
{
	char *addr;
	int rc;

	/**
	 * Calling the old signal handler by default for TODO 1
	 * old_action.sa_sigaction(signum, info, context);
	 */

	/* TODO 2 - check if the signal is SIGSEGV */
	if (signum != SIGSEGV) {
		old_action.sa_sigaction(signum, info, context);
		return;
	}

	/**
	 * TODO 2 - Obtain from siginfo_t the memory location
	 * which caused the page fault
	 */
	addr = (char *)info->si_addr;

	/* TODO 2 - Obtain the page which caused the page fault */
	int page = (addr - p) / pageSize;

	if (!(page >= 0 && page < 3)) {
		old_action.sa_sigaction(signum, info, context);
		return;
	}

	/* TODO 2 - Increase protection for that page */
	if (how[page] == PROT_NONE) {
		how[page] = PROT_READ;
		rc = mprotect(p + page * pageSize, pageSize, PROT_READ);
		DIE(rc == -1, "mprotect - giving PROT_READ to page");

		printf("giving PROT_READ to page %d\n", page);
	} else {
		how[page] = PROT_WRITE;
		rc = mprotect(p + page * pageSize, pageSize, PROT_WRITE);
		DIE(rc == -1, "mprotect - giving PROT_WRITE to page");

		printf("giving PROT_WRITE to page %d\n", page);
	}
}

static void set_signal(void)
{
	struct sigaction action;
	int rc;

	action.sa_sigaction = segv_handler;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGSEGV);
	action.sa_flags = SA_SIGINFO;

	rc = sigaction(SIGSEGV, &action, &old_action);
	DIE(rc == -1, "sigaction");
}

static void restore_signal(void)
{
	struct sigaction action;
	int rc;

	action.sa_sigaction = old_action.sa_sigaction;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGSEGV);
	action.sa_flags = SA_SIGINFO;

	rc = sigaction(SIGSEGV, &action, NULL);
	DIE(rc == -1, "sigaction");
}

int main(void)
{
	int rc;
	char ch;

	pageSize = getpagesize();

	/**
	 * TODO 1 - Map 3 pages with the desired memory protection
	 * Use global 'p' variable to keep the address returned by mmap
	 * Use mprotect to set memory protections based on global 'how' array
	 * 'how' array keeps protection level for each page
	 */
	p = mmap(NULL, 3 * pageSize, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	DIE(p == (char *)-1, "mmap");

	rc = mprotect(p + 0 * pageSize, pageSize, PROT_NONE);
	DIE(rc == -1, "mprotect p + 0*pageSize");

	rc = mprotect(p + 1 * pageSize, pageSize, PROT_READ);
	DIE(rc == -1, "mprotect p + 1*pageSize");

	rc = mprotect(p + 2 * pageSize, pageSize, PROT_WRITE);
	DIE(rc == -1, "mprotect p + 2*pageSize");

	set_signal();

	/* TODO 1 - Access these pages for read and write */
	ch = p[0 * pageSize];
	p[0 * pageSize] = 'a';
	ch = p[1 * pageSize];
	p[1 * pageSize] = 'a';
	ch = p[2 * pageSize];
	p[2 * pageSize] = 'a';

	restore_signal();

	/* TODO 1 - Unmap */
	rc = munmap(p, 3 * pageSize);
	DIE(rc == -1, "munmap");

	return 0;
}
