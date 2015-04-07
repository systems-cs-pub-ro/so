/* scheduler functions */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "run_test.h"
/* custom tests */
#include "vmsim_test.h"

/* Enable/disable exiting when program fails. */
/* #define EXIT_IF_FAIL */
#define max_points 90

/* global variables used by the test */
static char *description;
static int points;
static unsigned long test_index;


/* prints the header specified aligned with the other output */
static void print_header(const char *header)
{
	int i, padding;
	int header_len = strlen(header);

	printf("\n");
	if (header_len < 71) {
		padding = (71 - header_len) / 2;
		for (i = 0; i < padding; i++)
			printf(" ");
	}
	printf("= %s =\n\n", header);
}

void test_do_fail(void)
{
	printf("failed  [ 0/%02d]\n", points);
#ifdef EXIT_IF_FAIL
	exit(EXIT_FAILURE);
#endif
}

void test_do_pass(void)
{
	printf("passed  [%02d/%02d]\n", points, max_points);
}

void basic_test(int true)
{
	int i;
	int desc_len = strlen(description);

	printf("%02lu) %s", test_index + 1, description);
	for (i = 0; i < 56 - desc_len; i++)
		printf(".");
	if (true)
		test_do_pass();
	else
		test_do_fail();
}

static int init_world(void)
{
	print_header("Testing - Virtual Memory");
	return 0;
}

static int cleanup_world(void)
{
	return 0;
}

struct run_test_t test_fun_array[] = {
	{ test_vm_alloc_more_physical_than_virtual,
		"Test vm_alloc more pysical than virtual",		3 },
	{ test_vm_alloc_arguments_ok,
		"Test vm_alloc arguments",				3 },
	{ test_vm_alloc_ram_handle_ok,
		"Test vm_alloc ram handle",				3 },
	{ test_vm_alloc_swap_handle_ok,
		"Test vm_alloc swap handle",				3 },
	{ test_vm_alloc_ram_size,
		"Test vm_alloc ram size",				3 },
	{ test_vm_alloc_swap_size,
		"Test vm_alloc swap size",				3 },
	{ test_vm_free_bad_start,
		"Test vm_free bad start",				3 },
	{ test_vm_free_arguments_ok,
		"Test vm_free arguments",				3 },
	{ test_vm_free_start_unset,
		"Test vm_free arguments unset",				3 },
	{ test_vm_free_ram_handle_unset,
		"Test vm_free ram handle unset",			3 },
	{ test_vm_free_swap_handle_unset,
		"Test vm_free swap handle unset",			3 },
	{ test_mapping_is_set_in_handler,
		"Test mapping is set in handler",			3 },
	{ test_mapping_read_results_in_one_fault,
		"Test mapping read results in one fault",		3 },
	{ test_mapping_write_results_in_two_faults,
		"Test mapping write results in two faults",		3 },
	{ test_mapping_write_after_read_results_in_one_fault,
		"Test mapping write after read results in one fault",	3 },
	{ test_mapping_read_after_read_no_faults,
		"Test mapping read after read no faults",		3 },
	{ test_mapping_write_after_write_no_faults,
		"Test mapping write after write no faults",		3 },
	{ test_mapping_one_fault_per_page_read,
		"Test mapping one fault per page read",			3 },
	{ test_mapping_two_faults_per_page_write,
		"Test mapping two faults per page write",		3 },
	{ test_mapping_mixed_faults,
		"Test mapping mixed faults",				3 },
	{ test_mapping_write_is_carried_through_to_file,
		"Test mapping write is carried through to file",	3 },
	{ test_mapping_multiple_writes_fill_ram,
		"Test mapping multiple writes fill ram",		3 },
	{ test_mapping_multiple_writes_do_not_overwrite_ram,
		"Test mapping multiple writes do not overwrite ram",	3 },
	{ test_swap_out,
		"Test swap out",					3 },
	{ test_initial_readonly_page_is_swapped_out,
		"Test initial readonly page is swapped out",		3 },
	{ test_swap_in_faults,
		"Test swap in faults",					3 },
	{ test_swap_in,
		"Test swap in",						3 },
	{ test_clean_page_is_not_swapped_out,
		"Test clean page is not swapped out",			3 },
	{ test_vm_alloc_multiple_arguments_ok,
		"Test vm_alloc multiple arguments",			3 },
	{ test_vm_alloc_multiple_get_faults,
		"Test vm_alloc multiple get faults",			3 }
};

/* custom main testing thread */
int main(int argc, char **argv)
{
	unsigned long last_test;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <test_number> | init | cleanup\n",
			argv[0]);
		return -1;
	}

	/* check init and cleanup execution */
	if (strcmp(argv[1], "init") == 0) {
		if (init_world() < 0) {
			fprintf(stderr, "test init failed\n");
			return -1;
		}
		return 0;
	} else if (strcmp(argv[1], "cleanup") == 0) {
		if (cleanup_world() < 0) {
			fprintf(stderr, "test cleanup failed\n");
			return -1;
		}
		return 0;
	}

	test_index = strtoul(argv[1], NULL, 10) - 1;
	if (errno == EINVAL || errno == ERANGE) {
		fprintf(stderr, "%s not a number\n", argv[1]);
		return -1;
	}

	last_test = sizeof(test_fun_array) / sizeof(struct run_test_t) - 1;
	if (test_index < 0 || test_index > last_test) {
		fprintf(stderr, "Error: Test index is out of range"
				"(1 < test_index <= %lu).\n", last_test);
		return -1;
	}

	/* randomize time quantums */
	srand((unsigned int)time(NULL));

	description = test_fun_array[test_index].description;
	points = test_fun_array[test_index].points;
	test_fun_array[test_index].function();

	return 0;
}
