/**
 * SO
 * Lab IPC
 *
 * Task #2, Windows
 *
 * Generic semaphore functions
 */
#ifndef GENERIC_SEM_H
#define GENERIC_SEM_H		1

#include "common.h"

/**
 * Create a semaphore with an initial value
 */
sema_t sema_create(const char *name, int initial);

/**
 * Open a semaphore
 */
sema_t sema_get(const char *name);

/**
 * Destroy a semaphore
 */
void sema_destroy(sema_t x);

/**
 * Detach from an existing semaphore
 */
void sema_detach(sema_t x);

/**
 * Decrement (and wait on) a semaphore
 */
void sema_wait(sema_t s);

/**
 * Increment a semaphore
 */
void sema_signal(sema_t s);

/**
 * Decrement all semaphores in an array
 */
int sem_array_lock(sema_t *sems, size_t count);

/**
 * Increment all semaphores in an array
 */
int sem_array_unlock(sema_t *sems, size_t count);

#endif
