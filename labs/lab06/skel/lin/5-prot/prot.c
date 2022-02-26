/**
 * SO
 * Lab #6, Virtual Memory
 *
 * Task #5, Linux
 *
 * Changing page access protection
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
	 * Comment the line below when solving TODO 2
	 */
	old_action.sa_sigaction(signum, info, context);

	/* TODO 2 - Check if the signal is SIGSEGV */

	/**
	 * TODO 2 - Obtain from siginfo_t the memory location
	 * which caused the page fault
	 */

	/**
	 * TODO 2 - Obtain the page which caused the page fault
	 * Hint: use the address returned by mmap
	 */

	/* TODO 2 - Increase protection for that page */
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

	set_signal();

	/* TODO 1 - Access these pages for read and write */

	restore_signal();

	/* TODO 1 - Unmap */

	return 0;
}
