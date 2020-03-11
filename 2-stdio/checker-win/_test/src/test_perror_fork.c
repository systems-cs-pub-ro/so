#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "so_stdio.h"
#include "test_util.h"

#include "hooks.h"

BOOL WINAPI hook_CreateProcess(LPCSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpPsrocessAttributes,
			LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags,
			LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo,
			LPPROCESS_INFORMATION lpProcessInformation);


struct func_hook hooks[] = {
	{ "kernel32.dll", "CreateProcessA", (unsigned long)hook_CreateProcess, 0 },
};


BOOL WINAPI hook_CreateProcess(LPCSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpPsrocessAttributes,
			LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags,
			LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo,
			LPPROCESS_INFORMATION lpProcessInformation)
{
	return FALSE;
}

int main(int argc, char *argv[])
{
	SO_FILE *f;

	install_hooks("so_stdio.dll", hooks, 1);

	f = so_popen("ls", "r");
	FAIL_IF(f != NULL, "popen should have failed: got %p, expected %p\n", f, NULL);

	return 0;
}
