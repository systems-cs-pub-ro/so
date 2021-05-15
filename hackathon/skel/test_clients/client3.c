#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/logmemcache.h"

char logline[200];

int main() {
	int rc;
	struct logmemcache_st *client;

	client = logmemcache_create("specialC");

	printf("client = %p\n", client);

	for (rc = 0; rc < 50; rc ++) {
		fprintf(stderr, "Send log no %d\n", rc);
		memset(logline, 0, 200);
		sprintf(logline, "this is logline no %d",rc);
		logmemcache_add_log(client, logline);
	}

	logmemcache_flush(client);

	return 0;
}
