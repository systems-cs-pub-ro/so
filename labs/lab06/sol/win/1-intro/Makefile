CC = cl
L = link
CFLAGS = /W3 /nologo /I..\utils

all: intro.exe

intro.exe: intro.obj
	$(L) /nologo $**

intro.obj: intro.c ..\utils\utils.h
	$(CC) $(CFLAGS) intro.c

.IGNORE:
clean:
	del /Q /S /F *.obj intro.exe
