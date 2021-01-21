CC = gcc
CFLAGS = -Wall -g -O0
LDFLAGS = -L..
LDLIBS = -lso_stdio -ldl

src_dir := src
build_dir := _build
bin_dir := bin

# input test source files here
test_src = \
	src/test_fopen_args.c \
	src/test_fgetc.c \
	src/test_fgetc_large.c \
	src/test_fread_small.c \
	src/test_fread_large.c \
	src/test_fread_huge.c \
	src/test_fread_large_chunked.c \
	src/test_fread_huge_chunked.c \
	src/test_fputc.c \
	src/test_fputc_large.c \
	src/test_fwrite_small.c \
	src/test_fwrite_large.c \
	src/test_fwrite_huge.c \
	src/test_fwrite_large_chunked.c \
	src/test_fwrite_huge_chunked.c \
	src/test_fflush.c \
	src/test_fread_ftell.c \
	src/test_fwrite_ftell.c \
	src/test_fseek_ftell.c \
	src/test_fseek_fread.c \
	src/test_fseek_fwrite.c \
	src/test_fread_fwrite.c \
	src/test_fwrite_fread.c \
	src/test_feof_fgetc.c \
	src/test_feof_fread.c \
	src/test_append.c \
	src/test_append_read.c \
	src/test_ferror_read_small.c \
	src/test_ferror_read_large.c \
	src/test_ferror_write_small.c \
	src/test_ferror_write_large.c \
	src/test_popen_read.c \
	src/test_popen_write.c \
	src/test_ferror_fclose.c \
	src/test_ferror_fseek.c \
	src/test_fread_huge_random.c \
	src/test_fwrite_huge_random.c \
	src/test_perror_fork.c \
	src/test_perror_wait.c

$(shell mkdir -p $(bin_dir) $(build_dir))

test_obj = $(addprefix $(build_dir)/,$(test_src:src/%.c=%.o))
test_bin = $(addprefix $(bin_dir)/,$(test_src:src/%.c=%))

util_src = src/hooks.c src/test_util.c
util_obj = $(addprefix $(build_dir)/,$(util_src:src/%.c=%.o))

dep = $(test_obj:.o=.d) $(util_obj:.o=.d)

.PHONY: build
build: $(test_bin)

$(test_bin): $(bin_dir)/%: $(build_dir)/%.o $(util_obj)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(test_obj): $(build_dir)/%.o: $(src_dir)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(util_obj): $(build_dir)/%.o: $(src_dir)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

ifneq ($(MAKECMDGOALS),clean)
-include $(dep)
endif

$(dep): $(build_dir)/%.d: $(src_dir)/%.c
	$(CPP) $(CPPFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -rf $(bin_dir) $(build_dir)
