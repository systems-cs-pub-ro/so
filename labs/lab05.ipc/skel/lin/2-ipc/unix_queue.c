/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, lin
  *
  * Unix implementation of queue functions
  */
#include "generic_queue.h"
#include "utils.h"

/**
 * create and return a new message queue - called from the server
 */
msgq_t msgq_create(const char *name) {

	/* Init queue */
	msgq_t q = (msgq_t) malloc(sizeof(*q));
	struct mq_attr attr;

	/* Add '/' to queue name and save it */
	snprintf(q->name, MAX_IPC_NAME, "/%s", name);

	/* TODO 1 - init attr structure 
         * Watch out for mq_maxmsg and mq_msgsize !
         * Hint: man mq_setattr
         * Hint: use MQ_MAXMSG macro
	 */


	/* TODO 1 - Create queue */

	return q;
}

/**
 * open an existing queue - called from the client
 */
msgq_t msgq_get(const char* name) {

	/* Init queue */
	msgq_t q = (msgq_t) malloc(sizeof(*q));

	/* Add '/' to queue name and save it */
	snprintf(q->name, MAX_IPC_NAME, "/%s", name);

	/* TODO 1 - Open queue for sending messages */

	return q;
}

/**
 * detach(close) from an existing queue - called from the client
 */
void   msgq_detach(msgq_t x) {
	int rc;

	/* TODO 1 - close queue*/


}


/**
 * destroy an existing queue - called from the server
 */
void   msgq_destroy(msgq_t x) {
	int rc = 0;

	/* TODO 1 - delete queue */


}

/**
 * send a message through a queue
 *
 * The message is allready set in the 'message_t' structure
 * It only needs to be send
 */
void   msgq_send(msgq_t m, message_t *msg) {
	int rc;

	/* TODO 1 - send the message contained in 'msq' */

	/* Log actions */
	dprintf("sending %c %s \n", msg->cmd, msg->val);
}

/**
 * receive a message from queue
 */
void   msgq_recv(msgq_t m, message_t *msg) {
	int rc;

	/* TODO 1 - receive message in parameter 'msg' */

	/* Log actions - uncomment when TODO 1 is ready*/
	//dprintf("received %c %s \n", msg->cmd, msg->val);
}


