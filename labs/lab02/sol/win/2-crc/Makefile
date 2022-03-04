CC = cl
L = link
CFLAGS = /c /W3 /nologo /I..\utils /I.

all: crc.exe

crc.exe: crc.obj crc32.obj
	$(L) /nologo $**

crc32.obj: crc32.c
	$(CC) $(CFLAGS) crc32.c

crc.obj: crc.c ..\utils\utils.h
	$(CC) $(CFLAGS) crc.c

.IGNORE:
clean:
	del /Q /S /F *.obj crc.exe
