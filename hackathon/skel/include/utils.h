/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#ifndef __LOGMEMCACHE_UTILS
#define __LOGMEMCACHE_UTILS

#include <stdint.h>

#ifdef __unix__
#include <inttypes.h>
#elif defined(_WIN32)
#include <winsock2.h>
#endif

#ifdef __unix__
typedef int SOCKET;
#define UINT64_FMT		"%" PRIu64

#elif defined(_WIN32)
typedef int ssize_t;
#define snprintf _snprintf
#define strdup _strdup
#define UINT64_FMT		"%I64u"
#endif

#define	LINE_SIZE		256
#define	COMMAND_SIZE		1024
#define	SERVER_IP		"127.0.0.1"
#define	SERVER_PORT		38379
#define	CLIENT_MAX_NAME		16
#define	STATUS_MAX_SIZE		512
#define	TIME_FORMAT		"%Y/%m/%d-%H:%M:%S"
#define	FTIME_FORMAT		"%Y.%m.%d-%H.%M.%S"
#define	TIME_SIZE		20  /* strlen("YYYY/mm/dd-HH:MM:SS") + 1 */
#define	LOGLINE_SIZE		(LINE_SIZE - TIME_SIZE)
#define	STATS_FORMAT		"Status at %s\nMemory: %ldKB\nLoglines: %lu\n"

#define nitems(arr)		(sizeof(arr) / sizeof(*arr))

/*
 * Permitted operations:
 * connect <name>	// authentication to server
 * stat			// get stats about client (num of logs, memory used)
 * add <logline>	// add logline to store
 * flush		// flush logs to disk
 * disconnect		// deauthenitcation from server
 * unsubcribe		// flush logs to disk; deallocate data for client
 * getlogs [t1 [t2]]	// send back to client logs between t1 and t2
 */

enum op_code {
	CONNECT,	/* new service connects to app */
	SUBSCRIBE,
	STAT,		/* get stats */
	ADD,		/* add log line */
	FLUSH,		/* flush logs */
	DISCONNECT,
	UNSUBSCRIBE,
	GETLOGS,	/* get log [from t1 [to t2]] */
	UNKNOWN,
};

struct op {
	enum op_code code;
	char *op_str;
	char *op_reply;
	int requires_auth;
};

const extern struct op ops[];

struct client_logline {
	char time[TIME_SIZE];
	char logline[LOGLINE_SIZE];
};

const struct op *get_op(enum op_code);
const struct op *get_op_by_str(const char *);
ssize_t recv_data(SOCKET, void *, size_t, int);
ssize_t send_data(SOCKET, const void *, size_t, int);
int current_time_to_string(char *, size_t, const char *);
int check_old_logfile(char *);
int init_logdir(char *);

#endif
