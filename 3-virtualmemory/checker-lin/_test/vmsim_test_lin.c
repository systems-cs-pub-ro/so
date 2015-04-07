/*
 * virtual machine simulator - useful functions for Linux test suite
 *
 * 2011, Operating Systems
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include "vmsim_aux.h"
#include "common.h"

/* expect page fault - testing purposes */
static size_t sig_handler_num_calls;

/*
 * SIGSEGV handler
 */

void vmsim_test_segv_handler(int signum, siginfo_t *info, void *context)
{
	w_exception_handler_t handler;

	if (signum != SIGSEGV)
		return;
	if (info->si_signo != SIGSEGV)
		return;
	if (info->si_code != SEGV_ACCERR)
		return;

	sig_handler_num_calls++;

	/* call vmsim library handler */
	w_get_previous_exception_handler(&handler);
	handler(signum, info, context);
}

size_t vmsim_test_get_num_faults(void)
{
	return sig_handler_num_calls;
}

void vmsim_test_set_num_faults(size_t num_faults)
{
	sig_handler_num_calls = num_faults;
}
