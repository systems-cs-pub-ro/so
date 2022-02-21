#ifndef CRC32_H__
#define CRC32_H__

unsigned long update_crc(unsigned long crc,
		unsigned char *buf, unsigned long len);

#endif
