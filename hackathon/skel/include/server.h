/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#ifndef __LMC_SERVER
#define __LMC_SERVER

#include <sys/types.h>
#include "utils.h"

#ifdef __unix__
#include <sys/socket.h>
#endif

#define	LMC_DEFAULT_CLIENTS_NO	20
#define	LMC_FLUSH_TIME		1	/* minutes */
#define	LMC_LOGFILE_NAME_LEN	128

#ifdef __unix__
#define LMC_SEND_FLAGS	MSG_NOSIGNAL
typedef int HANDLE;
#elif defined(_WIN32)
#define LMC_SEND_FLAGS		0
#endif

/**
 * Cache entry for a client service. Contains:
 * @field sevice_name: An identifier for the client linked to this cache;
 * @field ptr: Pointer to the beginning of this cache;
 * @field pages: Number of pages allocated for this cache.
 */
struct lmc_cache {
	char *service_name;
	void *ptr;
	size_t pages;
};

/**
 * Connection to a client service. Contains:
 * @field client_sock: Socket opened to communicate with the client;
 * @field cache: Pointer to the cache allocated for this client.
 */
struct lmc_client {
	SOCKET client_sock;
	struct lmc_cache *cache;
};

/**
 * Command received from the client. Contains:
 * @field op: Operation descriptor. See the list in utils.c for details;
 * @field data: Data to use for the operation. Content depends on the operation.
 */
struct lmc_command {
	const struct lmc_op *op;
	char *data;
};

extern char *lmc_logfile_path;

struct lmc_client *lmc_create_client(SOCKET);
int lmc_get_command(struct lmc_client *);

/* OS Specific functions */
void lmc_init_server_os(void);
int lmc_init_client_cache(struct lmc_cache *);
int lmc_unsubscribe_os(struct lmc_client *);
int lmc_add_log_os(struct lmc_client *, struct lmc_client_logline *);
int lmc_flush_os(struct lmc_client *);

#endif
