/**
 * SO, 2017
 * Lab #3, Procese
 *
 * Task #1, Windows
 *
 * "Fork bomb" on windows
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int main(void)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL bRet;

	printf("Pid=%d\n", GetCurrentProcessId());

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	while (1) {
		bRet = CreateProcess(NULL, "1-bomb.exe", NULL, NULL, FALSE, 0,
				NULL, NULL, &si, &pi);
		DIE(bRet == FALSE, "Create Process");
	}

	return 0;
}
