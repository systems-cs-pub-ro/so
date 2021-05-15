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

const struct op ops[] = {
	{ CONNECT,	"connect",	"client connected",	0 },
	{ SUBSCRIBE,	"subscribe",	"client subscribed",	1 },
	{ STAT,		"stat",		"received stats",	1 },
	{ ADD,		"add",		"log added",		1 },
	{ FLUSH,	"flush",	"logs flushed",		1 },
	{ DISCONNECT,	"disconnect", 	"client disconnected",	1 },
	{ UNSUBSCRIBE,	"unsubcribe", 	"client unsubscribed",	1 },
	{ GETLOGS,	"getlogs", 	"logs received",	1 },
	{ UNKNOWN,	NULL,		"unknown command",	0 },
};

const struct op *get_op(enum op_code code) {
	int i;

	for (i = 0; i < nitems(ops); i++)
		if (ops[i].code == code)
			return &ops[i];

	return &ops[UNKNOWN];
}

const struct op *get_op_by_str(const char *str) {
	int i;
	const struct op *op;

	for (i = 0; i < nitems(ops); i++) {
		op = &ops[i];
		if (op->op_str != NULL && strcmp(str, op->op_str) == 0)
			return op;
	}

	return &ops[UNKNOWN];
}

static ssize_t xsend(SOCKET sockfd, const void *buf, size_t len, int flags)
{
	ssize_t total, sent_now, to_send;

	total = 0;
	to_send = len;
	while (total < to_send) {
		sent_now = send(sockfd, (char *)buf + total,
				to_send - total, flags);
		if (sent_now < 0) {
			total = -1;
			break;
		}
		total += sent_now;
	}

	return total;
}

ssize_t send_data(SOCKET sockfd, const void *buf, size_t len, int flags)
{
	ssize_t sent_now;
	uint32_t buf_l = htonl((uint32_t)len);

	sent_now = xsend(sockfd, &buf_l, sizeof(buf_l), flags);
	if (sent_now < 0)
		return sent_now;

	return xsend(sockfd, buf, len, flags);
}

static ssize_t xrecv(SOCKET sockfd, void *buf, size_t len, int flags)
{
	ssize_t total, recv_now, to_recv;

	total = 0;
	to_recv = len;
	while (total < to_recv) {
		recv_now = recv(sockfd, (char *)buf + total,
				to_recv - total, flags);
		if (recv_now == 0)
			break;
		else if (recv_now < 0) {
			total = -1;
			break;
		}
		total += recv_now;
	}

	return total;
}

ssize_t recv_data(SOCKET sockfd, void *buf, size_t len, int flags)
{
	uint32_t pack_size;
	ssize_t recv_now;
	uint32_t buf_l;

	recv_now = xrecv(sockfd, &buf_l, sizeof(buf_l), MSG_WAITALL | flags);

	if (recv_now < 0)
		return recv_now;

	pack_size = ntohl(buf_l);

	if (pack_size > len)
		return -1;

	return xrecv(sockfd, buf, pack_size, flags);
}


#ifdef __unix__

int current_time_to_string(char *result, size_t len, const char *fmt)
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

int check_old_logfile(char *filepath)
{
	struct stat s;
	int rc;
	char timeap[TIME_SIZE];
	char new_name[CLIENT_MAX_NAME * 4 + TIME_SIZE + 2];

	rc = stat(filepath, &s);
	if (rc != 0) /* file does not exist */
		return 0;

	if (s.st_mode & S_IFREG) {
		/* file exists and is regular file */
		if (current_time_to_string(timeap, TIME_SIZE,
				FTIME_FORMAT) != 0)
			return -1;

		sprintf(new_name,"%s.%s", filepath, timeap);
		rename(filepath, new_name);
		fprintf(stderr, "File %s was renamed in %s\n",
				filepath, new_name);
	} else {
		/* file exists, but is not regular file */
		return -1;
	}

	return 0;
}

int init_logdir(char *logfile_path)
{
	int rc;
	struct stat s = {0};

	stat(logfile_path, &s);

	if (s.st_mode & S_IFDIR)
		return 0;

	rc = mkdir(logfile_path, 0777);

	return rc;
}

#elif defined(_WIN32)
int current_time_to_string(char *result, size_t len, const char *fmt)
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

int check_old_logfile(char *filepath)
{
	DWORD fileAttribute;
	char timeap[TIME_SIZE];
	char new_name[CLIENT_MAX_NAME * 4 + TIME_SIZE + 2];

	fileAttribute = GetFileAttributes(filepath);

	if (fileAttribute != INVALID_FILE_ATTRIBUTES)
		/* file does not exist */
		return 0;

	if (fileAttribute & FILE_ATTRIBUTE_NORMAL) {
		/* file exist, must be renamed */
		if (current_time_to_string(timeap, TIME_SIZE,
				FTIME_FORMAT) != 0)
			return -1;

		snprintf(new_name, MAX_PATH, "%s.%s", filepath, timeap);
		MoveFile(filepath, new_name);
		fprintf(stderr, "File %s was renamed in %s\n",
				filepath, new_name);
	} else {
		/* file exist, but is not regular file */
		return -1;
	}

	return 0;
}

int init_logdir(char *logfile_path)
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
