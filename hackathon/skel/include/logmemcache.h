/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#ifndef __LIBLOGMEMCACHE
#define __LIBLOGMEMCACHE

#include <stdint.h>
#include "utils.h"

struct logmemcache_st {
	SOCKET socket;
	char *name;
};

/* Client API */
struct logmemcache_st *logmemcache_create(char *);
void logmemcache_free(struct logmemcache_st *);
int logmemcache_add_log(struct logmemcache_st *, char *);
int logmemcache_flush(struct logmemcache_st *);
int logmemcache_disconnect(struct logmemcache_st *);
int logmemcache_unsubscribe(struct logmemcache_st *);
struct client_logline **logmemcache_get_logs(struct logmemcache_st *,
	time_t, time_t, uint64_t *);
char *logmemcache_get_stats(struct logmemcache_st *);
int logmemcache_get_time(char *, struct client_logline *, size_t);
int logmemcache_get_logline(char *, struct client_logline *, size_t);

/* OS Specific functions */
int logmemcache_client_init_os(struct logmemcache_st *, char *);
void logmemcache_client_free_os(struct logmemcache_st *);

#endif
