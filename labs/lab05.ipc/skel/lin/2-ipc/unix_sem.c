/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, lin
  *
  * Unix implementation of semaphore functions
  */
#include "generic_sem.h"
#include "utils.h"

/**
 * create a semaphore with an initial value
 */
sema_t sema_create(const char* name, int initial) {
	
	/* Init sema_t structure */
	sema_t s = (sema_t) malloc(sizeof(*s));

	/* Add '/' to semaphore name */
	snprintf(s->name, MAX_IPC_NAME, "/%s", name);

	
	/* TODO 3 - Create semaphore */

	return s;
}

/**
 * open a semaphore
 */
sema_t sema_get(const char* name) {


	/* Init sema_t structure */
	sema_t s = (sema_t) malloc(sizeof(*s));

	/* Add '/' to semaphore name */
	snprintf(s->name, MAX_IPC_NAME, "/%s", name);

	/* TODO 3 - open semaphore */

	return s;
}

/**
 * detach(close) a semaphore - called from client only
 */
void	sema_detach(sema_t x){
	int rc;

	/* TODO 4 - close the semaphore */


}

/**
 * destroy an existing semaphore - called from server only
 */
void   sema_destroy(sema_t x) {
	int rc;

	/* TODO 4 - delete the semaphore*/


}

/**
 * decrement (and wait on) a semaphore
 */
void   sema_wait(sema_t s) {
	int rc;

	/* TODO 4 */


}

/**
 * increment a semaphore
 */
void   sema_signal(sema_t s) {
	int rc;

	/* TODO 4 */


}
