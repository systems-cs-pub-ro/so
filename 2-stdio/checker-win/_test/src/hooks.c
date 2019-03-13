#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include "hooks.h"

static char *GetImportByOrdName(HANDLE hModule, DWORD impOrd)
{
	IMAGE_DOS_HEADER *dosHdr;
	IMAGE_NT_HEADERS *peHdr;
	ULONG loadBase;
	IMAGE_DATA_DIRECTORY *dir;
	IMAGE_EXPORT_DIRECTORY *expDir;
	DWORD i;
	SHORT *AddressOfNameOrdinals;
	DWORD *AddressOfNames;

	dosHdr = (IMAGE_DOS_HEADER *)hModule;
	loadBase = (ULONG)dosHdr;

	peHdr = (IMAGE_NT_HEADERS *)(loadBase + dosHdr->e_lfanew);
	dir = &peHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

	expDir = (IMAGE_EXPORT_DIRECTORY *)(loadBase + dir->VirtualAddress);

	AddressOfNameOrdinals = (SHORT *)(loadBase + expDir->AddressOfNameOrdinals);
	AddressOfNames = (DWORD *)(loadBase + expDir->AddressOfNames);

	for (i = 0; i < expDir->NumberOfNames; i++) {

		if (AddressOfNameOrdinals[i] == (impOrd - expDir->Base))
			return (char *)(loadBase + AddressOfNames[i]);
	}

	return NULL;
}

static void PatchAddr(DWORD_PTR *addr, DWORD_PTR val)
{
	BOOL bRet;
	SYSTEM_INFO si;
	unsigned int pageSize;
	DWORD_PTR addr_align;
	DWORD oldProtect;
	DWORD dummy;
	MEMORY_BASIC_INFORMATION mbi;

	VirtualQuery(addr, &mbi, sizeof(mbi));

	bRet = VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &oldProtect);
	if (!bRet) {
		printf("VirtualProtect failed: %d\n", GetLastError());
		return;
	}

	*addr = val;

	bRet = VirtualProtect(mbi.BaseAddress, mbi.RegionSize, oldProtect, &dummy);
	if (!bRet) {
		printf("VirtualProtect failed: %d\n", GetLastError());
		return;
	}
}

int install_hooks(char *target_lib_name, struct func_hook *hooks, int num)
{
	IMAGE_DOS_HEADER *dosHdr;
	IMAGE_NT_HEADERS *peHdr;
	IMAGE_DATA_DIRECTORY *dir;
	DWORD *oft;
	DWORD *ft;
	IMAGE_IMPORT_BY_NAME *impName;
	DWORD impOrd;
	HANDLE hModule;
	char *dllName;
	char *func_name;
	IMAGE_IMPORT_DESCRIPTOR *impDesc;
	ULONG_PTR loadBase;
	int i;

	dosHdr = (IMAGE_DOS_HEADER *)GetModuleHandle(target_lib_name);
	if (!dosHdr)
		return FALSE;

	loadBase = (ULONG)dosHdr;

	peHdr = (IMAGE_NT_HEADERS *)(loadBase + dosHdr->e_lfanew);

	dir = &peHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	for (i = 0; i < num; i++) {
		impDesc = (IMAGE_IMPORT_DESCRIPTOR *)(loadBase + dir->VirtualAddress);

		while(impDesc->OriginalFirstThunk) {
			dllName = (char *)(loadBase + impDesc->Name);

			if (!_stricmp(dllName, hooks[i].lib_name)) {

				hModule = GetModuleHandle(dllName);
				if (!hModule) {
					fprintf(stderr, "Couldn't get handle to %s: %d\n", dllName, GetLastError());
					return FALSE;
				}

				oft = (DWORD *)(loadBase + impDesc->OriginalFirstThunk);
				ft = (DWORD *)(loadBase + impDesc->FirstThunk);
				for (; *oft; oft++, ft++) {
					if (*oft & 0x80000000) {
						impOrd = *oft;
						impOrd &= 0xffff;

						func_name = GetImportByOrdName(hModule, impOrd);

						if (func_name && !_stricmp(func_name, hooks[i].func_name)) {
							hooks[i].orig_addr = (unsigned long)GetProcAddress(hModule, hooks[i].func_name);
							PatchAddr(ft, hooks[i].addr);
						}
					} else {
						impName = (IMAGE_IMPORT_BY_NAME *)(loadBase + *oft);
						func_name = (char *)impName->Name;

						if (!_stricmp(func_name, hooks[i].func_name)) {
							hooks[i].orig_addr = (unsigned long)GetProcAddress(hModule, hooks[i].func_name);
							PatchAddr(ft, hooks[i].addr);
						}
					}
				}
			}

			impDesc++;
		}
	}
	return 0;
}
