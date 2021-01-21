
#include <string.h>

#include "common.h"
#include "utils.h"

shm_t shmem_create(const char* name, unsigned int size) {
	char shname[255];
	shm_t mem;

	/* TODO 2 - Create file mapping */

	return mem;
}

shm_t shmem_get(const char* name) {
	char shname[255];
	shm_t mem;

	/* TODO 2 - Open file mapping */
	
	return mem;
}

void  shmem_destroy(shm_t x) {
	/* TODO 2 */
}

void* shmem_attach(shm_t mem, unsigned int size) {
	/* TODO 2 */
	void *p = NULL;

	return p;
}

void  shmem_detach(const void *p, unsigned int size) {
	/* TODO 2 */
}

/*
 *	Function called from the server to create a mailslot
 */
msgq_t msgq_create(const char* name) {
	char slot_name[255];
	HANDLE mailslot;

	/* TODO 1 - create mailslot */
	
	return mailslot;
}

/*
 *	Function called from the client to open a mailslot
 */
msgq_t msgq_get(const char* name) {
	char slot_name[255];
	HANDLE hFile;
	
	/* TODO 1 - open existing mailslot */
	
	return hFile;
}

void	msgq_detach(sema_t x){
	/* TODO 1 */
}

void   msgq_destroy(shm_t x) {
	/* TODO 1 */
}

void   msgq_send(msgq_t m, message_t *x) {
	DWORD written=0;
	BOOL bRet;

	/* TODO 1 */
}

void   msgq_recv(msgq_t m, message_t *x) {
	DWORD count=0;
	DWORD bytes_read=0;
	BOOL bRet;
	
	/* TODO 1 */
}



sema_t sema_create(const char* name, int initial) {
	sema_t sem;
	char sema_name[255];

	/* TODO 4 */
	
	return sem;
}

sema_t sema_get(const char* name) {
	sema_t sem;
	char sema_name[255];

	/* TODO 4 */
	
	return sem;
}

void	sema_detach(sema_t x) {
	/* TODO 4 */
}

void   sema_destroy(sema_t x) {
	/* TODO 4 */
}

void   sema_wait(sema_t s) {
	DWORD dwRet;
	
	/* TODO 4 */
}

void   sema_signal(sema_t s) {
	BOOL bRet;

	/* TODO 4 */
}

void milli_sleep(int millis) {
	Sleep(millis);
}
