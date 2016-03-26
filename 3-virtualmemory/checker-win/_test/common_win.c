/*
 * common wrappers for Windows
 *
 * 2016, Operating Systems
 */

#include "common.h"
#include "util.h"
#include "debug.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <windows.h>

#define HANDLER_CALL_FIRST	1

w_size_t w_get_page_size(void)
{
	SYSTEM_INFO info;

	GetSystemInfo(&info);

	return info.dwAllocationGranularity;
}

/**
 * empty ExceptionHandler
 * for testing purposes only; it would not make sense using it
 */
LONG empty_exception_handler(PEXCEPTION_POINTERS ExceptionInfo)
{
	return EXCEPTION_CONTINUE_EXECUTION;
}

/**
 * add a new vectored exception handler
 */
w_ptr_t w_add_exception_handler(w_exception_handler_t handler)
{
	PVOID h;

	h = AddVectoredExceptionHandler(HANDLER_CALL_FIRST,
					(PVECTORED_EXCEPTION_HANDLER) handler);
	if (h == NULL)
		return NULL;

	return h;
}

w_boolean_t w_remove_exception_handler(w_ptr_t handle)
{
	if (RemoveVectoredExceptionHandler(handle) == 0)
		return FALSE;

	return TRUE;
}

w_boolean_t w_handle_is_valid(w_handle_t handle)
{
	LARGE_INTEGER size;

	dlog(LOG_DEBUG, "before calling GetFileSizeEx\n");

	dlog(LOG_DEBUG, "handle: %d\n", handle);
	if (GetFileSizeEx(handle, &size) == FALSE) {
		dlog(LOG_DEBUG, "GetFileSizeEx returns FALSE\n");
		return FALSE;
	}

	dlog(LOG_DEBUG, "GetFileSizeEx returns TRUE\n");
	return TRUE;
}

w_handle_t w_open_file(const char *name, w_mode_t mode)
{
	w_handle_t handle;

	switch (mode) {
	case MODE_FULL_OPEN:
		handle = CreateFile(
				name,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		break;
	case MODE_OPEN_EXISTING:
		handle = CreateFile(
				name,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		break;
	}

	return handle;
}

/**
 * returns file size if successful, 0 otherwise
 */
w_size_t w_get_file_size_by_handle(w_handle_t handle)
{
	w_size_t size;

	size = GetFileSize(handle, NULL);
	if (size == INVALID_FILE_SIZE)
		return 0;

	return size;
}

w_boolean_t w_set_file_pointer(w_handle_t handle, w_size_t offset)
{
	if (SetFilePointer(handle, offset, NULL, FILE_BEGIN) ==
	    INVALID_SET_FILE_POINTER)
		return FALSE;

	return TRUE;
}

w_boolean_t w_read_file(w_handle_t handle, w_ptr_t buf, w_size_t size)
{
	w_size_t bytesRead;

	if (ReadFile(
			handle,
			buf,
			size,
			&bytesRead,
			NULL) == FALSE)
		return FALSE;

	return TRUE;
}

w_boolean_t w_write_file(w_handle_t handle, w_ptr_t buf, w_size_t size)
{
	w_size_t bytesWritten;

	if (WriteFile(
			handle,
			buf,
			size,
			&bytesWritten,
			NULL) == FALSE)
		return FALSE;

	return TRUE;
}

w_boolean_t w_close_file(w_handle_t handle)
{
	if (CloseHandle(handle) == FALSE)
		return FALSE;

	return TRUE;
}

w_boolean_t w_delete_file(const char *name)
{
	if (DeleteFile(name) == FALSE)
		return FALSE;

	return TRUE;
}

w_boolean_t w_protect_mapping(w_ptr_t addr, w_size_t num_pages,
			      w_prot_t protection)
{
	DWORD prot, oldProt;

	switch (protection) {
	case PROTECTION_NONE:
		prot = PAGE_NOACCESS;
		break;
	case PROTECTION_READ:
		prot = PAGE_READONLY;
		break;
	case PROTECTION_WRITE:
		prot = PAGE_READWRITE;
		break;
	}

	/* commit page */
	if (VirtualAlloc(addr, num_pages * p_sz, MEM_COMMIT, prot) == NULL)
		return FALSE;

	dlog(LOG_DEBUG, "VirtualProtect address: %p\n", addr);
	if (VirtualProtect(addr, num_pages * p_sz, prot, &oldProt) == FALSE)
		return FALSE;

	dlog(LOG_DEBUG, "VirtualProtect called\n");

	return TRUE;
}

w_boolean_t w_sync_mapping(w_ptr_t addr, w_size_t num_pages)
{
	if (FlushViewOfFile(addr, num_pages * p_sz) == FALSE)
		return FALSE;

	dlog(LOG_DEBUG, "FlushViewOfFile called\n");

	return TRUE;
}

