
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "iocp.h"


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

/* file handles */
static HANDLE *fds;


/* OVERLAPPED array (one for each file) */
static OVERLAPPED *ov;

/* I/O completion port handle */
static HANDLE iocp;

static char g_buffer[BUFSIZ];

static void open_files(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;

	fds = (HANDLE *) malloc(n_files * sizeof(HANDLE));
	if (fds == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

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

	srand((unsigned int) time(NULL));

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
	memset(lpo, 0, sizeof(*lpo));
	lpo->Offset = offset;
	lpo->hEvent = hEvent;
}

static void init_io_async(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;

	/* TODO - allocate memory for ov array */

	/* TODO - init I/O completion port context */

	/* TODO - add handles to completion port */

}

static void free_io_async(void)
{
	free(ov);
}

/*
 * write data asynchronously (overlapped I/O)
 */

static void do_io_async(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;
	DWORD dwRet;

	for (i = 0; i < n_files; i++) {
		dwRet = WriteFile(
					fds[i],
					g_buffer,
					BUFSIZ,
					NULL,
					&ov[i]);
		/* TODO - treat possbile error that is not ERROR_IO_PENDING */

	}
}

static void wait_io_async(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;
	BOOL bRet;

	for (i = 0; i < n_files; i++) {
		DWORD bytes;
		OVERLAPPED *op;
		ULONG_PTR key;

		bRet = iocp_wait(iocp, &bytes, &key, &op);
		DIE(bRet == FALSE, "iocp_wait");

		printf("Written %lu bytes\n", bytes);
	}
}

int main(void)
{
	open_files();
	init_buffer();

#if IO_OP_TYPE == IO_ASYNC
	init_io_async();
	do_io_async();
	wait_io_async();
	free_io_async();
#elif IO_OP_TYPE == IO_SYNC
	do_io_sync();
#else
#error "Unknown value for IO_OP_TYPE"
#endif

	close_files();

	return 0;
}
