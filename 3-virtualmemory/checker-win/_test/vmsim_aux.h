/*
 * virtual machine monitor - OS specific header inclusions and wrapper calls
 *
 * implementations in vmsim_test_{lin,win}.h
 *
 * 2017, Operating Systems
 */

#ifndef VMSIM_AUX_H_
#define VMSIM_AUX_H_

#include "common.h"

/* return number of faults (internal counter in test_lin.c) */
size_t vmsim_test_get_num_faults(void);

/* set number of faults */
void vmsim_test_set_num_faults(size_t num_faults);

/* reset number of faults */
#define vmsim_test_reset_faults() vmsim_test_set_num_faults(0)

#if defined(_WIN32)

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

#endif

