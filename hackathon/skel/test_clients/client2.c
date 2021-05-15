#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/logmemcache.h"


int main() {
	struct logmemcache_st *client;

	client = logmemcache_create(NULL);

	printf("client = %p\n", client);

	if (client != NULL)
		fprintf(stderr, "Client was created\n");
	else
		fprintf(stderr, "Client was not created\n");

	logmemcache_add_log(client, "this is a log line");

	logmemcache_disconnect(client);

	return 0;
}
