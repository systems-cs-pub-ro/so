/*
 * Executable Parser Implementation
 *
 * 2018, Operating Systems
 */

#include <Windows.h>
#include <Winternl.h>
#include <stdio.h>

#include "exec_parser.h"

static BOOL SolveImports(uintptr_t loadBase, IMAGE_NT_HEADERS * peHdr);

static int readHeader(HANDLE hFile, DWORD offset, char *buf, DWORD size)
{
	DWORD bytesRead, totalBytesRead;
	BOOL bRet;

	if (SetFilePointer(hFile, offset, NULL, FILE_BEGIN) ==
			INVALID_SET_FILE_POINTER) {
		fprintf(stderr, "Couldn't change offset: %d\n", GetLastError());
		goto error;
	}

	totalBytesRead = 0;
	do {
		bRet = ReadFile(hFile, buf + totalBytesRead,
			size - totalBytesRead, &bytesRead, NULL);
		if (bRet == FALSE) {
			fprintf(stderr, "Couldn't read enough bytes\n");
			goto error;
		}
		totalBytesRead += bytesRead;
	} while (totalBytesRead < size);

	return 0;

error:
	CloseHandle(hFile);
	return -1;
}

so_exec_t *so_parse_exec(char *path)
{
	so_exec_t *exec = NULL;
	IMAGE_DOS_HEADER dosHdr;
	IMAGE_NT_HEADERS peHdr;
	IMAGE_SECTION_HEADER *secHdr;
	int i, nr_sections;
	so_seg_t *seg;
	HANDLE hFile;

	hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Couldn't open file %s: %d\n",
				path, GetLastError());
		return NULL;
	}

	if (readHeader(hFile, 0, (char *)&dosHdr,
			sizeof(IMAGE_DOS_HEADER)) < 0) {
		fprintf(stderr, "Couldn't read PE DOS header\n");
		goto out;
	}

	if (dosHdr.e_magic != 0x5a4d) {
		fprintf(stderr, "Couldn't find the DOS magic number\n");
		goto out;
	}

	if (readHeader(hFile, dosHdr.e_lfanew,
			(char *)&peHdr, sizeof(IMAGE_NT_HEADERS)) < 0) {
		fprintf(stderr, "Couldn't read PE DOS header\n");
		goto out;
	}

	if (peHdr.Signature != 0x4550) {
		fprintf(stderr, "Couldn't find the PE magic number\n");
		goto out;
	}

	nr_sections = peHdr.FileHeader.NumberOfSections;
	secHdr = malloc(nr_sections * sizeof(IMAGE_SECTION_HEADER));
	if (!secHdr) {
		fprintf(stderr, "Couldn't allocate space for sections\n");
		goto out;
	}

	if (readHeader(hFile, dosHdr.e_lfanew + sizeof(IMAGE_NT_HEADERS),
			(char *)secHdr, nr_sections *
			sizeof(IMAGE_SECTION_HEADER)) < 0) {
		fprintf(stderr, "Couldn't read PE DOS header\n");
		goto out_free;
	}

	exec = (so_exec_t *)malloc(sizeof(*exec));
	if (!exec) {
		fprintf(stderr, "Out of memory\n");
		goto out_free;
	}

	exec->base_addr = peHdr.OptionalHeader.ImageBase;
	exec->entry = peHdr.OptionalHeader.ImageBase +
		peHdr.OptionalHeader.AddressOfEntryPoint;
	exec->segments_no = peHdr.FileHeader.NumberOfSections +
		1 /* an extra segment for headers */;
	exec->segments =
		(so_seg_t *)malloc(exec->segments_no * sizeof(so_seg_t));

	i =  0;
	for (i = 0; i < peHdr.FileHeader.NumberOfSections; i++) {
		IMAGE_SECTION_HEADER *sec = &secHdr[i];

		seg = &exec->segments[i];
		seg->vaddr = peHdr.OptionalHeader.ImageBase +
			sec->VirtualAddress;
		seg->offset = sec->PointerToRawData;
		seg->file_size = sec->SizeOfRawData;
		seg->mem_size = sec->Misc.VirtualSize;
		seg->data = 0;

		seg->perm = 0;
		if (sec->Characteristics & IMAGE_SCN_MEM_READ)
			seg->perm |= PERM_R;
		if (sec->Characteristics & IMAGE_SCN_MEM_WRITE)
			seg->perm |= PERM_W;
		if (sec->Characteristics & IMAGE_SCN_MEM_EXECUTE)
			seg->perm |= PERM_X;
	}

	/* last segment is the header */
	seg = &exec->segments[i++];
	seg->vaddr = exec->base_addr;
	seg->offset = 0;
	seg->file_size = peHdr.OptionalHeader.SizeOfHeaders;
	seg->mem_size = peHdr.OptionalHeader.SizeOfHeaders;
	seg->data = 0;
	seg->perm = PERM_R;

out_free:
	free(secHdr);
out:
	CloseHandle(hFile);

	return exec;
}

void so_start_exec(so_exec_t *exec, char *argv[])
{
	IMAGE_NT_HEADERS *peHdr = (IMAGE_NT_HEADERS *)
		(((IMAGE_DOS_HEADER *)(exec->base_addr))->e_lfanew +
		 (DWORD)(exec->base_addr));
	SolveImports(exec->base_addr, peHdr);
	((void(*)(void))exec->entry)();
}

static BOOL SolveImports(uintptr_t loadBase, IMAGE_NT_HEADERS *peHdr)
{
	IMAGE_DATA_DIRECTORY *dir =
	&peHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	DWORD *oft;
	DWORD *ft;
	IMAGE_IMPORT_BY_NAME *impName;
	DWORD impOrd;
	HANDLE hModule;
	char *dllName;
	DWORD funcAddr;
	IMAGE_IMPORT_DESCRIPTOR *impDesc;

	if (!dir->Size)
		return TRUE;

	impDesc = (IMAGE_IMPORT_DESCRIPTOR *)(loadBase + dir->VirtualAddress);
	while (impDesc->OriginalFirstThunk) {
		dllName = (char *)((DWORD)loadBase + impDesc->Name);

		hModule = LoadLibrary(dllName);
		if (!hModule) {
			fprintf(stderr, "Failed to load %s: %d\n",
					dllName, GetLastError());
			return FALSE;
		}

		oft = (DWORD *)((DWORD)loadBase + impDesc->OriginalFirstThunk);
		ft = (DWORD *)((DWORD)loadBase + impDesc->FirstThunk);
		for (; *oft; oft++, ft++) {
			if (*oft & 0x80000000) {
				impOrd = *oft;
				impOrd &= 0xffff;
				funcAddr = (DWORD)
					GetProcAddress(hModule, (LPCSTR)impOrd);
				if (!funcAddr) {
					fprintf(stderr,
							"Couldn't find the function %d: %d\n",
							impOrd, GetLastError());
					return FALSE;
				}
				*ft = funcAddr;
			} else {
				impName = (IMAGE_IMPORT_BY_NAME *)
					((DWORD)loadBase + *oft);
				funcAddr = (DWORD)
					GetProcAddress(hModule, impName->Name);
				if (!funcAddr) {
					fprintf(stderr,
							"Couldn't find the function %s: %d\n",
							impName->Name,
							GetLastError());
					return FALSE;
				}
				*ft = funcAddr;
			}
		}

		impDesc++;
	}

	return TRUE;
}
