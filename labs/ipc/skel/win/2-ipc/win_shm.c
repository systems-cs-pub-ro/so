/**
 * SO
 * Lab IPC
 *
 * Task #2, Windows
 *
 * Unix implementation of shared memory functions
 */
#include "generic_shm.h"
#include "utils.h"

/**
 * Create and return a segment
 */
shm_t shmem_create(const char *name, unsigned int size)
{
	char shname[255];
	shm_t mem = INVALID_HANDLE_VALUE;

	/* Init segment name */
	strcpy(shname, "Global\\");
	strcat(shname, name);

	/* TODO 2 - Create file mapping */

	return mem;
}

/**
 * Open an existing segment
 */
shm_t shmem_get(const char *name)
{
	char shname[255];
	shm_t mem = INVALID_HANDLE_VALUE;

	/* Init segment name */
	strcpy(shname, "Global\\");
	strcat(shname, name);

	/* TODO 2 - Open and return existing file mapping */

	return mem;
}

/**
 * Aattach a given memory segment
 * @return the address of the mapping
 */
void *shmem_attach(shm_t mem, unsigned int size)
{
	void *p = NULL;

	/**
	 * TODO 2 - Get memory pointer from the shared memory
	 *
	 * Hint: use MapViewOfFile
	 */

	return p;
}

/**
 * Destroy a shared mamory segment - only called from server
 */
void shmem_destroy(shm_t x)
{
	/**
	 * TODO 2 - close the shared memory segment
	 *
	 * Hint: what is shm_t? check common.h
	 */

}

/**
 * Unmap a shared memory segment
 */
void shmem_detach(const void *p, unsigned int size)
{
	/* TODO 2 - unmap the shared memory from address space */
}
