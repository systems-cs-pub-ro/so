CC = cl
L = link
CFLAGS = /W3 /nologo /I..\utils

all: server.exe client.exe

server.exe: server.obj
	$(L) /nologo $**

server.obj: server.c ..\utils\utils.h
	$(CC) $(CFLAGS) server.c

client.exe: client.obj
	$(L) /nologo $**

client.obj: client.c ..\utils\utils.h
	$(CC) $(CFLAGS) client.c

.IGNORE:
clean:
	del /Q /S /F *.obj server.exe client.exe
