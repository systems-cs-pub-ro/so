/**
 * SO
 * Lab #5
 *
 * Task #2, Linux
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
	/* TODO 2 - init shared memory structure */
	shm_t shm = (shm_t) malloc(sizeof(*shm));
	int rc;

	snprintf(shm->name, MAX_IPC_NAME, "/%s", name);

	/* TODO 2 - Create file descriptor of shared memory */
	shm->fd = shm_open(shm->name, O_CREAT | O_RDWR, 0644);
	DIE(shm->fd < 0, "shm_open");

	/* TODO 2 - Set size of shared memory */
	rc = ftruncate(shm->fd, size);
	DIE(rc < 0, "ftruncate");

	return shm;
}

/**
 * Open an existing segment
 */
shm_t shmem_get(const char *name)
{
	/* TODO 2 - init shared memory structure */
	shm_t shm = (shm_t) malloc(sizeof(*shm));

	snprintf(shm->name, MAX_IPC_NAME, "/%s", name);

	/* TODO 2 - Get file descriptor of shared memory */
	shm->fd = shm_open(shm->name, O_RDWR, 0644);
	DIE(shm->fd < 0, "shm_open");

	return shm;
}

/**
 * Destroy a shared mamory segment
 */
void shmem_destroy(shm_t x)
{
	int rc;

	/* TODO 2 */
	rc = close(x->fd);
	DIE(rc < 0, "close");

	rc = shm_unlink(x->name);
	DIE(rc < 0, "shm_unlink");
}

/**
 * Attach a given memory segment
 * @return the address of the mapping
 */
void *shmem_attach(shm_t mem, unsigned int size)
{
	void *p;

	/* TODO 2 */
	p = mmap(NULL, size, PROT_READ|PROT_WRITE,
		 MAP_SHARED, mem->fd, 0);
	DIE(p == MAP_FAILED, "mmap");

	return p;
}

/**
 * Setach the mapping of a segment
 */
void  shmem_detach(const void *p, unsigned int size)
{
	int rc;

	/* TODO 2 */
	rc = munmap((void *)p, size);
	DIE(rc < 0, "munmap");
}
