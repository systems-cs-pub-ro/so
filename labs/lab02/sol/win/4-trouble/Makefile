CC = cl
L = link
CFLAGS = /W3 /nologo /I..\utils

all: trouble.exe

trouble.exe: trouble.obj
	$(L) /nologo $**

trouble.obj: trouble.c ..\utils\utils.h
	$(CC) $(CFLAGS) trouble.c

.IGNORE:
clean:
	del /Q /S /F *.obj trouble.exe
