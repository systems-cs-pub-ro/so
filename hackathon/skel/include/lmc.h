/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#ifndef __LMC
#define __LMC

#include <stdint.h>
#include "utils.h"

/**
 * A connection to the server. Contains:
 * @field socket: Connection socket;
 * @field name: An identifier for the client.
 */
struct lmc_conn {
	SOCKET socket;
	char *name;
};

/* Client API */
struct lmc_conn *lmc_connect(char *);
void lmc_free(struct lmc_conn *);
int lmc_send_log(struct lmc_conn *, char *);
int lmc_flush(struct lmc_conn *);
int lmc_disconnect(struct lmc_conn *);
int lmc_unsubscribe(struct lmc_conn *);
struct lmc_client_logline **lmc_get_logs(struct lmc_conn *,
	time_t, time_t, uint64_t *);
char *lmc_get_stats(struct lmc_conn *);
void lmc_free_buf(void *);

/* OS Specific functions */
int lmc_conn_init_os(struct lmc_conn *, char *);
void lmc_conn_free_os(struct lmc_conn *);

#endif
