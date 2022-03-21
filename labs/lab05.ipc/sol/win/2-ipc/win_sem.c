/**
  * SO, 2011
  * Lab #5
  *
  * Task #2, lin
  *
  * Win implementation of semaphore functions
  */
#include "generic_sem.h"
#include "utils.h"


/**
 * create a semaphore with an initial value
 */
sema_t sema_create(const char* name, int initial) {
	sema_t sem;
	char sema_name[255];

	/* Init semaphore name */
	strcpy(sema_name, "Global\\");
	strcat(sema_name, name);

	/* TODO 3 - Create semaphore with initial value 'initial' */
	sem = CreateSemaphore(NULL, initial, 0xFFFF, sema_name);
	DIE(sem == INVALID_HANDLE_VALUE, "CreateSemaphore");

	return sem;
}

/**
 * open a semaphore
 */
sema_t sema_get(const char* name) {
	sema_t sem;
	char sema_name[255];

	/* Init semaphore name */
	strcpy(sema_name, "Global\\");
	strcat(sema_name, name);

	/* TODO 3 - Open existing named semaphore */
	sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, sema_name);
	DIE(sem == INVALID_HANDLE_VALUE, "OpenSemaphore");

	return sem;
}

/**
 * detach(close) a semaphore - called from client
 */
void	sema_detach(sema_t x) {

	/* TODO 4 - close the semaphore 
	 *
	 * Hint: what is sema_t? check common.h
	 */
	CloseHandle(x);
}

/**
 * destroy an existing semaphore - called from server only
 */
void   sema_destroy(sema_t x) {

	/* TODO 4 - delete the semaphore*/
	CloseHandle(x);
}


/**
 * decrement (and wait on) a semaphore
 */
void   sema_wait(sema_t s) {
	DWORD dwRet;
	
	/* TODO 4 - wait indefinitely to get the semaphore */
	dwRet = WaitForSingleObject(s, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
}

/**
 * increment (and release) a semaphore
 */
void   sema_signal(sema_t s) {
	BOOL bRet;

	/* TODO 4 */
	bRet = ReleaseSemaphore(s, 1, NULL);
	DIE(bRet == FALSE, "ReleaseSemaphore");
}

