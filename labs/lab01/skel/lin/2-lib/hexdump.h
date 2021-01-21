/**
 * SO, 2016
 * Lab #1, Introducere
 *
 * Task #2, Linux
 *
 * Building libraries
 */

#ifndef _HEXDUMP_H
#define _HEXDUMP_H

#include <linux/types.h>
#include <sys/types.h>

struct xframe {
	__u8 len;
	__u8 data[32];
};

void generate_frame(struct xframe *frame);

void hex_dump_to_buffer(const void *buf, size_t len, int rowsize,
			int groupsize, char *linebuf, size_t linebuflen,
			int ascii);
#endif
