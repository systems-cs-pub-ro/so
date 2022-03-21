/**
 * SO
 * Lab #5
 *
 * Task #2, Linux
 *
 * Unix implementation of queue functions
 */
#include "generic_queue.h"
#include "utils.h"

/**
 * Create and return a new message queue
 */
msgq_t msgq_create(const char *name)
{
	/* TODO 1 - Init queue */
	msgq_t q = (msgq_t) malloc(sizeof(*q));
	struct mq_attr attr;

	snprintf(q->name, MAX_IPC_NAME, "/%s", name);

	/* TODO 1 - init attr structure */
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(message_t);
	attr.mq_curmsgs = 0;


	/* TODO 1 - Open queue */
	q->mq = mq_open(q->name, O_CREAT | O_RDONLY, 0644, &attr);
	DIE(q->mq == (mqd_t)-1, "mq_open");

	return q;
}

/**
 * Open an existing queue
 */
msgq_t msgq_get(const char *name)
{
	/* TODO 1 - Init queue */
	msgq_t q = (msgq_t) malloc(sizeof(*q));

	snprintf(q->name, MAX_IPC_NAME, "/%s", name);

	/* TODO 1 - Open queue */
	q->mq = mq_open(q->name, O_WRONLY);
	DIE(q->mq == (mqd_t)-1, "mq_open");

	return q;
}

/**
 * Detach from an existing queue
 */
void msgq_detach(msgq_t x)
{
	int rc;

	/* TODO 1 */
	rc = mq_close(x->mq);
	DIE(rc < 0, "close");

	free(x);
}

/**
 * Destroy an existing queue
 */
void msgq_destroy(msgq_t x)
{
	int rc = 0;

	/* TODO 1 */
	rc = mq_close(x->mq);
	DIE(rc < 0, "close");

	rc = mq_unlink(x->name);
	DIE(rc < 0, "unlink");

	free(x);
}

/**
 * Send a message through a queue
 */
void msgq_send(msgq_t m, message_t *x)
{
	int rc;

	/* TODO 1 */
	rc = mq_send(m->mq, (const char *)x, sizeof(message_t), 0);
	DIE(rc < 0, "mq_send");

	dprintf("sending %c %s\n", x->cmd, x->val);
}

/**
 * Receive a message from a queue
 */
void msgq_recv(msgq_t m, message_t *x)
{
	int rc;

	rc = mq_receive(m->mq, (char *)x, sizeof(message_t), NULL);
	DIE(rc < 0, "mq_recv");

	dprintf("received %c %s\n", x->cmd, x->val);
}
