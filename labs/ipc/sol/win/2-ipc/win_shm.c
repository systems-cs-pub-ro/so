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
shm_t shmem_create(const char *name, unsigned int size) {
	char shname[255];
	shm_t mem;

	/* Init segment name */
	strcpy(shname, "Global\\");
	strcat(shname, name);

	/* TODO 2 - Create file mapping */
	mem = CreateFileMapping(INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE
			0,
			(DWORD)size,
			shname);
	DIE(mem == INVALID_HANDLE_VALUE, "CreateFileMapping");

	return mem;
}

/**
 * Open an existing segment
 */
shm_t shmem_get(const char *name) {
	char shname[255];
	shm_t mem;

	/* Init segment name */
	strcpy(shname, "Global\\");
	strcat(shname, name);

	/* TODO 2 - Open and return existing file mapping */
	mem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, shname);
	DIE(mem == INVALID_HANDLE_VALUE, "OpenFileMapping");

	return mem;
}

/**
 * Attach a given memory segment
 * @return the address of the mapping
 */
void *shmem_attach(shm_t mem, unsigned int size) {
	void *p = NULL;

	/**
	 * TODO 2 - Get memory pointer from the shared memory
	 *
	 * Hint: use MapViewOfFile
	 */
	p = MapViewOfFile(mem, FILE_MAP_ALL_ACCESS, 0, 0, size);
	DIE(p == NULL, "MapViewOfFile");

	return p;
}

/**
 * Destroy a shared mamory segment - only called from server
 */
void shmem_destroy(shm_t x) {
	/**
	 * TODO 2 - close the shared memory segment
	 *
	 * Hint: what is shm_t? check common.h
	 */
	CloseHandle(x);
}

/**
 * Unmap a shared memory segment
 */
void shmem_detach(const void *p, unsigned int size) {

	/* TODO 2 - unmap the shared memory from address space */
	BOOL bRet = UnmapViewOfFile(p);
	DIE(bRet == FALSE, "UnmapViewofFile");
}


