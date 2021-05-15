#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/logmemcache.h"

int main() {
	struct logmemcache_st *client;
	char *stats;

	client = logmemcache_create(NULL);

	logmemcache_add_log(client, "this is a log line");
	logmemcache_add_log(client, "this is a log line");
	logmemcache_add_log(client, "this is a log line");
	logmemcache_add_log(client, "this is a log line");

	stats = logmemcache_get_stats(client);
	printf("%s\n", stats);

	return 0;
}
