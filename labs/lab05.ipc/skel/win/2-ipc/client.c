/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, lin
  *
  * Generic client implementation
  */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "generic_queue.h"
#include "generic_shm.h"
#include "generic_sem.h"
#include "hashtable.h"

#include "utils.h"

static sema_t 		sems[BUCKET_COUNT];
static shm_t 		shmem;
static msgq_t		queue;
static struct htable	*h;	

int main(int argc, char **argv)
{
	int i = 0;
	char c = 0;
	char name[MAX_IPC_NAME];
	message_t msg;

	/* init semaphores */
	for (i = 0; i < BUCKET_COUNT; i++) {
		snprintf(name, MAX_IPC_NAME, "%s%d", BASE_SEMA_NAME, i);
		sems[i] = sema_get(name);
	}

	/* init queue */
	queue = msgq_get(BASE_QUEUE_NAME);
	
	/* init shared memory */
	shmem = shmem_get(BASE_SHM_NAME);
	h = shmem_attach(shmem, sizeof(*h));
	
	/* Execute commands */
	for (i = 1; i < argc; i++) {
		c = argv[i][0];
		memset(&msg, 0, sizeof(msg));
		switch (c) {
			case 'p':  {
				htable_print(h, sems);
				break;
			}
			default: {
				if (c == 'a' || c == 'r') {
					DIE(i + 1 == argc, "invalid input");
					strncpy(msg.val, argv[++i], MAX_WORD_SIZE);
				}
				msg.cmd = c;
				msgq_send(queue, &msg);
			}
		}
	}
	
	/* Detach from shared memory */
	shmem_detach(h, sizeof(*h));
	
	/* Detach from queue */
	msgq_detach(queue);
	
	/* Detach from semaphores */
	for (i=0; i<BUCKET_COUNT; i++)
		sema_detach(sems[i]);

	return EXIT_SUCCESS;
}
