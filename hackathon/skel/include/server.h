/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#ifndef __LOGMEMCACHE_SERVER
#define __LOGMEMCACHE_SERVER

#include <sys/types.h>
#include "utils.h"

#ifdef __unix__
#include <sys/socket.h>
#endif

#define	DEFAULT_CLIENTS_NO	20
#define	FLUSH_TIME		1	/* minutes */
#define	LOGFILE_NAME_LEN	128

#ifdef __unix__
#define SEND_FLAGS	MSG_NOSIGNAL
typedef int HANDLE;
#elif defined(_WIN32)
#define SEND_FLAGS		0
#endif

struct logmemcache_cache {
	char *service_name;
	void *ptr;
	size_t pages;
};

struct logmemcache_client_st {
	SOCKET client_sock;
	struct logmemcache_cache *cache;
};

struct command {
	const struct op *op;
	char *data;
};

char *logfile_path;

struct logmemcache_client_st *logmemcache_create_client(SOCKET);
int get_command(struct logmemcache_client_st *);

/* OS Specific functions */
void logmemcache_init_server_os(SOCKET *);
int logmemcache_init_client_cache(struct logmemcache_cache *);
int logmemcache_unsubscribe_os(struct logmemcache_client_st *);
int logmemcache_add_log_os(struct logmemcache_client_st *,
	struct client_logline *);
int logmemcache_flush_os(struct logmemcache_client_st *);

#endif
