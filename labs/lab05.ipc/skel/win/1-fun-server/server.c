#undef UNICODE
#undef _UNICODE

#include <windows.h>
#include "utils.h"
 
/* where to create the mailslot */
LPSTR lpszSlotName = "\\\\.\\mailslot\\sample_mailslot";

 
int main(void)
{
	DWORD cbMessage, cMessage, cbRead, dwRet; 
	HANDLE hMailslot;
	BOOL bRet;
	LPSTR lpszBuffer;

	/* Create Mailslot */
	hMailslot = CreateMailslot(
		lpszSlotName,
		0,                     /* no maximum message size */
		MAILSLOT_WAIT_FOREVER, /* no expiration period */
		NULL);                 /* no security attributes */
	DIE(hMailslot == INVALID_HANDLE_VALUE, "CreateMailSlot");
 
	printf("Waiting for messages form clients...\n");
 
	/* Read all messages from Mailslot */
	while (1) {
 		
		/* Get number of messages form Mailslot */
		bRet = GetMailslotInfo( 
			hMailslot,             /* mailslot handle */
			(LPDWORD) NULL,        /* no maximum message size */
			&cbMessage,            /* size of next message */
			&cMessage,             /* number of messages */
			(LPDWORD) NULL);       /* no read time-out */
		DIE(bRet == FALSE, "GetMailslotInfo");

		if (cMessage == 0) {
			/* Wait for a new message */
			Sleep(1000);
			continue;
		}

		/* Alocate space for the message */
 		lpszBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbMessage);
		DIE(lpszBuffer == NULL, "HeapAlloc");
 
		/* Get the actual message */
 		bRet = ReadFile(
			hMailslot,
 			lpszBuffer,
 			cbMessage,
 			&cbRead,
 			(LPOVERLAPPED) NULL);
		DIE(bRet == FALSE, "ReadFile from Mailslot");
 
 		printf("Received: %s\n", lpszBuffer);
 
 		HeapFree(GetProcessHeap(), 0, lpszBuffer);
 	} /* end while */
 
	dwRet = CloseHandle(hMailslot);
	DIE (dwRet == FALSE, "CloseHandle");
 
	return 0;
}

