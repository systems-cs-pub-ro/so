CFLAGS = /nologo /W3 /EHsc /Z7
CC = cl
LINK = link
LDFLAGS = /nologo
LDLIBS = ../so_stdio.lib
build_dir = _build
bin_dir = bin

test_bin = \
	$(bin_dir)/test_fopen_args.exe \
	$(bin_dir)/test_fgetc.exe \
	$(bin_dir)/test_fgetc_large.exe \
	$(bin_dir)/test_fread_small.exe \
	$(bin_dir)/test_fread_large.exe \
	$(bin_dir)/test_fread_huge.exe \
	$(bin_dir)/test_fread_large_chunked.exe \
	$(bin_dir)/test_fread_huge_chunked.exe \
	$(bin_dir)/test_fputc.exe \
	$(bin_dir)/test_fputc_large.exe \
	$(bin_dir)/test_fwrite_small.exe \
	$(bin_dir)/test_fwrite_large.exe \
	$(bin_dir)/test_fwrite_huge.exe \
	$(bin_dir)/test_fwrite_large_chunked.exe \
	$(bin_dir)/test_fwrite_huge_chunked.exe \
	$(bin_dir)/test_fflush.exe \
	$(bin_dir)/test_fread_ftell.exe \
	$(bin_dir)/test_fwrite_ftell.exe \
	$(bin_dir)/test_fseek_ftell.exe \
	$(bin_dir)/test_fseek_fread.exe \
	$(bin_dir)/test_fseek_fwrite.exe \
	$(bin_dir)/test_fread_fwrite.exe \
	$(bin_dir)/test_fwrite_fread.exe \
	$(bin_dir)/test_feof_fgetc.exe \
	$(bin_dir)/test_feof_fread.exe \
	$(bin_dir)/test_append.exe \
	$(bin_dir)/test_append_read.exe \
	$(bin_dir)/test_ferror_read_small.exe \
	$(bin_dir)/test_ferror_read_large.exe \
	$(bin_dir)/test_ferror_write_small.exe \
	$(bin_dir)/test_ferror_write_large.exe \
	$(bin_dir)/test_popen_read.exe \
	$(bin_dir)/test_popen_write.exe

util_src = src/test_util.c src/hooks.c
util_obj = $(build_dir)/test_util.obj $(build_dir)/hooks.obj

other_bin = $(bin_dir)/my_wc.exe

all: dirs build

dirs:
	-md $(build_dir) $(bin_dir) 1>NUL 2>&1

build: $(test_bin) $(other_bin)

$(bin_dir)/test_fopen_args.exe: $(build_dir)/test_fopen_args.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fopen_args.obj: src/test_fopen_args.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fopen_args.c /Fo$@

$(bin_dir)/test_fgetc.exe: $(build_dir)/test_fgetc.obj $(util_obj)
	$(LINK) $(LDFLAGS) /debug /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fgetc.obj: src/test_fgetc.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fgetc.c /Fo$@

$(bin_dir)/test_fgetc_large.exe: $(build_dir)/test_fgetc_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fgetc_large.obj: src/test_fgetc_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fgetc_large.c /Fo$@

$(bin_dir)/test_fread_small.exe: $(build_dir)/test_fread_small.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fread_small.obj: src/test_fread_small.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_small.c /Fo$@

$(bin_dir)/test_fread_large.exe: $(build_dir)/test_fread_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fread_large.obj: src/test_fread_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_large.c /Fo$@

$(bin_dir)/test_fread_huge.exe: $(build_dir)/test_fread_huge.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fread_huge.obj: src/test_fread_huge.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_huge.c /Fo$@

$(bin_dir)/test_fread_large_chunked.exe: $(build_dir)/test_fread_large_chunked.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fread_large_chunked.obj: src/test_fread_large_chunked.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_large_chunked.c /Fo$@

$(bin_dir)/test_fread_huge_chunked.exe: $(build_dir)/test_fread_huge_chunked.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fread_huge_chunked.obj: src/test_fread_huge_chunked.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_huge_chunked.c /Fo$@

$(bin_dir)/test_fputc.exe: $(build_dir)/test_fputc.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fputc.obj: src/test_fputc.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fputc.c /Fo$@

$(bin_dir)/test_fputc_large.exe: $(build_dir)/test_fputc_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fputc_large.obj: src/test_fputc_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fputc_large.c /Fo$@

$(bin_dir)/test_fwrite_small.exe: $(build_dir)/test_fwrite_small.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fwrite_small.obj: src/test_fwrite_small.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_small.c /Fo$@

$(bin_dir)/test_fwrite_large.exe: $(build_dir)/test_fwrite_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fwrite_large.obj: src/test_fwrite_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_large.c /Fo$@

$(bin_dir)/test_fwrite_huge.exe: $(build_dir)/test_fwrite_huge.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fwrite_huge.obj: src/test_fwrite_huge.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_huge.c /Fo$@

$(bin_dir)/test_fwrite_large_chunked.exe: $(build_dir)/test_fwrite_large_chunked.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fwrite_large_chunked.obj: src/test_fwrite_large_chunked.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_large_chunked.c /Fo$@

$(bin_dir)/test_fwrite_huge_chunked.exe: $(build_dir)/test_fwrite_huge_chunked.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fwrite_huge_chunked.obj: src/test_fwrite_huge_chunked.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_huge_chunked.c /Fo$@

$(bin_dir)/test_fflush.exe: $(build_dir)/test_fflush.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fflush.obj: src/test_fflush.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fflush.c /Fo$@

$(bin_dir)/test_fread_ftell.exe: $(build_dir)/test_fread_ftell.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fread_ftell.obj: src/test_fread_ftell.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_ftell.c /Fo$@

$(bin_dir)/test_fwrite_ftell.exe: $(build_dir)/test_fwrite_ftell.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fwrite_ftell.obj: src/test_fwrite_ftell.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_ftell.c /Fo$@

$(bin_dir)/test_fseek_ftell.exe: $(build_dir)/test_fseek_ftell.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fseek_ftell.obj: src/test_fseek_ftell.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fseek_ftell.c /Fo$@

$(bin_dir)/test_fseek_fread.exe: $(build_dir)/test_fseek_fread.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fseek_fread.obj: src/test_fseek_fread.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fseek_fread.c /Fo$@

$(bin_dir)/test_fseek_fwrite.exe: $(build_dir)/test_fseek_fwrite.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fseek_fwrite.obj: src/test_fseek_fwrite.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fseek_fwrite.c /Fo$@

$(bin_dir)/test_fread_fwrite.exe: $(build_dir)/test_fread_fwrite.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fread_fwrite.obj: src/test_fread_fwrite.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fread_fwrite.c /Fo$@

$(bin_dir)/test_fwrite_fread.exe: $(build_dir)/test_fwrite_fread.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_fwrite_fread.obj: src/test_fwrite_fread.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_fwrite_fread.c /Fo$@

$(bin_dir)/test_feof_fgetc.exe: $(build_dir)/test_feof_fgetc.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_feof_fgetc.obj: src/test_feof_fgetc.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_feof_fgetc.c /Fo$@

$(bin_dir)/test_feof_fread.exe: $(build_dir)/test_feof_fread.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_feof_fread.obj: src/test_feof_fread.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_feof_fread.c /Fo$@

$(bin_dir)/test_append.exe: $(build_dir)/test_append.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_append.obj: src/test_append.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_append.c /Fo$@

$(bin_dir)/test_append_read.exe: $(build_dir)/test_append_read.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_append_read.obj: src/test_append_read.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_append_read.c /Fo$@

$(bin_dir)/test_ferror_read_small.exe: $(build_dir)/test_ferror_read_small.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_ferror_read_small.obj: src/test_ferror_read_small.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_ferror_read_small.c /Fo$@

$(bin_dir)/test_ferror_read_large.exe: $(build_dir)/test_ferror_read_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_ferror_read_large.obj: src/test_ferror_read_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_ferror_read_large.c /Fo$@

$(bin_dir)/test_ferror_write_small.exe: $(build_dir)/test_ferror_write_small.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_ferror_write_small.obj: src/test_ferror_write_small.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_ferror_write_small.c /Fo$@

$(bin_dir)/test_ferror_write_large.exe: $(build_dir)/test_ferror_write_large.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_ferror_write_large.obj: src/test_ferror_write_large.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_ferror_write_large.c /Fo$@

$(bin_dir)/test_popen_read.exe: $(build_dir)/test_popen_read.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_popen_read.obj: src/test_popen_read.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_popen_read.c /Fo$@

$(bin_dir)/test_popen_write.exe: $(build_dir)/test_popen_write.obj $(util_obj)
	$(LINK) $(LDFLAGS) /OUT:$@ $** $(LDLIBS)

$(build_dir)/test_popen_write.obj: src/test_popen_write.c src/hooks.h src/test_util.h
	$(CC) $(CFLAGS) /c src/test_popen_write.c /Fo$@

$(build_dir)/test_util.obj: src/test_util.c src/test_util.h
	$(CC) $(CFLAGS) /c src/test_util.c /Fo$@

$(build_dir)/hooks.obj: src/hooks.c src/hooks.h
	$(CC) $(CFLAGS) /c src/hooks.c /Fo$@

$(bin_dir)/my_wc.exe: $(build_dir)/my_wc.obj
	$(LINK) $(LDFLAGS) /OUT:$@ $**

$(build_dir)/my_wc.obj: src/my_wc.c
	$(CC) $(CFLAGS) /c $** /Fo$@

clean:
	-del /Q /S $(bin_dir) 1>NUL 2>&1
	-del /Q /S $(build_dir) 1>NUL 2>&1
	-rd $(bin_dir) 1>NUL 2>&1
	-rd $(build_dir) 1>NUL 2>&1
