#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int create_file_with_contents(char *fname, unsigned char *data, int len)
{
	FILE *f = fopen(fname, "wb");
	int ret;

	if (!f)
		return -1;

	ret = fwrite(data, 1, len, f);
	if (ret != len)
		return -1;

	ret = fclose(f);
	if (ret != 0)
		return -1;

	return 0;
}


int get_file_contents(char *fname, unsigned char **pbuf, int *plen)
{
	FILE *f = fopen(fname, "rb");
	int ret;
	long fsize;
	unsigned char *fdata;

	if (!f)
		return -1;

	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (fsize == -1)
		goto out;

	fdata = malloc(fsize);
	if (!fdata)
		goto out;

	ret = fread(fdata, 1, fsize, f);
	if (ret != fsize)
		goto out_free;

	ret = fclose(f);
	if (ret != 0)
		goto out_free;

	*pbuf = fdata;
	*plen = fsize;

	return 0;

out_free:
	free(fdata);
out:
	return -1;
}

int compare_file(char *fname, unsigned char *buf, int len)
{
	unsigned char *fdata;
	int flen;
	int ret;

	ret = get_file_contents(fname, &fdata, &flen);
	if (ret < 0)
		return 0;

	if (len != flen) {
		free(fdata);
		return 0;
	}

	if (memcmp(fdata, buf, len)) {
		free(fdata);
		return 0;
	}

	free(fdata);

	return 1;
}

int file_exists(char *fname)
{
	struct stat st;

	return stat(fname, &st) == 0;
}

int file_size(char *fname)
{
	FILE *f = fopen(fname, "rb");
	long fsize;

	if (!f)
		return -1;

	fseek(f, 0, SEEK_END);
	fsize = ftell(f);

	fclose(f);

	return fsize;
}

void fail_if(int cond, char *file, int line, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	if (cond) {
		fprintf(stderr, "%s:%d: ", file, line);
		vfprintf(stderr, fmt, ap);
		fflush(stderr);
		exit(1);
	}

	va_end(ap);
}
