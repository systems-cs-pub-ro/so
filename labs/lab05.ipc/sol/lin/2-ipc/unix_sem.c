/**
 * SO
 * Lab #5
 *
 * Task #2, Linux
 *
 * Unix implementation of semaphore functions
 */
#include "generic_sem.h"
#include "utils.h"

/**
 * Create a semaphore with an initial value
 */
sema_t sema_create(const char *name, int initial)
{
	/* TODO 4 */
	sema_t s = (sema_t) malloc(sizeof(*s));

	snprintf(s->name, MAX_IPC_NAME, "/%s", name);

	s->sem = sem_open(s->name, O_CREAT, 0644, initial);
	DIE(s->sem == SEM_FAILED, "sem_open");

	return s;
}

/**
 * Open a semaphore
 */
sema_t sema_get(const char *name)
{
	/* TODO 4 */
	sema_t s = (sema_t) malloc(sizeof(*s));

	snprintf(s->name, MAX_IPC_NAME, "/%s", name);

	s->sem = sem_open(s->name, 0);
	DIE(s->sem == SEM_FAILED, "sem_open");

	return s;
}

/**
 * Destroy a semaphore
 */
void sema_detach(sema_t x)
{
	int rc;

	/* TODO 4 */
	rc = sem_close(x->sem);
	DIE(rc < 0, "sem_close");

	free(x);
}

/**
 * Detach from an existing semaphore
 */
void sema_destroy(sema_t x)
{
	int rc;

	/* TODO 4 */
	rc = sem_close(x->sem);
	DIE(rc < 0, "sem_close");

	rc = sem_unlink(x->name);
	DIE(rc < 0, "sem_unlink");

	free(x);
}

/**
 * Decrement (and wait on) a semaphore
 */
void sema_wait(sema_t s)
{
	int rc;

	/* TODO 4 */
	rc = sem_wait(s->sem);
	DIE(rc < 0, "sem_post");
}

/**
 * Increment a semaphore
 */
void sema_signal(sema_t s)
{
	int rc;

	/* TODO 4 */
	rc = sem_post(s->sem);
	DIE(rc < 0, "sem_post");
}
