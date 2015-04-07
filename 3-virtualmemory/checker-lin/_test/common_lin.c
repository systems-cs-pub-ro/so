/*
 * common wrappers for Linux
 *
 * 2011, Operating Systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>

#include "common.h"
#include "debug.h"

size_t w_get_page_size(void)
{
	return getpagesize();
}

/*
 * empty SIGSEGV handler - does nothing, successfully
 *   for testing purposes only; it would not make sense using it
 */

void empty_exception_handler(int signum, siginfo_t *info, void *context)
{
}

/*
 * set exception handler (catch SIGSEGV signal)
 */

static struct sigaction previous_action;

w_boolean_t w_set_exception_handler(w_exception_handler_t handler)
{
	static struct sigaction sa;

	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGSEGV);
	sa.sa_flags = SA_SIGINFO;

	if (sigaction(SIGSEGV, &sa, &previous_action) < 0)
		return FALSE;

	return TRUE;
}

w_boolean_t w_get_current_exception_handler(w_exception_handler_t *phandler)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGSEGV, NULL, &sa) < 0)
		return FALSE;

	*phandler = sa.sa_sigaction;

	return TRUE;
}

w_boolean_t w_get_previous_exception_handler(w_exception_handler_t *phandler)
{
	*phandler = previous_action.sa_sigaction;

	return TRUE;
}

w_boolean_t w_handle_is_valid(w_handle_t handle)
{
	struct stat sbuf;

	dlog(LOG_DEBUG, "before calling fstat\n");

	dlog(LOG_DEBUG, "handle: %d\n", handle);
	if (fstat(handle, &sbuf) < 0) {
		dlog(LOG_DEBUG, "fstat returns FALSE\n");
		return FALSE;
	}

	dlog(LOG_DEBUG, "fstat returns TRUE\n");
	return TRUE;
}

w_handle_t w_open_file(const char *name, w_mode_t mode)
{
	w_handle_t handle;

	switch (mode) {
	case MODE_FULL_OPEN:
		handle = open(name, O_CREAT | O_TRUNC | O_RDWR, 0644);
		break;
	case MODE_OPEN_EXISTING:
		handle = open(name, O_RDWR);
		break;
	}

	return handle;
}

w_size_t w_get_file_size_by_handle(w_handle_t handle)
{
	struct stat sbuf;

	assert(fstat(handle, &sbuf) == 0);

	return (w_size_t) sbuf.st_size;
}

w_boolean_t w_set_file_pointer(w_handle_t handle, w_size_t offset)
{
	if (lseek(handle, offset, SEEK_SET) < 0)
		return FALSE;

	return TRUE;
}

w_boolean_t w_read_file(w_handle_t handle, w_ptr_t buf, w_size_t size)
{
	if (read(handle, buf, size) < 0)
		return FALSE;

	return TRUE;
}

w_boolean_t w_write_file(w_handle_t handle, w_ptr_t buf, w_size_t size)
{
	if (write(handle, buf, size) < 0)
		return FALSE;

	return TRUE;
}

w_boolean_t w_close_file(w_handle_t handle)
{
	if (close(handle) < 0)
		return FALSE;

	return TRUE;
}

w_boolean_t w_delete_file(const char *name)
{
	if (unlink(name) < 0)
		return FALSE;

	return TRUE;
}

w_boolean_t w_protect_mapping(w_ptr_t addr, w_size_t num_pages,
			      w_prot_t protection)
{
	int prot;

	switch (protection) {
	case PROTECTION_NONE:
		prot = PROT_NONE;
		break;
	case PROTECTION_READ:
		prot = PROT_READ;
		break;
	case PROTECTION_WRITE:
		prot = PROT_READ | PROT_WRITE;
		break;
	}

	dlog(LOG_DEBUG, "addr: %p\n", addr);
	if (mprotect(addr, num_pages * w_get_page_size(), prot) < 0)
		return FALSE;

	dlog(LOG_DEBUG, "mprotect called\n");

	return TRUE;
}

w_boolean_t w_sync_mapping(w_ptr_t addr, w_size_t num_pages)
{
	if (msync(addr, num_pages * w_get_page_size(), 0) < 0) {
		perror("msync");
		return FALSE;
	}
	dlog(LOG_DEBUG, "msync called\n");

	return TRUE;
}
