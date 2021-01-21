/*
 * SO, 2016 - Lab 10 (Advanced I/O Windows)
 * Task #3
 */

#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#ifndef BUFSIZ
#define BUFSIZ		4096
#endif

#define IO_SYNC		1
#define IO_ASYNC	2
#define IO_OP_TYPE	IO_ASYNC

/* file names */
static char *files[] = {
	"slo.txt",
	"oer.txt",
	"rse.txt",
	"ufver.txt"
};

/* file descriptors */
static HANDLE *fds;
static char g_buffer[BUFSIZ];

static void open_files(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;

	fds = (HANDLE *) malloc(n_files * sizeof(HANDLE));
	DIE(fds == NULL, "malloc");

	for (i = 0; i < n_files; i++) {
		fds[i] = CreateFile(
				files[i],
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
#if IO_OP_TYPE == IO_ASYNC
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
#elif IO_OP_TYPE == IO_SYNC
				FILE_ATTRIBUTE_NORMAL,
#endif
				NULL);
		DIE(fds[i] == INVALID_HANDLE_VALUE, "CreateFile");
	}
}

static void close_files(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;

	for (i = 0; i < n_files; i++)
		CloseHandle(fds[i]);

	free(fds);
}

/*
 * init buffer with random data
 */

static void init_buffer(void)
{
	size_t i;

	srand((int)time(NULL));

	for (i = 0; i < BUFSIZ; i++)
		g_buffer[i] = (char) rand();
}

static void xwrite(HANDLE fd, const char *buf, DWORD len)
{
	DWORD bytes_so_far = 0;
	DWORD bytes_written, dwRet;

	while (len > bytes_so_far) {
		dwRet = WriteFile(
				fd,
				buf + bytes_so_far,
				len - bytes_so_far,
				&bytes_written,
				NULL);
		DIE(dwRet == FALSE, "WriteFile");
		bytes_so_far += bytes_written;
	}
}

/*
 * write data synchronously (using write(2))
 */

static void do_io_sync(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;

	for (i = 0; i < n_files; i++)
		xwrite(fds[i], g_buffer, BUFSIZ);
}

/*
 * init OVERLAPPED structure
 */

static void init_overlapped(OVERLAPPED *lpo, DWORD offset,
		HANDLE hEvent)
{
	/* TODO - prepare overlapp structure */

}

/*
 * write data asynchronously (overlapped I/O)
 */

static void do_io_async(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;
	OVERLAPPED *ov;
	DWORD bytes_written, dwRet;

	/* TODO - allocate memory for ov array (n_files elements) */

	/* TODO - init structure and write data asynchronously for all files */

	/* TODO - wait for all asynchronous operations to complete */

}

int main(void)
{
	open_files();
	init_buffer();

#if IO_OP_TYPE == IO_ASYNC
	do_io_async();
#elif IO_OP_TYPE == IO_SYNC
	do_io_sync();
#else
#error "Unknown value for IO_OP_TYPE"
#endif

	close_files();

	return 0;
}
