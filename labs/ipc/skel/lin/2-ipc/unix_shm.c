/**
 * SO
 * Lab IPC
 *
 * Task #2
 *
 * Unix implementation of shared memory functions
 */
#include "generic_shm.h"
#include "utils.h"

/**
 * Create and return a segment
 */
shm_t shmem_create(const char* name, unsigned int size) {
	int rc;

	/* Init shared memory structure */
	shm_t shm = (shm_t) malloc(sizeof(*shm));

	/**
	 * Save shm name in shm->name
	 * Shared memory name must begin with a /
	 */
	snprintf(shm->name, MAX_IPC_NAME, "/%s", name);

	/* TODO 2 - Create shared memory and save the file descriptor */

	/* TODO 2 - Set size of shared memory */

	return shm;
}

/**
 * Open an existing segment
 */
shm_t shmem_get(const char* name) {
	/* Init shared memory structure */
	shm_t shm = (shm_t) malloc(sizeof(*shm));

	/**
	 * Save shm name in shm->name
	 * Shared memory name must begin with a /
	 */
	snprintf(shm->name, MAX_IPC_NAME, "/%s", name);

	/* TODO 2 - Open the shared memory area */

	return shm;
}

/**
 * Attach a given memory segment
 * @return the address of the mapping
 */
void* shmem_attach(shm_t mem, unsigned int size) {
	void *p = NULL;

	/**
	 * TODO 2 - Get memory pointer from the share memory
	 * Hint: use mmap
	 */

	return p;
}

/**
 * Destroy a shared mamory segment - only called from server
 */
void shmem_destroy(shm_t x) {
	int rc;

	/* TODO 2 - Delete the shared memory segment */

}

/**
 * Unmap a shared memory segment
 */
void shmem_detach(const void *p, unsigned int size) {
	int rc;

	/* TODO 2 - unmap the shared memory from address space */

}
