CC = cl
L = link
CFLAGS = /W3 /nologo /I..\utils

all: sort.exe generator.exe

sort.exe: sort.obj
	$(L) /nologo $**

sort.obj: sort.c ..\utils\utils.h
	$(CC) $(CFLAGS) sort.c

generator.exe: generator.obj
	$(L) /nologo $**

generator.obj: generator.c ..\utils\utils.h
	$(CC) $(CFLAGS) generator.c
	
.IGNORE:
clean:
	del /Q /S /F *.obj sort.exe generator.exe
