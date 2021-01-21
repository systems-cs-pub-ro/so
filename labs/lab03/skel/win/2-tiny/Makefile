CC = cl
L = link
CFLAGS = /W3 /nologo /I..\utils

all: tiny.exe

tiny.exe: tiny.obj
	$(L) /nologo $**

tiny.obj: tiny.c ..\utils\utils.h
	$(CC) $(CFLAGS) tiny.c

.IGNORE:
clean:
	del /Q /S /F *.obj tiny.exe
