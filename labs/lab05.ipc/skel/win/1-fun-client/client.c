#undef UNICODE
#undef _UNICODE

#include <windows.h>
#include <stdio.h>
#include "utils.h"
 
LPSTR lpFormat= "\\\\%s\\mailslot\\sample_mailslot";

int main(void)
{
	HANDLE hMailslot;
	BOOL bRet;
	DWORD cbWritten, dwRet;
	LPSTR lpBuffer[256];
	LPSTR lpMailslotName[128];
	LPSTR lpServerName[32];

	/* Get server name to connect */
	printf("Server name to connect: ");
	scanf_s("%s", lpServerName, sizeof(lpServerName));

	sprintf_s((char *)lpMailslotName, sizeof(lpMailslotName), lpFormat, lpServerName);
	printf("Connecting to %s\n", lpMailslotName);

	/* Open Mailslot */
	hMailslot = CreateFile(
		(LPCTSTR)lpMailslotName,
 		GENERIC_WRITE,  
 		FILE_SHARE_READ,
 		NULL,		
 		OPEN_EXISTING,  
 		FILE_ATTRIBUTE_NORMAL,
 		NULL);		
	DIE(hMailslot == INVALID_HANDLE_VALUE, "CreateFile");
 
	while (1){
		
		printf("Message to be send:");
		scanf_s("%s", lpBuffer, sizeof(lpBuffer));

		/* Send message */
		bRet = WriteFile(
			hMailslot,
 			lpBuffer,
 			(DWORD) lstrlen((LPCTSTR)lpBuffer) + 1,
 			&cbWritten,
 			NULL);
		DIE(bRet == FALSE, "Write file to Mailslot");
	}
 
	/* Close Mailslot */
	dwRet = CloseHandle(hMailslot);
	DIE (dwRet == FALSE, "CloseHandle");
 
	return 0;
}

