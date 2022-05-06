/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __unix__

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>

#elif defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#include "include/utils.h"

/**
 * List of operation descriptors. See structure declaration for details.
 */
const struct lmc_op lmc_ops[] = {
	{ LMC_CONNECT,		"connect",	"client connected",	0 },
	{ LMC_SUBSCRIBE,	"subscribe",	"client subscribed",	1 },
	{ LMC_STAT,		"stat",		"received stats",	1 },
	{ LMC_ADD,		"add",		"log added",		1 },
	{ LMC_FLUSH,		"flush",	"logs flushed",		1 },
	{ LMC_DISCONNECT,	"disconnect", 	"client disconnected",	1 },
	{ LMC_UNSUBSCRIBE,	"unsubcribe", 	"client unsubscribed",	1 },
	{ LMC_GETLOGS,		"getlogs", 	"logs received",	1 },
	{ LMC_UNKNOWN,		NULL,		"unknown command",	0 },
};


/**
 * Get an operation from the list descriptor based on its code.
 *
 * @param code: Code of the operation identify. Must be a value from enum
 *              lmc_op_code.
 *
 * @return: Always returns a valid pointer. If the code is not found in the list
 *          the function returns the LMC_UNKNOWN's descriptor.
 */
const struct lmc_op *
lmc_get_op(enum lmc_op_code code) {
	int i;

	for (i = 0; i < nitems(lmc_ops); i++)
		if (lmc_ops[i].code == code)
			return &lmc_ops[i];

	return &lmc_ops[LMC_UNKNOWN];
}

/**
 * Get an operation from the list descriptor based on its name.
 *
 * @param str: Name of the operation to identify.
 *
 * @return: Always returns a valid pointer. If the name is not found in the list
 *          the function returns the LMC_UNKNOWN's descriptor.
 */
const struct lmc_op *
lmc_get_op_by_str(const char *str) {
	int i;
	const struct lmc_op *op;

	for (i = 0; i < nitems(lmc_ops); i++) {
		op = &lmc_ops[i];
		if (op->op_str != NULL && strcmp(str, op->op_str) == 0)
			return op;
	}

	return &lmc_ops[LMC_UNKNOWN];
}

/**
 * Wrapper function that transfers data over a socket. Makes sure that all
 * requested data is transfered unless an error occurs.
 *
 * @param sock: Socket to transfer data over;
 * @param buf: Source / destination buffer;
 * @param len: Length of the buffer;
 * @param flags: Transfer flags;
 * @param dir: Direction of the transfer. 0 for send, 1 for recv.
 *
 * @return: The amount of data transfered, or -1 otherwise.
 */
static ssize_t
lmc_xfer(SOCKET sock, const void *buf, size_t len, int flags, int dir)
{
	ssize_t total, xfered, rc;
	char *ptr = (char *)buf;

	total = (ssize_t) len;
	xfered = 0;
	while (xfered < total) {
		if (dir == 0)
			rc = send(sock, ptr + xfered, total - xfered, flags);
		else
			rc = recv(sock, ptr + xfered, total - xfered, flags);

		if (rc == 0)
			break;
		if (rc < 0) {
			xfered = -1;
			break;
		}

		xfered += rc;
	}

	return xfered;
}

/**
 * Send data over a socket. The transfer is performed in two steps - first send
 * a message with the amount of data that the receipient should expect, and then
 * send the data over the socket. Ensures all data is transfered unless an error
 * occurs.
 *
 * @param sock: Socket to transfer data over;
 * @param buf: Source buffer;
 * @param len: Length of the buffer;
 * @param flags: Transfer flags.
 *
 * @return: The amount of data sent, or -1 otherwise.
 */
ssize_t
lmc_send(SOCKET sock, const void *buf, size_t len, int flags)
{
	ssize_t rc;
	uint32_t buf_l;

	buf_l = htonl((uint32_t)len);
	rc = lmc_xfer(sock, &buf_l, sizeof(buf_l), flags, 0);
	if (rc < 0)
		return rc;

	return lmc_xfer(sock, buf, len, flags, 0);
}

/**
 * Receive data over a socket. The transfer is performed in two steps - first
 * receive a message with the amount of data that the function should expect,
 * and then receive the data over the socket. Ensures all data is transfered
 * unless an error occurs.
 *
 * @param sock: Socket to transfer data over;
 * @param buf: Destination buffer;
 * @param len: Length of the buffer;
 * @param flags: Transfer flags.
 *
 * @return: The amount of data received, or -1 otherwise.
 */
ssize_t
lmc_recv(SOCKET sock, void *buf, size_t len, int flags)
{
	ssize_t rc;
	uint32_t pack_size, buf_l;

	buf_l = 0;
	rc = lmc_xfer(sock, &buf_l, sizeof(buf_l), MSG_WAITALL | flags, 1);
	if (rc < 0)
		return rc;

	pack_size = ntohl(buf_l);
	if (pack_size > len)
		return -1;

	return lmc_xfer(sock, buf, pack_size, flags, 1);
}


#ifdef __unix__
/**
 * Convert the current time into a human-readable string.
 *
 * @param result: Buffer to write the format into;
 * @param len: Length of the buffer;
 * @param fmt: Time format string.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_crttime_to_str(char *result, size_t len, const char *fmt)
{
	time_t t;
	struct tm tm;

	t = time(NULL);
	if (localtime_r(&t, &tm) == NULL)
		return -1;

	if (strftime(result, len, fmt, &tm) == 0)
		return -1;

	return 0;
}

/**
 * Deprecate an old log file. If the file indicated by filepath already exists,
 * move it so a new log file can be created.
 *
 * @param filepath: Path to the file to deprecate.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_rotate_logfile(char *filepath)
{
	struct stat s;
	int rc;
	char timeap[LMC_TIME_SIZE];
	char new_name[LMC_CLIENT_MAX_NAME * 4 + LMC_TIME_SIZE + 2];

	rc = stat(filepath, &s);
	/* file does not exist */
	if (rc != 0)
		return 0;

	if (s.st_mode & S_IFREG) {
		/* file exists and is a regular file */
		if (lmc_crttime_to_str(timeap, LMC_TIME_SIZE, LMC_TIME_FORMAT))
			return -1;

		sprintf(new_name, "%s.%s", filepath, timeap);
		rename(filepath, new_name);
		fprintf(stderr, "File %s was renamed to %s\n",
				filepath, new_name);
	} else {
		/* file exists, but is not regular file */
		return -1;
	}

	return 0;
}

/**
 * Ensure a directory exists.
 *
 * @param logfile_path: Path to the directory.
 *
 * @return: 0 in case the directory already exists or could be created, -1
 *          otherwise.
 */
int
lmc_init_logdir(char *logfile_path)
{
	struct stat s = {0};

	stat(logfile_path, &s);
	if (s.st_mode & S_IFDIR)
		return 0;

	return mkdir(logfile_path, 0777);
}

#elif defined(_WIN32)
/**
 * Convert the current time into a human-readable string.
 *
 * @param result: Buffer to write the format into;
 * @param len: Length of the buffer;
 * @param fmt: Time format string.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_crttime_to_str(char *result, size_t len, const char *fmt)
{
	time_t t;
	struct tm tm;

	t = time(NULL);
	if (localtime_s(&tm, &t) != 0)
		return -1;

	if (strftime(result, len, fmt, &tm) == 0)
		return -1;

	return 0;
}

/**
 * Deprecate an old log file. If the file indicated by filepath already exists,
 * move it so a new log file can be created.
 *
 * @param filepath: Path to the file to deprecate.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_rotate_logfile(char *filepath)
{
	DWORD fileAttribute;
	char timeap[LMC_TIME_SIZE];
	char new_name[LMC_CLIENT_MAX_NAME * 4 + LMC_TIME_SIZE + 2];

	fileAttribute = GetFileAttributes(filepath);

	if (fileAttribute != INVALID_FILE_ATTRIBUTES)
		/* file does not exist */
		return 0;

	if (fileAttribute & FILE_ATTRIBUTE_NORMAL) {
		/* file exist, must be renamed */
		if (lmc_crttime_to_str(timeap, LMC_TIME_SIZE, LMC_TIME_FORMAT))
			return -1;

		snprintf(new_name, MAX_PATH, "%s.%s", filepath, timeap);
		MoveFile(filepath, new_name);
		fprintf(stderr, "File %s was renamed to %s\n",
				filepath, new_name);
	} else {
		/* file exist, but is not regular file */
		return -1;
	}

	return 0;
}

/**
 * Ensure a directory exists.
 *
 * @param logfile_path: Path to the directory.
 *
 * @return: 0 in case the directory already exists or could be created, -1
 *          otherwise.
 */
int
lmc_init_logdir(char *logfile_path)
{
	DWORD fileAttribute;

	fileAttribute = GetFileAttributes(logfile_path);
	if (fileAttribute != INVALID_FILE_ATTRIBUTES)
		if (fileAttribute & FILE_ATTRIBUTE_DIRECTORY)
			return 0;
		else
			return -1;

	if (CreateDirectory(logfile_path, NULL) != 0)
		return 0;
	else
		return -1;

}
#endif
