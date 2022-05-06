/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#ifndef __LMC_UTILS
#define __LMC_UTILS

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

#define	LMC_LINE_SIZE		256
#define	LMC_COMMAND_SIZE	1024
#define	LMC_SERVER_IP		"127.0.0.1"
#define	LMC_SERVER_PORT		38379
#define	LMC_CLIENT_MAX_NAME	16
#define	LMC_STATUS_MAX_SIZE	512
#define	LMC_TIME_FORMAT		"%Y/%m/%d-%H:%M:%S"
#define	LMC_FTIME_FORMAT	"%Y.%m.%d-%H.%M.%S"
#define	LMC_TIME_SIZE		20  /* strlen("YYYY/mm/dd-HH:MM:SS") + 1 */
#define	LMC_LOGLINE_SIZE	(LMC_LINE_SIZE - LMC_TIME_SIZE)
#define	LMC_STATS_FORMAT	"Status at %s\nMemory: %ldKB\nLoglines: %lu\n"

#define nitems(arr)		(sizeof(arr) / sizeof(*arr))

/**
 * Permitted operations:
 * connect <name>	// authentication to server
 * stat			// get stats about client (num of logs, memory used)
 * add <logline>	// add logline to store
 * flush		// flush logs to disk
 * disconnect		// deauthenitcation from server
 * unsubcribe		// flush logs to disk; deallocate data for client
 * getlogs [t1 [t2]]	// send back to client logs between t1 and t2
 */
enum lmc_op_code {
	LMC_CONNECT,		/* new service connects to app */
	LMC_SUBSCRIBE,
	LMC_STAT,		/* get stats */
	LMC_ADD,		/* add log line */
	LMC_FLUSH,		/* flush logs */
	LMC_DISCONNECT,
	LMC_UNSUBSCRIBE,
	LMC_GETLOGS,		/* get log [from t1 [to t2]] */
	LMC_UNKNOWN,
};

/**
 * Operation descriptor format. Contains:
 * @field code: operation code;
 * @field op_str: operation name as a string;
 * @field op_reply: string replied by the server when the operation succeeds;
 * @field requires_auth: whether the operation requires authentication.
 */
struct lmc_op {
	enum lmc_op_code code;
	char *op_str;
	char *op_reply;
	int requires_auth;
};

const extern struct lmc_op lmc_ops[];

/**
 * Format of a log line. Must contain:
 * @field time: a timestamp, in LMC_TIME_FORMAT format;
 * @field logline: free text.
 */
#pragma pack(push, 1)
struct lmc_client_logline {
	char time[LMC_TIME_SIZE];
	char logline[LMC_LOGLINE_SIZE];
};
#pragma pack(pop)

const struct lmc_op *lmc_get_op(enum lmc_op_code);
const struct lmc_op *lmc_get_op_by_str(const char *);
ssize_t lmc_recv(SOCKET, void *, size_t, int);
ssize_t lmc_send(SOCKET, const void *, size_t, int);
int lmc_crttime_to_str(char *, size_t, const char *);
int lmc_rotate_logfile(char *);
int lmc_init_logdir(char *);

#endif
