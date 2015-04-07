/*
 * virtual machine monitor - OS specific header inclusions and wrapper calls
 *
 * implementations in vmsim_test_{lin,win}.h
 *
 * 2011, Operating Systems
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/* return number of faults (internal counter in test_lin.c) */
size_t vmsim_test_get_num_faults(void);

/* set number of faults */
void vmsim_test_set_num_faults(size_t num_faults);

/* reset number of faults */
#define vmsim_test_reset_faults()		\
	vmsim_test_set_num_faults(0)

#if defined(_WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x501	/* vectored exception handler */
#endif

#include <windows.h>
LONG vmsim_test_exception_handler(PEXCEPTION_POINTERS info);

#elif defined(__linux__)

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

/* SIGSEGV (exception) handler */
void vmsim_test_segv_handler(int signum, siginfo_t *info, void *context);

#else
  #error "Unknown platform"
#endif

#ifdef __cplusplus
}
#endif
