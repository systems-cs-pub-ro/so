/*
 * common wrappers
 *
 * 2011, Operating Systems
 */

#ifndef COMMON_H_
#define COMMON_H_	1

#define p_sz		w_get_page_size()
#define MAGIC		0x12

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	MODE_FULL_OPEN = 1,
	MODE_OPEN_EXISTING
} w_mode_t;

typedef enum {
	PROTECTION_READ,
	PROTECTION_WRITE,
	PROTECTION_NONE
} w_prot_t;

#if defined(_WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x501	/* vectored exception handler */
#endif

#include <windows.h>

typedef BOOL w_boolean_t;

#define INVALID_HANDLE		INVALID_HANDLE_VALUE

/* type wrappers */

typedef DWORD w_size_t;
typedef HANDLE w_handle_t;
typedef LPVOID w_ptr_t;

typedef LONG(*w_exception_handler_t)(PEXCEPTION_POINTERS);

#elif defined(__linux__)

typedef enum {
	FALSE = -1,
	TRUE = 0
} w_boolean_t;

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#define INVALID_HANDLE		-1

/* vm types */
typedef size_t w_size_t;
typedef int w_handle_t;
typedef void *w_ptr_t;

typedef void (*w_exception_handler_t) (int, siginfo_t *, void *);
void empty_exception_handler(int signum, siginfo_t *info, void *context);

#else
  #error "Unknown platform"
#endif

w_boolean_t w_set_exception_handler(w_exception_handler_t handler);
w_handle_t w_add_exception_handler(w_exception_handler_t handler);
w_boolean_t w_remove_exception_handler(w_ptr_t handle);
w_boolean_t w_get_current_exception_handler(w_exception_handler_t *phandler);
w_boolean_t w_get_previous_exception_handler(w_exception_handler_t *phandler);
w_size_t w_get_page_size(void);

w_boolean_t w_handle_is_valid(w_handle_t handle);
w_handle_t w_open_file(const char *name, w_mode_t mode);
w_boolean_t w_set_file_pointer(w_handle_t handle, w_size_t offset);
w_size_t w_get_file_size_by_handle(w_handle_t handle);
w_boolean_t w_read_file(w_handle_t handle, w_ptr_t buf, w_size_t size);
w_boolean_t w_write_file(w_handle_t handle, w_ptr_t buf, w_size_t size);
w_boolean_t w_close_file(w_handle_t handle);
w_boolean_t w_delete_file(const char *name);

w_boolean_t w_protect_mapping(w_ptr_t addr, w_size_t size, w_prot_t protection);
w_boolean_t w_sync_mapping(w_ptr_t addr, w_size_t num_pages);

#ifdef __cplusplus
}
#endif

#endif
