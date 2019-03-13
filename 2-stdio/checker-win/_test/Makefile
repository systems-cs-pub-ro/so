CFLAGS = /nologo /W3 /EHsc /Z7
CC = cl
LINK = link
LDFLAGS = /nologo
LDLIBS = ../so_stdio.lib

test_bin = \
	bin/test_fopen_args.exe \
	bin/test_fgetc.exe \
	bin/test_fgetc_large.exe \
	bin/test_fread_small.exe \
	bin/test_fread_large.exe \
	bin/test_fread_huge.exe \
	bin/test_fread_large_chunked.exe \
	bin/test_fread_huge_chunked.exe \
	bin/test_fputc.exe \
	bin/test_fputc_large.exe \
	bin/test_fwrite_small.exe \
	bin/test_fwrite_large.exe \
	bin/test_fwrite_huge.exe \
	bin/test_fwrite_large_chunked.exe \
	bin/test_fwrite_huge_chunked.exe \
	bin/test_fflush.exe \
	bin/test_fseek_ftell.exe \
	bin/test_fseek_fread.exe \
	bin/test_fseek_fwrite.exe \
	bin/test_fread_fwrite.exe \
	bin/test_fwrite_fread.exe \
	bin/test_feof_fgetc.exe \
	bin/test_feof_fread.exe \
	bin/test_append.exe \
	bin/test_append_read.exe \
	bin/test_ferror_read_small.exe \
	bin/test_ferror_read_large.exe \
	bin/test_ferror_write_small.exe \
	bin/test_ferror_write_large.exe \
	bin/test_popen_read.exe \
	bin/test_popen_write.exe

util_src = src/test_util.c src/hooks.c
util_obj = build/test_util.obj build/hooks.obj

other_bin = bin/my_wc.exe

all: dirs build

dirs:
	-md build bin 1>NUL 2>&1

build: $(test_bin) $(other_bin)

bin/test_fopen_args.exe: build/test_fopen_args.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fopen_args.obj: src/test_fopen_args.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fopen_args.c /Fo$@

bin/test_fgetc.exe: build/test_fgetc.obj $(util_obj)
	$(LINK) $(LDFLAGS) /debug /OUT:$@ $** $(LDLIBS)

build/test_fgetc.obj: src/test_fgetc.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fgetc.c /Fo$@

bin/test_fgetc_large.exe: build/test_fgetc_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fgetc_large.obj: src/test_fgetc_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fgetc_large.c /Fo$@

bin/test_fread_small.exe: build/test_fread_small.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fread_small.obj: src/test_fread_small.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_small.c /Fo$@

bin/test_fread_large.exe: build/test_fread_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fread_large.obj: src/test_fread_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_large.c /Fo$@

bin/test_fread_huge.exe: build/test_fread_huge.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fread_huge.obj: src/test_fread_huge.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_huge.c /Fo$@

bin/test_fread_large_chunked.exe: build/test_fread_large_chunked.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fread_large_chunked.obj: src/test_fread_large_chunked.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_large_chunked.c /Fo$@

bin/test_fread_huge_chunked.exe: build/test_fread_huge_chunked.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fread_huge_chunked.obj: src/test_fread_huge_chunked.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_huge_chunked.c /Fo$@

bin/test_fputc.exe: build/test_fputc.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fputc.obj: src/test_fputc.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fputc.c /Fo$@

bin/test_fputc_large.exe: build/test_fputc_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fputc_large.obj: src/test_fputc_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fputc_large.c /Fo$@

bin/test_fwrite_small.exe: build/test_fwrite_small.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fwrite_small.obj: src/test_fwrite_small.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_small.c /Fo$@

bin/test_fwrite_large.exe: build/test_fwrite_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fwrite_large.obj: src/test_fwrite_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_large.c /Fo$@

bin/test_fwrite_huge.exe: build/test_fwrite_huge.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fwrite_huge.obj: src/test_fwrite_huge.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_huge.c /Fo$@

bin/test_fwrite_large_chunked.exe: build/test_fwrite_large_chunked.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fwrite_large_chunked.obj: src/test_fwrite_large_chunked.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_large_chunked.c /Fo$@

bin/test_fwrite_huge_chunked.exe: build/test_fwrite_huge_chunked.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fwrite_huge_chunked.obj: src/test_fwrite_huge_chunked.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_huge_chunked.c /Fo$@

bin/test_fflush.exe: build/test_fflush.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fflush.obj: src/test_fflush.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fflush.c /Fo$@

bin/test_fseek_ftell.exe: build/test_fseek_ftell.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fseek_ftell.obj: src/test_fseek_ftell.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fseek_ftell.c /Fo$@

bin/test_fseek_fread.exe: build/test_fseek_fread.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fseek_fread.obj: src/test_fseek_fread.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fseek_fread.c /Fo$@

bin/test_fseek_fwrite.exe: build/test_fseek_fwrite.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fseek_fwrite.obj: src/test_fseek_fwrite.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fseek_fwrite.c /Fo$@

bin/test_fread_fwrite.exe: build/test_fread_fwrite.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fread_fwrite.obj: src/test_fread_fwrite.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_fwrite.c /Fo$@

bin/test_fwrite_fread.exe: build/test_fwrite_fread.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_fwrite_fread.obj: src/test_fwrite_fread.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_fread.c /Fo$@

bin/test_feof_fgetc.exe: build/test_feof_fgetc.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_feof_fgetc.obj: src/test_feof_fgetc.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_feof_fgetc.c /Fo$@

bin/test_feof_fread.exe: build/test_feof_fread.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_feof_fread.obj: src/test_feof_fread.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_feof_fread.c /Fo$@

bin/test_append.exe: build/test_append.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_append.obj: src/test_append.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_append.c /Fo$@

bin/test_append_read.exe: build/test_append_read.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_append_read.obj: src/test_append_read.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_append_read.c /Fo$@

bin/test_ferror_read_small.exe: build/test_ferror_read_small.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_ferror_read_small.obj: src/test_ferror_read_small.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_ferror_read_small.c /Fo$@

bin/test_ferror_read_large.exe: build/test_ferror_read_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_ferror_read_large.obj: src/test_ferror_read_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_ferror_read_large.c /Fo$@

bin/test_ferror_write_small.exe: build/test_ferror_write_small.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_ferror_write_small.obj: src/test_ferror_write_small.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_ferror_write_small.c /Fo$@

bin/test_ferror_write_large.exe: build/test_ferror_write_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_ferror_write_large.obj: src/test_ferror_write_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_ferror_write_large.c /Fo$@

bin/test_popen_read.exe: build/test_popen_read.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_popen_read.obj: src/test_popen_read.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_popen_read.c /Fo$@

bin/test_popen_write.exe: build/test_popen_write.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

build/test_popen_write.obj: src/test_popen_write.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_popen_write.c /Fo$@

build/test_util.obj: src/test_util.c src/test_util.h
	$(CC) $(CFLAGS) /c src/test_util.c /Fo$@

build/hooks.obj: src/hooks.c src/hooks.h
	$(CC) $(CFLAGS) /c src/hooks.c /Fo$@

bin/my_wc.exe: build/my_wc.obj
	$(LINK) $(LDFLAGS) /OUT:$@ $**

build/my_wc.obj: src/my_wc.c
	$(CC) $(CFLAGS) /c $** /Fo$@

clean:
	-del /Q /S bin 1>NUL 2>&1
	-del /Q /S build 1>NUL 2>&1
	-rd bin 1>NUL 2>&1
	-rd build 1>NUL 2>&1
