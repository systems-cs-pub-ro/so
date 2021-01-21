/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>

#include "utils.h"

#define	BUFSIZE		16

struct thread_info {
	char name[BUFSIZE];
};

/* Ingredients */
HANDLE Sugar;
HANDLE Milk;
HANDLE Eggs;
HANDLE Flour;

VOID create_ingredients(VOID);
DWORD WINAPI MakeCake(LPVOID lpParameter);
DWORD WINAPI MakeMarshmallows(LPVOID lpParameter);
DWORD WINAPI MakeTiramisu(LPVOID lpParameter);

HANDLE StartThread(LPTHREAD_START_ROUTINE ThreadFunc, LPVOID params)
{
	HANDLE hThread;

	/* TODO - Create Thread to executed ThreadFunc */


	/* TODO - Return Thread Handle */

	return NULL;
}


int main(VOID)
{

	HANDLE ta, tb, tc;
	DWORD dwReturn;

	struct thread_info ti_a, ti_b, ti_c;

	create_ingredients();

	/* Init parameters */
	sprintf_s(ti_a.name, sizeof(ti_a.name), "Chef A");
	sprintf_s(ti_b.name, sizeof(ti_b.name), "Chef B");
	sprintf_s(ti_c.name, sizeof(ti_c.name), "Chef C");

	/* Make food */
	ta = StartThread(MakeCake,			&ti_a);
	tb = StartThread(MakeMarshmallows,	&ti_b);
	tc = StartThread(MakeTiramisu,		&ti_c);

	/* Wait for threads to finish */
	dwReturn = WaitForSingleObject(ta, INFINITE);
	DIE(dwReturn == WAIT_FAILED, "WaitForSingleObject");

	dwReturn = WaitForSingleObject(tb, INFINITE);
	DIE(dwReturn == WAIT_FAILED, "WaitForSingleObject");

	dwReturn = WaitForSingleObject(tc, INFINITE);
	DIE(dwReturn == WAIT_FAILED, "WaitForSingleObject");

	return 0;
}



DWORD WINAPI MakeCake(LPVOID lpParameter)
{
	DWORD dwRet;
	struct thread_info *ti = (struct thread_info *) lpParameter;

	printf("Chef %s wants to make Cake\n", ti->name);

	dwRet = WaitForSingleObject(Sugar, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the sugar\n", ti->name);

	dwRet = WaitForSingleObject(Milk, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the milk\n", ti->name);

	dwRet = WaitForSingleObject(Eggs, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the eggs\n", ti->name);

	printf("Chef %s is making Cake\n", ti->name);
	Sleep(1);

	dwRet = ReleaseSemaphore(Eggs, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");
	dwRet = ReleaseSemaphore(Milk, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");
	dwRet = ReleaseSemaphore(Sugar, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");

	printf("Chef %s finished!!!\n", ti->name);

	return 0;
}


DWORD WINAPI MakeTiramisu(LPVOID lpParameter)
{
	DWORD dwRet;
	struct thread_info *ti = (struct thread_info *) lpParameter;

	printf("Chef %s wants to make tiramisu\n", ti->name);

	dwRet = WaitForSingleObject(Milk, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the milk\n", ti->name);

	dwRet = WaitForSingleObject(Sugar, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the sugar\n", ti->name);

	dwRet = WaitForSingleObject(Eggs, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the eggs\n", ti->name);

	dwRet = WaitForSingleObject(Flour, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the flour\n", ti->name);




	printf("Chef %s is making tiramisu\n", ti->name);
	Sleep(1);

	dwRet = ReleaseSemaphore(Flour, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");
	dwRet = ReleaseSemaphore(Eggs, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");
	dwRet = ReleaseSemaphore(Milk, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");
	dwRet = ReleaseSemaphore(Sugar, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");

	printf("Chef %s finished!!!", ti->name);

	return 0;
}


DWORD WINAPI MakeMarshmallows(LPVOID lpParameter)
{
	DWORD dwRet;
	struct thread_info *ti = (struct thread_info *) lpParameter;

	printf("Chef %s wants to make marshmallows\n", ti->name);

	dwRet = WaitForSingleObject(Flour, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the flour\n", ti->name);
	Sleep(1);

	dwRet = WaitForSingleObject(Milk, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the milk\n", ti->name);

	dwRet = WaitForSingleObject(Eggs, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the eggs\n", ti->name);

	dwRet = WaitForSingleObject(Sugar, INFINITE);
	DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
	printf("Chef %s wants the sugar\n", ti->name);

	printf("Chef %s is making marshmallows\n", ti->name);
	Sleep(1);

	dwRet = ReleaseSemaphore(Eggs, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");
	dwRet = ReleaseSemaphore(Milk, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");
	dwRet = ReleaseSemaphore(Sugar, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");
	dwRet = ReleaseSemaphore(Flour, 1, NULL);
	DIE(dwRet == 0, "ReleaseSemaphore");


	printf("Chef %s finished!!!\n", ti->name);

	return 0;
}



VOID create_ingredients(VOID)
{
	Sugar = CreateSemaphore(NULL, 1, 1, "Sugar");
	DIE(Sugar == NULL, "CreateSemaphore failed");

	Milk = CreateSemaphore(NULL, 1, 1, "Milk");
	DIE(Milk == NULL, "CreateSemaphore failed");

	Eggs = CreateSemaphore(NULL, 1, 1, "Eggs");
	DIE(Eggs == NULL, "CreateSemaphore failed");

	Flour = CreateSemaphore(NULL, 1, 1, "Flour");
	DIE(Flour == NULL, "CreateSemaphore failed");
}
