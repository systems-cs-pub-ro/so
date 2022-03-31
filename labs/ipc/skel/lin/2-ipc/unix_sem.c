/**
 * SO
 * Lab IPC
 *
 * Task #2
 *
 * Unix implementation of semaphore functions
 */
#include "generic_sem.h"
#include "utils.h"

/**
 * Create a semaphore with an initial value
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
 * Open a semaphore
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
 * Detach(close) a semaphore - called from client only
 */
void sema_detach(sema_t x){
	int rc;

	/* TODO 4 - close the semaphore */

}

/**
 * Destroy an existing semaphore - called from server only
 */
void sema_destroy(sema_t x) {
	int rc;

	/* TODO 4 - delete the semaphore*/

}

/**
 * Decrement (and wait on) a semaphore
 */
void sema_wait(sema_t s) {
	int rc;

	/* TODO 4 */

}

/**
 * Increment a semaphore
 */
void sema_signal(sema_t s) {
	int rc;

	/* TODO 4 */

}
