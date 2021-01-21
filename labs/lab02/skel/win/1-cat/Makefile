CC = cl
L = link
CFLAGS = /W3 /nologo /I..\utils

all: cat.exe

cat.exe: cat.obj
	$(L) /nologo $**

cat.obj: cat.c ..\utils\utils.h
	$(CC) $(CFLAGS) cat.c

.IGNORE:
clean:
	del /Q /S /F *.obj cat.exe
