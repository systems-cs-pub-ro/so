/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, lin
  *
  * Generic server implementation
  */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "generic_queue.h"
#include "generic_shm.h"
#include "generic_sem.h"
#include "hashtable.h"



static sema_t sems[BUCKET_COUNT];
static shm_t shmem;
static msgq_t queue;
static struct htable *h;

int main(void)
{
	int i;
	char name[MAX_IPC_NAME];
	message_t msg;

	/* Create semaphores */
	for (i = 0; i < BUCKET_COUNT; i++) {
		snprintf(name, MAX_IPC_NAME, "%s%d", BASE_SEMA_NAME, i);
		sems[i] = sema_create(name, 1);
	}

	/* Create message queue */
	queue = msgq_create(BASE_QUEUE_NAME);
	
	/* Create shared memory */
	shmem = shmem_create(BASE_SHM_NAME, sizeof(*h));
	h = shmem_attach(shmem, sizeof(*h));

	/* no need for locking here */
	htable_clear(h, NULL);

	/* Staisfy client requests */
	for (;;) {
		msgq_recv(queue, &msg);

		if (msg.cmd == 'e')
			break;
		
		switch (msg.cmd) {
			case 'a': {
				sema_wait(sems[hash(msg.val)]);
				htable_insert(h, msg.val);
				sema_signal(sems[hash(msg.val)]);
				break;
			}
			case 'r': {
				sema_wait(sems[hash(msg.val)]);
				htable_delete(h, msg.val);
				sema_signal(sems[hash(msg.val)]);
				break;
			}
			case 'c': {
				htable_clear(h, sems);
			}
		}
	}
	
	/* Close shared memory */
	shmem_detach(h, sizeof(*h));
	shmem_destroy(shmem);
	
	/* Close queue */
	msgq_destroy(queue);
	
	/* Close semaphores */
	for (i=0; i<BUCKET_COUNT; i++)
		sema_destroy(sems[i]);
	
	return EXIT_FAILURE;
}
