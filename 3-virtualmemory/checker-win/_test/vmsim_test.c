/*
 * virtual memory simulator - test suite
 *
 * 2011, Operating Systems
 */

#include <stdlib.h>
#include <string.h>

#include "run_test.h"

/* custom tests */
#include "vmsim.h"
#include "vmsim_aux.h"

/* useful headers */
#include "debug.h"



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
		to_write = (BUFSIZ > left ? left : BUFSIZ);
		w_write_file(handle, buf, to_write);
		left -= to_write;
	}
}

#define zero_file(handle, size)		fill_file(handle, size, 0)

int read_byte_from_file(w_handle_t handle, w_size_t pos)
{
	int value;

	w_set_file_pointer(handle, pos);
	w_read_file(handle, &value, 1);

	return value;
}

/* Cannot have more physical than virtual pages. */
void test_vm_alloc_more_physical_than_virtual(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 11;

	vmsim_init();
	rc = vm_alloc(num_pages, num_frames, &map);
	basic_test(rc == FALSE);
}

/* Handle wrong parameters gracefully. */
void test_vm_alloc_arguments_ok(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	dlog(LOG_DEBUG, "vmsim_init called\n");
	rc = vm_alloc(num_pages, num_frames, &map);
	dlog(LOG_DEBUG, "vm_alloc called\n");
	basic_test(rc == TRUE);
}

/* Allocation goes smoothly. */
void test_vm_alloc_start_ok(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	dlog(LOG_DEBUG, "vmsim_init called\n");
	vm_alloc(num_pages, num_frames, &map);
	dlog(LOG_DEBUG, "vm_alloc called\n");

	/* if mapping's protection can be changed, the mapping is OK */
	rc = w_protect_mapping(map.start, num_pages, PROTECTION_WRITE);
	dlog(LOG_DEBUG, "w_protect_mapping called\n");
	basic_test(rc == TRUE);
}

/* RAM file is valid after allocation. */
void test_vm_alloc_ram_handle_ok(void)
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
	basic_test(rc == TRUE);
}

/* Swap file is valid after allocation. */
void test_vm_alloc_swap_handle_ok(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	rc = w_handle_is_valid(map.swap_handle);
	basic_test(rc == TRUE);
}

/* RAM file size is correct. */
void test_vm_alloc_ram_size(void)
{
	vm_map_t map;
	w_size_t size;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	size = w_get_file_size_by_handle(map.ram_handle);

	basic_test(size == num_frames * p_sz);
}

/* Swap file size is correct. */
void test_vm_alloc_swap_size(void)
{
	vm_map_t map;
	w_size_t size;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	size = w_get_file_size_by_handle(map.swap_handle);

	basic_test(size == num_pages * p_sz);
}

/* Free invalid address. */
void test_vm_free_bad_start(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);
	map.start = NULL;

	rc = vm_free(map.start);
	basic_test(rc == FALSE);
}

/* Free works. */
void test_vm_free_arguments_ok(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);

	rc = vm_free(map.start);
	basic_test(rc == TRUE);
}

/* Free actually unmaps a zone. */
void test_vm_free_start_unset(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);
	vm_free(map.start);

	rc = w_protect_mapping(map.start, num_pages, PROTECTION_NONE);
	basic_test(rc == FALSE);
}

/* After free, ram file in invalid. */
void test_vm_free_ram_handle_unset(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 5;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);
	vm_free(map.start);

	rc = w_handle_is_valid(map.ram_handle);
	basic_test(rc == FALSE);
}

/* After free, swap file is invalid. */
void test_vm_free_swap_handle_unset(void)
{
	vm_map_t map;
	w_boolean_t rc;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	vm_alloc(num_pages, num_frames, &map);
	vm_free(map.start);

	rc = w_handle_is_valid(map.swap_handle);
	basic_test(rc == FALSE);
}

/* Handler is called at page fault. */
void test_mapping_is_set_in_handler(void)
{
	vm_map_t map;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map);

	vmsim_test_reset_faults();

	/* do write; force exception handler call */
	((char *) map.start)[0] = 0;

	basic_test(vmsim_test_get_num_faults() != 0);
}

/* Do read; only one fault should result. */
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

	basic_test(vmsim_test_get_num_faults() == 1);
}

/* Do write; should result in two faults. */
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

	basic_test(vmsim_test_get_num_faults() == 2);
}

/* Do read, then do write; write should result in a single fault. */
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

	basic_test(vmsim_test_get_num_faults() == 1);
}

/* Do read; second read in same page should not fault. */
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

	basic_test(vmsim_test_get_num_faults() == 0);
}

/* Do write; second write in same page should not fault. */
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

	basic_test(vmsim_test_get_num_faults() == 0);
}

/* Do page fault when reading -- demand paging. */
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
		dlog(LOG_DEBUG, "read from page %ld\n", i);
		tmp = ((char *) map.start)[i * p_sz + pos];
	}

	basic_test(vmsim_test_get_num_faults() == num_pages);
}

/* Do page fault when writing -- demand paging. */
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

	basic_test(vmsim_test_get_num_faults() == expected_faults);
}

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

	basic_test(byte_found == TRUE);
}

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
				dlog(LOG_DEBUG, "match found, frame index: "
				     "%ld, offset: %ld\n", i, offset);
				match_count++;
				break;
			}
		}
	}

	dlog(LOG_DEBUG, "match count: %ld\n", match_count);

	basic_test(match_count == num_frames);
}

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

	offset_array = malloc((num_frames+1) * sizeof(*offset_array));

	/*
	 * at most num_frame pages will be backed by RAM
	 * the (num_frame+1)-th access will result in a swap out;
	 */
	for (i = 0; i < num_frames+1; i++) {
		offset_array[i] = get_random_byte_mapping_position(1);
		*((char *) map.start + i * p_sz + offset_array[i]) = MAGIC;
	}

	/* sync ram file to ensure proper swap out */
	w_sync_mapping(map.start, num_pages);

	/* go through swap to look for swapped pages */
	for (i = 0; i < num_pages; i++) {
		for (j = 0; j < num_frames+1; j++) {
			w_size_t offset = i * p_sz + offset_array[j];

			tmp = read_byte_from_file(map.swap_handle, offset);
			if (tmp == MAGIC) {
				dlog(LOG_DEBUG, "match found, frame index: "
				     "%ld, offset: %ld\n", i, offset);
				match_count++;
				break;
			}
		}
	}

	dlog(LOG_DEBUG, "match count: %ld\n", match_count);

	basic_test(match_count > 0);
}

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
	 * the (num_frame+1)-th access will result in a swap out (initial
	 * read-only)
	 */
	for (i = 0; i < num_frames+1; i++) {
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
			dlog(LOG_DEBUG, "match found, frame index: "
			     "%ld, offset: %ld\n", i, offset);
			match_found = TRUE;
		}
	}

	dlog(LOG_DEBUG, "match found: %d\n", match_found);

	basic_test(match_found == TRUE);
}

/* after pages are swapped, access to those pages results in faults */
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

	/*
	 * at most num_frame pages will be backed by RAM; at least the
	 * num-frame+1)-th page is in swap; it will cause a page fault
	 */

	vmsim_test_reset_faults();

	/* fault num_frame+1 pages */
	for (i = 0; i < num_frames+1; i++) {
		offset = get_random_byte_mapping_position(1);
		*((char *) map.start + i * p_sz + offset) = MAGIC;
	}

	basic_test(vmsim_test_get_num_faults() > 0);
}

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
	 * at most num_frame pages will be backed by RAM; at least the
	 * num-frame+1)-th page is in swap; it will be swapped in
	 */

	/* fault num_frame+1 pages */
	for (i = 0; i < num_frames+1; i++) {
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

	basic_test(match_found == TRUE);
}

/*
 * test whether clean pages are swapped out (it shouldn't happen)
 */

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

	basic_test(value1 == ~MAGIC && value2 == ~MAGIC);
}

void test_vm_alloc_multiple_arguments_ok(void)
{
	vm_map_t map1, map2;
	const w_size_t num_pages = 10;
	const w_size_t num_frames = 4;

	vmsim_init();
	w_add_exception_handler(vmsim_test_exception_handler);
	vm_alloc(num_pages, num_frames, &map1);
	vm_alloc(num_pages, num_frames, &map2);

	basic_test(map1.start != map2.start &&
			map1.ram_handle != map2.ram_handle &&
			map1.swap_handle != map2.swap_handle);
}

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

	basic_test(vmsim_test_get_num_faults() == expected_faults);
}
/**/


