#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/logmemcache.h"

int main() {
	struct logmemcache_st *client;
	struct client_logline **lines;
	char *stats;
	uint64_t logs, i;
	char time[TIME_SIZE + 1];
	char logline[LINE_SIZE - TIME_SIZE + 1];

	client = logmemcache_create(NULL);

	logmemcache_add_log(client, "this is a log line");
	logmemcache_add_log(client, "this is a log line");
	logmemcache_add_log(client, "this is a log line");
	logmemcache_add_log(client, "this is a log line");

	stats = logmemcache_get_stats(client);
	printf("%s\n", stats);
	lines = logmemcache_get_logs(client, 0, 0, &logs);

	for (i = 0; i < logs; i++) {
		logmemcache_get_time(time, lines[i], sizeof(time));
		logmemcache_get_logline(logline, lines[i], sizeof(logline));
		printf("%s: %s\n", time, logline);
	}

	return 0;
}
