/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, lin
  *
  * Win implementation of queue functions
  */

#include "generic_queue.h"
#include "utils.h"

/**
 * create and return a new mailslot - called from server
 */
msgq_t msgq_create(const char* name) {
	char slot_name[255];
	msgq_t mailslot = INVALID_HANDLE_VALUE;

	/* Init mailslot proper name */
	strcpy(slot_name, "\\\\.\\mailslot\\");
	strcat(slot_name, name);

	/* TODO 1 - create mailslot */

	return mailslot;
}

/*
 *  Open existing mailslot - called from client
 */
msgq_t msgq_get(const char* name) {
	char slot_name[255];
	msgq_t mailslot = INVALID_HANDLE_VALUE;
	
	/* Init mailslot proper name */	
	strcpy(slot_name, "\\\\.\\mailslot\\");
	strcat(slot_name, name);

	/* TODO 1 - open existing mailslot 
	 *
	 * Hint: open a mailslot like a regular file
	 */


	return mailslot;
}

/**
 * detach from an existing queue
 */
void	msgq_detach(msgq_t  x){

	/* TODO 1 */
	
}

/**
 * destroy an existing queue
 */
void   msgq_destroy(msgq_t  x) {

	/* TODO 1 */
	
}


/**
 * send a message through the mailslot
 *
 * The message is allready set in the 'message_t' structure
 * It only needs to be send
 */
void   msgq_send(msgq_t m, message_t *msg) {
	DWORD written=0;
	BOOL bRet;

	/* TODO 1 - send the message contained in 'msq' 
	 *
	 * Hint: Think of the mailslot as a regular file
	 */


	/* Log actions */
	dprintf("sending %c %s \n", msg->cmd, msg->val);
}

/**
 * receive a message from queue
 */
void   msgq_recv(msgq_t m, message_t *msg) {
	DWORD bytes_read=0;
	BOOL bRet;
	
	/* TODO 1 - receive message in parameter 'msg' 
	 *
	 * Hint: Think of the mailslot as a regular file
	 */

	/* Log actions - uncomment when TODO 1 is ready*/
	//dprintf("received %c %s \n", msg->cmd, msg->val);
}

