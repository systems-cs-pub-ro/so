/**
 * SO, 2019
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

    /* While read returns less than count bytes check for:
     *  - 0 => EOF
     *  - <0 => I/O error
     *  - else continue reading until count is reached
     */
    while (bytes_read < count) {
        ssize_t bytes_read_now = read(fd, buf + bytes_read,
                                      count - bytes_read);

        /* TODO 3 */
    }

    return bytes_read;
}

ssize_t xwrite(int fd, const void *buf, size_t count)
{
    size_t bytes_written = 0;

    /* While write returns less than count bytes check for:
     *  - <=0 => I/O error
     *  - else continue writing until count is reached
     */
    while (bytes_written < count) {
        ssize_t bytes_written_now = write(fd, buf + bytes_written,
                                          count - bytes_written);

        /* TODO 3 */
    }
    return bytes_written;
}
