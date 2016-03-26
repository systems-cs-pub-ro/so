#ifndef _VMSIM_TEST_H_
#define _VMSIM_TEST_H_

void test_vmsim_init_exception(void);
void test_vmsim_cleanup_exception(void);
void test_vm_alloc_arguments(void);
void test_vm_alloc_start_addr(void);
void test_vm_alloc_ram_handle(void);
void test_vm_alloc_swap_handle(void);
void test_vm_alloc_ram_size(void);
void test_vm_alloc_swap_size(void);
void test_vm_free_arguments(void);
void test_vm_free_start_addr(void);
void test_vm_free_ram_handle(void);
void test_vm_free_swap_handle(void);
void test_mapping_is_set_in_handler(void);
void test_mapping_read_results_in_one_fault(void);
void test_mapping_write_results_in_two_faults(void);
void test_mapping_write_after_read_results_in_one_fault(void);
void test_mapping_read_after_read_no_faults(void);
void test_mapping_write_after_write_no_faults(void);
void test_mapping_one_fault_per_page_read(void);
void test_mapping_two_faults_per_page_write(void);
void test_mapping_mixed_faults(void);
void test_page_is_cleared_at_first_allocation(void);
void test_mapping_write_is_carried_through_to_file(void);
void test_mapping_multiple_writes_fill_ram(void);
void test_mapping_multiple_writes_do_not_overwrite_ram(void);
void test_swap_out(void);
void test_initial_readonly_page_is_swapped_out(void);
void test_swap_in_faults(void);
void test_swap_in(void);
void test_clean_page_is_not_swapped_out(void);
void test_page_has_read_prot_after_swap_in(void);
void test_page_content_is_not_accessed_from_swap(void);
void test_vm_alloc_multiple_arguments(void);
void test_vm_alloc_multiple_get_faults(void);

#endif

