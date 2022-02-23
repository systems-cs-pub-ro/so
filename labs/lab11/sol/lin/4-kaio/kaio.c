/**
 * SO
 * Lab #11
 *
 * Task #4,
 *
 * Asyncronous I/O - KAIO
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/eventfd.h>
#include <linux/types.h>
#include <sys/syscall.h>
#include <libaio.h>

#include "utils.h"

#define USE_EVENTFD	1

#ifndef BUFSIZ
	#define BUFSIZ		4096
#endif

/* File names */
static char *files[] = {
	"/tmp/slo.txt",
	"/tmp/oer.txt",
	"/tmp/rse.txt",
	"/tmp/ufver.txt"
};

/* eventfd file descriptor */
int efd;

/* File descriptors */
static int *fds;
static char g_buffer[BUFSIZ];

static void open_files(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;

	fds = malloc(n_files * sizeof(int));
	DIE(fds == NULL, "malloc");

	for (i = 0; i < n_files; i++) {
		fds[i] = open(files[i], O_CREAT | O_WRONLY | O_TRUNC, 0666);
		DIE(fds[i] < 0, "open");
	}
}

static void close_files(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;
	int rc;

	for (i = 0; i < n_files; i++) {
		rc = close(fds[i]);
		DIE(rc < 0, "close");
	}

	free(fds);
}

/**
 * Init buffer with random data
 */
static void init_buffer(void)
{
	size_t i;

	srand(time(NULL));

	for (i = 0; i < BUFSIZ; i++)
		g_buffer[i] = (char) rand();
}


/**
 * Wait for asynchronous I/O operations
 * (eventfd or io_getevents)
 */
static void wait_aio(io_context_t ctx, int nops)
{
	struct io_event *events;
	u_int64_t efd_ops = 0;
	int rc, i;

	/* TODO 1 - alloc structure */
	events = (struct io_event *) malloc(nops * sizeof(struct io_event));
	DIE(events == NULL, "malloc");

#ifndef USE_EVENTFD

	/**
	 * TODO 1 - wait for async operations to finish
	 *
	 * Use only io_getevents()
	 */

	rc = io_getevents(ctx, nops, nops, events, NULL);
	DIE(rc < 0, "io_getevents");

#else

	/**
	 * TODO 2 - wait for async operations to finish
	 *
	 * Use eventfd for completion notify
	 */
	for (i = 0; i < nops; ) {
		rc = read(efd, &efd_ops, sizeof(efd_ops));
		DIE(rc < 0, "read eventfd");

		printf("%d aio ops completed\n", (int) efd_ops);
		i += efd_ops;
	}
#endif
	free(events);
}

/**
 * write data asynchronously (using io_setup(2), io_sumbit(2),
 *	io_getevents(2), io_destroy(2))
 */
static void do_io_async(void)
{
	size_t n_files = sizeof(files) / sizeof(files[0]);
	size_t i;
	io_context_t ctx = 0;
	struct iocb *iocb;
	struct iocb **piocb;
	int n_aio_ops;
	int rc;

	/* TODO 1 - Allocate iocb and piocb */
	iocb = (struct iocb *) malloc(n_files * sizeof(*iocb));
	DIE(iocb == NULL, "malloc");

	piocb = (struct iocb **) malloc(n_files * sizeof(*piocb));
	DIE(piocb == NULL, "malloc");

	/* TODO 1 - Initialiaze iocb and piocb */
	for (i = 0; i < n_files; i++) {
		io_prep_pwrite(&iocb[i], fds[i], g_buffer, BUFSIZ, 0);
		piocb[i] = &iocb[i];

#ifdef USE_EVENTFD
		/* TODO 2 - Set up eventfd notification */
		io_set_eventfd(&iocb[i], efd);
#endif
	}

	/* TODO 1 - Setup aio context */
	rc = io_setup(n_files, &ctx);
	DIE(rc < 0, "io_setup");

	/* TODO 1 - Submit aio */
	n_aio_ops = io_submit(ctx, n_files, piocb);
	DIE(n_aio_ops < 0, "io_submit");

	/* Wait for completion*/
	wait_aio(ctx, n_files);

	/* TODO 1 - Destroy aio context */
	io_destroy(ctx);
}

int main(void)
{
	open_files();
	init_buffer();

#ifdef USE_EVENTFD
	/* TODO 2 - Init eventfd */
	efd = eventfd(0, 0);
	DIE(efd < 0, "eventfd");
#endif

	do_io_async();

#ifdef USE_EVENTFD
	close(efd);
#endif
	close_files();

	return 0;
}
