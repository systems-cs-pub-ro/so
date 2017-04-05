/*
 * virtual memory simulator - test suite
 *
 * 2017, Operating Systems
 */

#include "run_test.h"

/* custom tests */
#include "vmsim.h"
#include "vmsim_aux.h"

/* useful headers */
#include "debug.h"

#include <stdlib.h>
#include <string.h>

w_size_t get_random_byte_mapping_position(w_size_t num_pages)
{
	return (w_size_t) (rand() % (num_pages * p_sz));
}

void fill_file(w_handle_t handle, w_size_t size, char byte)
{
	char buf[BUFSIZ];
	w_size_t to_write;
	w_size_t left;

	w_set_file_pointer(handle, 0);

	memset(buf, byte, BUFSIZ);
	left = size;
	while (left > 0) {
		to_write = (left < BUFSIZ ? left : BUFSIZ);
		w_write_file(handle, buf, to_write);
		left -= to_write;
	}
}

#define zero_file(handle, size) fill_file(handle, size, 0)

char read_byte_from_file(w_handle_t handle, w_size_t pos)
{
	char value;

	w_set_file_pointer(handle, pos);
	w_read_file(handle, &value, 1);

	return value;
}

static w_boolean_t test_vm_alloc_good_arguments(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_frames = 4;
	const w_size_t num_pages = 8;

	vmsim_init();
	rc = vm_alloc(num_pages, num_frames, &map);

	vm_free(map.start);
	vmsim_cleanup();

	return rc;
}

static w_boolean_t test_vm_alloc_bad_arguments(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_frames = 4;
	const w_size_t num_pages = 2;

	vmsim_init();
	rc = vm_alloc(num_pages, num_frames, &map);

	vmsim_cleanup();

	return rc;
}

/* check 'vm_alloc' parameters */
void test_vm_alloc_arguments(void)
{
	basic_test(test_vm_alloc_good_arguments() == TRUE &&
		test_vm_alloc_bad_arguments() == FALSE);
}

/* check that virtual memory allocation goes smoothly */
void test_vm_alloc_start_addr(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 1;
	const w_size_t num_frames = 1;

	vmsim_init();
	dlog(LOG_DEBUG, "vmsim_init called\n");
	vm_alloc(num_pages, num_frames, &map);
	dlog(LOG_DEBUG, "vm_alloc called\n");

	/* if mapping's protection can be changed, the mapping is OK */
	rc = w_protect_mapping(map.start, num_pages, PROTECTION_READ);
	dlog(LOG_DEBUG, "w_protect_mapping called\n");

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(rc == TRUE);
}

/* check that RAM file is valid after allocation */
void test_vm_alloc_ram_handle(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	dlog(LOG_DEBUG, "vmsim_init called\n");
	vm_alloc(num_pages, num_frames, &map);
	dlog(LOG_DEBUG, "vm_alloc called\n");

	rc = w_handle_is_valid(map.ram_handle);
	dlog(LOG_DEBUG, "w_handle_is_valid called\n");

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(rc == TRUE);
}

/* check that swap file is valid after allocation */
void test_vm_alloc_swap_handle(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	rc = w_handle_is_valid(map.swap_handle);

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(rc == TRUE);
}

/* check RAM file size */
void test_vm_alloc_ram_size(void)
{
	vm_map_t map;
	w_size_t size;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	size = w_get_file_size_by_handle(map.ram_handle);

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(size == num_frames * p_sz);
}

/* check swap file size */
void test_vm_alloc_swap_size(void)
{
	vm_map_t map;
	w_size_t size;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	size = w_get_file_size_by_handle(map.swap_handle);

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(size == num_pages * p_sz);
}

static w_boolean_t test_vm_free_good_arguments(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	rc = vm_free(map.start);
	vmsim_cleanup();

	return rc;
}

static w_boolean_t test_vm_free_bad_arguments(void)
{
	w_boolean_t rc;

	vmsim_init();

	rc = vm_free(NULL);

	vmsim_cleanup();

	return rc;
}

/* check 'vm_free' arguments */
void test_vm_free_arguments(void)
{
	basic_test(test_vm_free_good_arguments() == TRUE &&
		test_vm_free_bad_arguments() == FALSE);
}

/* check that free actually unmaps a zone */
void test_vm_free_start_addr(void)
{
	vm_map_t map;
	w_boolean_t rc_before_free, rc_after_free;
	const w_size_t num_pages = 1;
	const w_size_t num_frames = 1;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	rc_before_free = w_protect_mapping(map.start, num_pages,
		PROTECTION_READ);
	vm_free(map.start);
	rc_after_free = w_protect_mapping(map.start, num_pages,
		PROTECTION_READ);

	vmsim_cleanup();

	basic_test(rc_before_free == TRUE && rc_after_free == FALSE);
}

/* check that after free, RAM file in invalid */
void test_vm_free_ram_handle(void)
{
	vm_map_t map;
	w_boolean_t rc_before_free, rc_after_free;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	rc_before_free = w_handle_is_valid(map.ram_handle);
	vm_free(map.start);
	rc_after_free = w_handle_is_valid(map.ram_handle);

	vmsim_cleanup();

	basic_test(rc_before_free == TRUE && rc_after_free == FALSE);
}

/* check that after free, swap file is invalid */
void test_vm_free_swap_handle(void)
{
	vm_map_t map;
	w_boolean_t rc_before_free, rc_after_free;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	rc_before_free = w_handle_is_valid(map.swap_handle);
	vm_free(map.start);
	rc_after_free = w_handle_is_valid(map.swap_handle);

	vmsim_cleanup();

	basic_test(rc_before_free == TRUE && rc_after_free == FALSE);
}

/* check that handler is called at page fault */
void test_mapping_is_set_in_handler(void)
{
	vm_map_t map;
	char tmp;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	vmsim_test_reset_faults();

	/* do read; force exception handler call */
	tmp = ((char *) map.start)[0];

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() != 0);
}

/* do read; only one fault should result */
void test_mapping_read_results_in_one_fault(void)
{
	vm_map_t map;
	char tmp;
	w_size_t pos;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	pos = get_random_byte_mapping_position(num_pages);

	vmsim_test_reset_faults();
	tmp = ((char *) map.start)[pos];

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == 1);
}

/* do write; should result in two faults */
void test_mapping_write_results_in_two_faults(void)
{
	vm_map_t map;
	w_size_t pos;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	pos = get_random_byte_mapping_position(num_pages);

	vmsim_test_reset_faults();
	((char *) map.start)[pos] = MAGIC;

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == 2);
}

/* do read, then do write; write should result in a single fault */
void test_mapping_write_after_read_results_in_one_fault(void)
{
	vm_map_t map;
	char tmp;
	w_size_t pos;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	pos = get_random_byte_mapping_position(num_pages);

	tmp = ((char *) map.start)[pos];
	vmsim_test_reset_faults();
	((char *) map.start)[pos] = MAGIC;

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == 1);
}

/* do read; second read in same page should not fault */
void test_mapping_read_after_read_no_faults(void)
{
	vm_map_t map;
	char tmp;
	w_size_t pos;
	w_size_t new_offset;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	pos = get_random_byte_mapping_position(num_pages);

	tmp = ((char *) map.start)[pos];
	vmsim_test_reset_faults();
	new_offset = pos / p_sz * p_sz + get_random_byte_mapping_position(1);
	tmp = ((char *) map.start)[new_offset];

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == 0);
}

/* do write; second write in same page should not fault */
void test_mapping_write_after_write_no_faults(void)
{
	vm_map_t map;
	w_size_t pos;
	w_size_t new_offset;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	pos = get_random_byte_mapping_position(num_pages);

	((char *) map.start)[pos] = MAGIC;
	vmsim_test_reset_faults();
	new_offset = pos / p_sz * p_sz + get_random_byte_mapping_position(1);
	((char *) map.start)[new_offset] = MAGIC;

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == 0);
}

/* do page fault when reading -- demand paging */
void test_mapping_one_fault_per_page_read(void)
{
	vm_map_t map;
	char tmp;
	w_size_t pos;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	w_size_t i;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	vmsim_test_reset_faults();
	for (i = 0; i < num_pages; i++) {
		pos = get_random_byte_mapping_position(1);
		dlog(LOG_DEBUG, "read from page %u\n", i);
		tmp = ((char *) map.start)[i * p_sz + pos];
	}

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == num_pages);
}

/* do page fault when writing -- demand paging */
void test_mapping_two_faults_per_page_write(void)
{
	vm_map_t map;
	w_size_t pos;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	w_size_t i;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	vmsim_test_reset_faults();
	for (i = 0; i < num_pages; i++) {
		pos = get_random_byte_mapping_position(1);
		((char *) map.start)[i * p_sz + pos] = MAGIC;
	}

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == 2 * num_pages);
}

void test_mapping_mixed_faults(void)
{
	vm_map_t map;
	char tmp;
	w_size_t pos;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	w_size_t expected_faults = 0;
	w_size_t i;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	vmsim_test_reset_faults();

	/* go through even pages */
	for (i = 0; i < num_pages; i += 2) {
		pos = get_random_byte_mapping_position(1);
		/* do page fault when reading -- demand paging */
		tmp = ((char *) map.start)[i * p_sz + pos];
		/* when reading, one page fault occurs */
		expected_faults++;
	}

	/* go through odd pages */
	for (i = 1; i < num_pages; i += 2) {
		pos = get_random_byte_mapping_position(1);
		/* do page fault when writing -- demand paging */
		((char *) map.start)[i * p_sz + pos] = MAGIC;
		/* when writing, two page faults occur */
		expected_faults += 2;
	}

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == expected_faults);
}

/* check that page is cleared at first time allocation in RAM */
void test_page_is_cleared_at_first_allocation(void)
{
	vm_map_t map;
	char tmp, value;
	const w_size_t offset = 10;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(2, 1, &map);

	/* fault first page (write) */
	*((char *) map.start + 0 * p_sz + offset) = MAGIC;

	/* fault second page (read-only) */
	tmp = *((char *) map.start + 1 * p_sz + offset);

	w_sync_mapping(map.start, 2);

	w_set_file_pointer(map.ram_handle, 0 * p_sz + offset);
	w_read_file(map.ram_handle, &value, 1);

	dlog(LOG_LEVEL, "value = %02x, tmp = %02x\n", value, tmp);

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(value == 0 && tmp == 0);
}

/* check that a write is carried through to file */
void test_mapping_write_is_carried_through_to_file(void)
{
	vm_map_t map;
	w_size_t pos;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	char tmp;
	w_boolean_t byte_found = FALSE;
	w_size_t page_offset;
	w_size_t i;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);
	zero_file(map.ram_handle, num_frames * p_sz);

	pos = get_random_byte_mapping_position(num_pages);

	((char *) map.start)[pos] = MAGIC;

	w_sync_mapping(map.start, num_pages);

	page_offset = pos % p_sz;
	for (i = 0; i < num_frames; i++) {
		tmp = read_byte_from_file(map.ram_handle,
					  i * p_sz + page_offset);
		if (tmp == MAGIC)
			byte_found = TRUE;
	}

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(byte_found == TRUE);
}

/* check that multiple writes fill the RAM */
void test_mapping_multiple_writes_fill_ram(void)
{
	vm_map_t map;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	char tmp;
	w_size_t i;
	w_size_t j;
	w_size_t *offset_array;
	w_size_t match_count = 0;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);
	zero_file(map.ram_handle, num_frames * p_sz);

	offset_array = malloc(num_frames * sizeof(*offset_array));

	for (i = 0; i < num_frames; i++) {
		offset_array[i] = get_random_byte_mapping_position(1);
		*((char *) map.start + i * p_sz + offset_array[i]) = MAGIC;
	}

	w_sync_mapping(map.start, num_pages);

	for (i = 0; i < num_frames; i++) {
		for (j = 0; j < num_frames; j++) {
			w_size_t offset = i * p_sz + offset_array[j];

			tmp = read_byte_from_file(map.ram_handle, offset);
			if (tmp == MAGIC) {
				match_count++;
				break;
			}
		}
	}

	vm_free(map.start);
	vmsim_cleanup();
	free(offset_array);

	basic_test(match_count == num_frames);
}

void test_mapping_multiple_writes_do_not_overwrite_ram(void)
{
	vm_map_t map;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	char tmp;
	w_size_t i;
	w_size_t j;
	w_size_t *offset_array;
	w_size_t match_count = 0;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);
	zero_file(map.ram_handle, num_frames * p_sz);

	offset_array = malloc(num_pages * sizeof(*offset_array));

	for (i = 0; i < num_pages; i++) {
		offset_array[i] = get_random_byte_mapping_position(1);
		*((char *) map.start + i * p_sz + offset_array[i]) = MAGIC;
	}

	w_sync_mapping(map.start, num_pages);

	for (i = 0; i < num_frames; i++) {
		for (j = 0; j < num_pages; j++) {
			w_size_t offset = i * p_sz + offset_array[j];

			tmp = read_byte_from_file(map.ram_handle, offset);
			if (tmp == MAGIC) {
				dlog(LOG_DEBUG, "match found, frame index:"
					"%u, offset: %u\n", i, offset);
				match_count++;
				break;
			}
		}
	}

	dlog(LOG_DEBUG, "match count: %u\n", match_count);

	vm_free(map.start);
	vmsim_cleanup();
	free(offset_array);

	basic_test(match_count == num_frames);
}

/* test swap out */
void test_swap_out(void)
{
	vm_map_t map;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	char tmp;
	w_size_t i;
	w_size_t j;
	w_size_t *offset_array;
	w_size_t match_count = 0;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	/* zero swap for further checking */
	zero_file(map.swap_handle, num_pages * p_sz);

	offset_array = malloc((num_frames + 1) * sizeof(*offset_array));

	/*
	 * at most num_frame pages will be backed by RAM
	 * the (num_frame + 1)-th access will result in a swap out
	 */
	for (i = 0; i < num_frames + 1; i++) {
		offset_array[i] = get_random_byte_mapping_position(1);
		*((char *) map.start + i * p_sz + offset_array[i]) = MAGIC;
	}

	/* sync ram file to ensure proper swap out */
	w_sync_mapping(map.start, num_pages);

	/* go through swap to look for swapped pages */
	for (i = 0; i < num_pages; i++) {
		for (j = 0; j < num_frames + 1; j++) {
			w_size_t offset = i * p_sz + offset_array[j];

			tmp = read_byte_from_file(map.swap_handle, offset);
			if (tmp == MAGIC) {
				dlog(LOG_DEBUG, "match found, frame index:"
					"%u, offset: %u\n", i, offset);
				match_count++;
				break;
			}
		}
	}

	dlog(LOG_DEBUG, "match count: %u\n", match_count);

	vm_free(map.start);
	vmsim_cleanup();
	free(offset_array);

	basic_test(match_count > 0);
}

/* check that initial read-only page is swapped out */
void test_initial_readonly_page_is_swapped_out(void)
{
	vm_map_t map;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	char tmp;
	w_size_t i;
	w_size_t offset;
	w_boolean_t match_found = FALSE;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	/* zero swap for further checking */
	fill_file(map.swap_handle, num_pages * p_sz, ~MAGIC);

	/*
	 * at most num_frame pages will be backed by RAM
	 * the (num_frame + 1)-th access will result in a swap out
	 */
	for (i = 0; i < num_frames + 1; i++) {
		offset = get_random_byte_mapping_position(1);
		tmp = *((char *) map.start + i * p_sz + offset);
		/* sync ram file to ensure proper swap out */
		w_sync_mapping(map.start, num_pages);
	}

	/* go through swap to look for swapped pages */
	for (i = 0; i < num_pages; i++) {
		tmp = read_byte_from_file(map.swap_handle, i * p_sz + offset);
		dlog(LOG_DEBUG, "tmp = %02x\n", tmp);
		if (tmp != ~MAGIC) {
			dlog(LOG_DEBUG, "match found, frame index:"
				"%u, offset: %u\n", i, offset);
			match_found = TRUE;
		}
	}

	dlog(LOG_DEBUG, "match found: %d\n", match_found);

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(match_found == TRUE);
}

/*
 * check that after pages are swapped,
 * access to those pages results in faults
 */
void test_swap_in_faults(void)
{
	vm_map_t map;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	w_size_t i;
	w_size_t offset;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	/* fault all pages */
	for (i = 0; i < num_pages; i++) {
		offset = get_random_byte_mapping_position(1);
		*((char *) map.start + i * p_sz + offset) = MAGIC;
	}

	dlog(LOG_INFO, "all pages faulted\n");

	vmsim_test_reset_faults();

	/*
	 * at most num_frame pages will be backed by RAM;
	 * at least the (num_frame + 1)-th page is in swap;
	 * it will cause a page fault
	 */
	for (i = 0; i < num_frames + 1; i++) {
		offset = get_random_byte_mapping_position(1);
		*((char *) map.start + i * p_sz + offset) = MAGIC;
	}

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() > 0);
}

/* test swap in */
void test_swap_in(void)
{
	vm_map_t map;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	char tmp;
	w_size_t i;
	w_size_t offset;
	w_boolean_t match_found = FALSE;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	/* fault all pages */
	for (i = 0; i < num_pages; i++) {
		offset = get_random_byte_mapping_position(1);
		*((char *) map.start + i * p_sz + offset) = MAGIC;
	}

	dlog(LOG_INFO, "all pages faulted\n");

	/* fill swap with other data */
	fill_file(map.swap_handle, num_pages * p_sz, ~MAGIC);

	/*
	 * at most num_frame pages will be backed by RAM;
	 * at least the (num_frame + 1)-th page is in swap;
	 * it will be swapped in
	 */
	for (i = 0; i < num_frames + 1; i++) {
		offset = get_random_byte_mapping_position(1);
		*((char *) map.start + i * p_sz + offset) = MAGIC;
	}

	/* some pages will be swapped in */
	for (i = 0; i < num_pages; i++) {
		offset = get_random_byte_mapping_position(1);
		tmp = *((char *) map.start + i * p_sz + offset);
		dlog(LOG_DEBUG, "tmp = %02x, not-magic = %02x\n", tmp, ~MAGIC);
		if (tmp == ~MAGIC)
			match_found = TRUE;
	}

	dlog(LOG_DEBUG, "match found: %d\n", match_found);

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(match_found == TRUE);
}

/* test whether clean pages are swapped out (it shouldn't happen) */
void test_clean_page_is_not_swapped_out(void)
{
	vm_map_t map;
	char tmp, value1, value2;
	const w_size_t offset1 = 10;
	const w_size_t offset2 = 20;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(2, 1, &map);

	/* fault first page (write) */
	*((char *) map.start + 0 * p_sz + offset1) = MAGIC;
	w_sync_mapping(map.start, 2);

	/* fault second page (write) -- first page goes to swap */
	*((char *) map.start + 1 * p_sz + offset2) = MAGIC;
	w_sync_mapping(map.start, 2);

	/* fault first page (read-only) -- second page goes to swap */
	tmp = *((char *) map.start + 0 * p_sz + offset1);
	w_sync_mapping(map.start, 2);

	fill_file(map.swap_handle, 2 * p_sz, ~MAGIC);

	/* fault second page (read-only) -- first page should not go to swap */
	tmp = *((char *) map.start + 1 * p_sz + offset2);
	w_sync_mapping(map.start, 2);

	/* fault first page (read-only) -- second page should not go to swap */
	tmp = *((char *) map.start + 0 * p_sz + offset1);
	w_sync_mapping(map.start, 2);

	w_set_file_pointer(map.swap_handle, 0 * p_sz + offset1);
	w_read_file(map.swap_handle, &value1, 1);
	w_set_file_pointer(map.swap_handle, 1 * p_sz + offset2);
	w_read_file(map.swap_handle, &value2, 1);

	dlog(LOG_LEVEL, "value1 = %02x, value2 = %02x\n", value1, value2);

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(value1 == ~MAGIC && value2 == ~MAGIC);
}

/* check that page has read protection after swap in */
void test_page_has_read_prot_after_swap_in(void)
{
	vm_map_t map;
	const w_size_t offset1 = 10;
	const w_size_t offset2 = 20;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(2, 1, &map);

	/* fault first page (write) */
	*((char *) map.start + 0 * p_sz + offset1) = MAGIC;
	w_sync_mapping(map.start, 2);

	/* fault second page (write) -- first page goes to swap */
	*((char *) map.start + 1 * p_sz + offset2) = MAGIC;
	w_sync_mapping(map.start, 2);

	vmsim_test_reset_faults();

	/* fault first page (write) */
	*((char *) map.start + 0 * p_sz + offset1) = MAGIC;
	w_sync_mapping(map.start, 2);

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == 2);
}

/* check that page content is not accessed from swap */
void test_page_content_is_not_accessed_from_swap(void)
{
	vm_map_t map;
	char tmp, byte1, byte2;
	const w_size_t offset1 = 10;
	const w_size_t offset2 = 20;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(2, 1, &map);

	/* fault first page (write) */
	*((char *) map.start + 0 * p_sz + offset1) = MAGIC;
	w_sync_mapping(map.start, 2);

	/* fault second page (write) -- first page goes to swap */
	*((char *) map.start + 1 * p_sz + offset2) = 2 * MAGIC;
	w_sync_mapping(map.start, 2);

	vmsim_test_reset_faults();

	/* fault first page (read-only) -- second page goes to swap */
	tmp = *((char *) map.start + 0 * p_sz + offset1);

	/* fault second page (read-only) -- first page goes to swap */
	tmp = *((char *) map.start + 1 * p_sz + offset2);

	/* fault first page (read-only) -- second page goes to swap */
	byte1 = *((char *) map.start + 0 * p_sz + offset1);

	/* fault second page (read-only) -- first page goes to swap */
	byte2 = *((char *) map.start + 1 * p_sz + offset2);

	vm_free(map.start);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == 4 &&
		(byte1 == MAGIC && byte2 == 2 * MAGIC));
}

/* check that multiple allocations create different maps */
void test_vm_alloc_multiple_arguments(void)
{
	vm_map_t map1, map2;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	w_ptr_t start1, start2;
	w_handle_t ram_handle1, ram_handle2;
	w_handle_t swap_handle1, swap_handle2;

	memset(&map1, 0, sizeof(vm_map_t));
	memset(&map2, 0, sizeof(vm_map_t));

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map1);
	vm_alloc(num_pages, num_frames, &map2);

	start1 = map1.start;
	start2 = map2.start;
	ram_handle1 = map1.ram_handle;
	ram_handle2 = map2.ram_handle;
	swap_handle1 = map1.swap_handle;
	swap_handle2 = map2.swap_handle;

	vm_free(map1.start);
	vm_free(map2.start);
	vmsim_cleanup();

	basic_test(start1 != start2 &&
			ram_handle1 != ram_handle2 &&
			swap_handle1 != swap_handle2);
}

/* test multiple allocations */
void test_vm_alloc_multiple_get_faults(void)
{
	const w_size_t num_mappings = 6;
	vm_map_t *maps;
	char tmp;
	w_size_t pos;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;
	w_size_t expected_faults = 0;
	w_size_t i, mcount;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vmsim_test_reset_faults();

	maps = malloc(num_mappings * sizeof(*maps));
	for (mcount = 0; mcount < num_mappings; mcount++)
		vm_alloc(num_pages, num_frames, maps + mcount);

	for (mcount = 0; mcount < num_mappings; mcount++) {
		/* go through even pages */
		for (i = 0; i < num_pages; i += 2) {
			pos = get_random_byte_mapping_position(1);
			/* do page fault when reading -- demand paging */
			tmp = ((char *) maps[mcount].start)[i * p_sz + pos];
			/* when reading, one page fault occurs */
			expected_faults++;
		}

		/* go through odd pages */
		for (i = 1; i < num_pages; i += 2) {
			pos = get_random_byte_mapping_position(1);
			/* do page fault when writing -- demand paging */
			((char *) maps[mcount].start)[i * p_sz + pos] = MAGIC;
			/* when writing, two page faults occur */
			expected_faults += 2;
		}
	}

	for (mcount = 0; mcount < num_mappings; mcount++)
		vm_free(maps[mcount].start);
	free(maps);
	vmsim_cleanup();

	basic_test(vmsim_test_get_num_faults() == expected_faults);
}

