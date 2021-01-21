CC = cl
L = link
CFLAGS = /W3 /nologo /I..\utils

all: ls.exe

ls.exe: ls.obj
	$(L) /nologo $**

ls.obj: ls.c ..\utils\utils.h
	$(CC) $(CFLAGS) ls.c

.IGNORE:
clean:
	del /Q /S /F *.obj ls.exe
