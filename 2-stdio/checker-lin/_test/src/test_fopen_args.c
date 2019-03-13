#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "so_stdio.h"
#include "test_util.h"

int main(int argc, char *argv[])
{
	SO_FILE *f;
	int fsize;
	int ret;
	char *test_work_dir;
	char nonexistent_file[256];
	char existent_file[256];

	if (argc == 2)
		test_work_dir = argv[1];
	else
		test_work_dir = "_test";

	sprintf(nonexistent_file, "%s/nonexistent_file", test_work_dir);
	sprintf(existent_file, "%s/existent_file", test_work_dir);

	unlink(nonexistent_file);

	/* invalid args */
	create_file_with_contents(existent_file, (unsigned char *)"Hello, World!\n", 14);

	f = so_fopen(existent_file, "xyz");
	FAIL_IF(f != NULL, "Incorrect return value for so_fopen: got %p, expected %p\n", f, NULL);

	/* "r" on nonexistent file */

	f = so_fopen(nonexistent_file, "r");
	FAIL_IF(f != NULL, "Incorrect return value for so_fopen: got %p, expected %p\n", f, NULL);
	FAIL_IF(file_exists(nonexistent_file), "File exists where it shouldn't");


	/* "w" on nonexistent file: file should be created*/
	f = so_fopen(nonexistent_file, "w");
	FAIL_IF(f == NULL, "Incorrect return value for so_fopen: got %p, expected != %p\n", f, NULL);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	fsize = file_size(nonexistent_file);
	FAIL_IF(fsize != 0, "Incorrect file size: got %d, expected %d\n", fsize, 0);


	/* "w" on existent file: file should be truncated */
	create_file_with_contents(existent_file, (unsigned char *)"Hello, World!\n", 14);

	f = so_fopen(existent_file, "w");
	FAIL_IF(f == NULL, "Incorrect return value for so_fopen: got %p, expected != %p\n", f, NULL);

	ret = so_fclose(f);
	FAIL_IF(ret != 0, "Incorrect return value for so_fclose: got %d, expected %d\n", ret, 0);

	fsize = file_size(existent_file);
	FAIL_IF(fsize != 0, "Incorrect file size: got %d, expected %d\n", fsize, 0);


	return 0;
}
