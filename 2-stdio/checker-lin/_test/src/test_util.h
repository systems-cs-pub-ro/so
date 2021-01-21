#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <stdint.h>

void fail_if(int cond, char *file, int line, const char *fmt, ...);
int create_file_with_contents(char *fname, unsigned char *data, int len);
int compare_file(char *fname, unsigned char *buf, int len);
int get_file_contents(char *fname, unsigned char **buf, int *len);
int file_exists(char *fname);
int file_size(char *fname);

#define FAIL_IF(cond, ...) fail_if((cond), __FILE__, __LINE__, __VA_ARGS__)

#endif //TEST_UTIL_H
