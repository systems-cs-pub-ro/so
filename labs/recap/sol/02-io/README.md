# 02: IO

The [`splice`](https://man7.org/linux/man-pages/man2/splice.2.html) syscall uses a zero-copy mechanism.
It uses an intermediary pipe to write data to a file descriptor.

Owing to the existence of a cache for IO operations inside the operating system **(pagecache)**, a second access to the file is much faster, the data being served from this cache, which resides in the RAM.

When truncating the file, the structure that is modified is kept in memory, i.e. the `inode`, so the overhead is small.

When using an input file that is already mapped in memory (eg. a char device such as `/dev/zero`), the difference is substantial.
