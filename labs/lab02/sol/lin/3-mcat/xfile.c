/**
 * SO, 2017
 * Lab #2, Operatii I/O simple
 *
 * Task #3, Linux
 *
 * Full length read/write
 */

#include "xfile.h"

#include <stdio.h>

ssize_t xread(int fd, void *buf, size_t count)
{
    size_t bytes_read = 0;

    while (bytes_read < count) {
        ssize_t bytes_read_now = read(fd, buf + bytes_read,
                                      count - bytes_read);

        if (bytes_read_now == 0) /* EOF */
            return bytes_read;

        if (bytes_read_now < 0) /* I/O error */
            return -1;

        bytes_read += bytes_read_now;
    }

    return bytes_read;
}

ssize_t xwrite(int fd, const void *buf, size_t count)
{
    size_t bytes_written = 0;

    while (bytes_written < count) {
        ssize_t bytes_written_now = write(fd, buf + bytes_written,
                                          count - bytes_written);

        if (bytes_written_now <= 0) /* I/O error */
            return -1;

        bytes_written += bytes_written_now;
    }

    return bytes_written;
}
