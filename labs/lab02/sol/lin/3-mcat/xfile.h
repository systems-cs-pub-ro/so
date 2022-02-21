/**
 * SO, 2017
 * Lab #2, Operatii I/O simple
 *
 * Task #3, Linux
 *
 * Full length read/write
 */

#ifndef XFILE_UTIL_H
#define XFILE_UTIL_H

#include <unistd.h>

/* Read exactly count bytes or die trying.
 *
 * Return values:
 *  < 0     - error.
 *  >= 0    - number of bytes read.
 */
ssize_t xread(int fd, void *buf, size_t count);

/* Write exactly count bytes or die trying.
 *
 * Return values:
 *  < 0     - error.
 *  >= 0    - number of bytes read.
 */
ssize_t xwrite(int fd, const void *buf, size_t count);

#endif
