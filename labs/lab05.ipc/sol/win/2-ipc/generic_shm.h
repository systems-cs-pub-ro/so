/**
 * SO
 * Lab #5
 *
 * Task #2, Windows
 *
 * Generic shared memory functions
 */
#ifndef GENERIC_SHM_H
#define GENERIC_SHM_H		1

#include "common.h"

/**
 * Create and return a segment
 */
shm_t shmem_create(const char *name, unsigned int size);

/**
 * Open an existing segment
 */
shm_t shmem_get(const char *name);

/**
 * Destroy a shared mamory segment
 */
void shmem_destroy(shm_t x);

/**
 * Attach a given memory segment
 * @return the address of the mapping
 */
void *shmem_attach(shm_t mem, unsigned int size);

/**
 * Detach the mapping of a segment
 */
void shmem_detach(const void *p, unsigned int size);

#endif
