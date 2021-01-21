/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, win
  *
  * Generic semaphore functions
  */
#ifndef GENERIC_SEM_H
#define GENERIC_SEM_H		1


#include "common.h"

/**
 * create a semaphore with an initial value
 */
sema_t sema_create(const char* name, int initial);

/**
 * open a semaphore
 */
sema_t sema_get(const char* name);

/**
 * destroy a semaphore
 */
void   sema_destroy(sema_t x);

/**
 * detach from an existing semaphore
 */
void	sema_detach(sema_t x);

/**
 * decrement (and wait on) a semaphore
 */
void   sema_wait(sema_t s);

/**
 * increment a semaphore
 */
void   sema_signal(sema_t s);

/**
 * decrement all semaphores in an array
 */
int sem_array_lock(sema_t *sems, size_t count);

/**
 * increment all semaphores in an array
 */
int sem_array_unlock(sema_t *sems, size_t count);


#endif
