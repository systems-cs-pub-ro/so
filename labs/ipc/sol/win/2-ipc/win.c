/**
 * SO
 * Lab IPC
 *
 * Task #2, Windwos
 *
 */
#include <string.h>

#include "common.h"
#include "utils.h"

shm_t shmem_create(const char *name, unsigned int size) {
	char shname[255];
	shm_t mem;

	/* TODO 2 - Create file mapping */
	strcpy(shname, "Global\\");
	strcat(shname, name);

	mem = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
								0, (DWORD)size, shname);
	DIE(mem == INVALID_HANDLE_VALUE, "CreateFileMapping");

	return mem;
}

shm_t shmem_get(const char *name) {
	char shname[255];
	shm_t mem;

	/* TODO 2 - Open file mapping */
	strcpy(shname, "Global\\");
	strcat(shname, name);

	mem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, shname);
	DIE(mem == INVALID_HANDLE_VALUE, "OpenFileMapping");

	return mem;
}

void shmem_destroy(shm_t x) {
	/* TODO 2 */
	CloseHandle(x);
}

void *shmem_attach(shm_t mem, unsigned int size) {
	/* TODO 2 */
	void *p = MapViewOfFile(mem, FILE_MAP_ALL_ACCESS, 0, 0, size);
	DIE(p == NULL, "MapViewOfFile");
}

void shmem_detach(const void *p, unsigned int size) {
	BOOL bRet = UnmapViewOfFile(p);
	DIE(bRet == FALSE, "UnmapViewofFile");
}

/**
 * Function called from the server to create a mailslot
 */
msgq_t msgq_create(const char* name) {
	char slot_name[255];
	HANDLE mailslot;

	/* TODO 1 - create mailslot */
	strcpy(slot_name, "\\\\.\\mailslot\\");
	strcat(slot_name, name);

	mailslot = CreateMailslot(slot_name, 0, MAILSLOT_WAIT_FOREVER, NULL);
	DIE(mailslot == INVALID_HANDLE_VALUE, "CreateMailSlot");

	return mailslot;
}

/**
 * Function called from the client to open a mailslot
 */
msgq_t msgq_get(const char* name) {
	char slot_name[255];
	HANDLE hFile;

	/* TODO 1 - open existing mailslot */
	strcpy(slot_name, "\\\\.\\mailslot\\");
	strcat(slot_name, name);

	hFile = CreateFile(slot_name, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DIE(hFile == INVALID_HANDLE_VALUE,"CreateFile");

	return hFile;
}

void msgq_detach(sema_t x){
	/* TODO 1 */
	CloseHandle(x);
}

void msgq_destroy(shm_t x) {
	/* TODO 1 */
	CloseHandle(x);
}

void msgq_send(msgq_t m, message_t *x) {
	DWORD written=0;
	BOOL bRet;

	bRet = WriteFile(m, x, sizeof(message_t), &written, NULL);
	DIE(bRet == FALSE, "WriteFile");
}

void msgq_recv(msgq_t m, message_t *x) {
	DWORD count=0;
	DWORD bytes_read=0;
	BOOL bRet;

	bRet = ReadFile(m, x, sizeof(message_t), &bytes_read, NULL);
	DIE(bRet == FALSE, "WriteFile");
}

sema_t sema_create(const char* name, int initial) {
	sema_t sem;
	char sema_name[255];

	/* TODO 4 */
	strcpy(sema_name, "Global\\");
	strcat(sema_name, name);

	sem = CreateSemaphore(NULL, initial, 0xFFFF, sema_name);
	DIE(sem == INVALID_HANDLE_VALUE, "CreateSemaphore");

	return sem;
}

sema_t sema_get(const char *name) {
	sema_t sem;
	char sema_name[255];

	/* TODO 4 */
	strcpy(sema_name, "Global\\");
	strcat(sema_name, name);

	sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, sema_name);
	DIE(sem == INVALID_HANDLE_VALUE, "OpenSemaphore");

	return sem;
}

void sema_detach(sema_t x) {
	/* TODO 4 */
	CloseHandle(x);
}

void sema_destroy(sema_t x) {
	/* TODO 4 */
	CloseHandle(x);
}

void sema_wait(sema_t s) {
	DWORD dwRet;

	/* TODO 4 */
	dwRet = WaitForSingleObject(s, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
}

void sema_signal(sema_t s) {
	BOOL bRet;

	/* TODO 4 */
	bRet = ReleaseSemaphore(s, 1, NULL);
	DIE(bRet == FALSE, "ReleaseSemaphore");
}

void milli_sleep(int millis) {
	Sleep(millis);
}
