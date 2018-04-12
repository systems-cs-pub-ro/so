CFLAGS = /nologo /W3 /EHsc
CC = cl
LINK = link

build: so_loader.dll

so_loader.dll: loader.obj exec_parser.obj
	$(LINK) /nologo /dll /out:$@ /implib:so_loader.lib $**

loader.obj: loader/loader.c
	$(CC) $(CFLAGS) /Fo$@ /c $**

exec_parser.obj: loader/exec_parser.c
	$(CC) $(CFLAGS) /Fo$@ /c $**

clean:
	del /Q loader.obj exec_parser.obj 2>NUL
	del /Q so_loader.dll so_loader.lib so_loader.exp 2>NUL
