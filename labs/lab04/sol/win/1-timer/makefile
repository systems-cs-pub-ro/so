CFLAGS = /nologo /W4 /EHsc /D_CRT_SECURE_NO_WARNINGS /I../utils/

all: mytimer.exe

mytimer.exe: mytimer.obj
	$(CC) $(CFLAGS) /Fe$@ $**

clean:
	del /Q /S *.obj mytimer.exe *~
