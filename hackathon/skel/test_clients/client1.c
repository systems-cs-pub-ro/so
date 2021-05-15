#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/logmemcache.h"


int main() {
	struct logmemcache_st *client;

	client = logmemcache_create(NULL);
	logmemcache_unsubscribe(client);

	printf("client = %p\n", client);

	return 0;
}
