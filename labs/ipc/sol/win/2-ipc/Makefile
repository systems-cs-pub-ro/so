CFLAGS = /nologo /I. /W3 /D_CRT_SECURE_NO_DEPRECATE /DWIN32 /EHsc -I../utils
LDFLAGS = /nologo

WIN_IPC_OBJ = win_queue.obj win_shm.obj win_sem.obj

all: build

build: client.exe server.exe

client.exe: client.obj hash.obj hashtable.obj $(WIN_IPC_OBJ)
	link $(LDFLAGS) /OUT:$@ $**
	
server.exe: server.obj hash.obj hashtable.obj $(WIN_IPC_OBJ)
	link $(LDFLAGS) /OUT:$@ $**

clean:
	del *.obj *.exe

