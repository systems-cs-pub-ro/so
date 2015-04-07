#ifndef _VMSIM_TEST_H_
#define _VMSIM_TEST_H_

extern void test_vmsim_init_exception_set(void);
extern void test_vmsim_cleanup_exception_unset(void);
extern void test_vm_alloc_more_physical_than_virtual(void);
extern void test_vm_alloc_arguments_ok(void);
extern void test_vm_alloc_start_ok(void);
extern void test_vm_alloc_ram_handle_ok(void);
extern void test_vm_alloc_swap_handle_ok(void);
extern void test_vm_alloc_ram_size(void);
extern void test_vm_alloc_swap_size(void);
extern void test_vm_free_bad_start(void);
extern void test_vm_free_arguments_ok(void);
extern void test_vm_free_start_unset(void);
extern void test_vm_free_ram_handle_unset(void);
extern void test_vm_free_swap_handle_unset(void);
extern void test_mapping_is_set_in_handler(void);
extern void test_mapping_read_results_in_one_fault(void);
extern void test_mapping_write_results_in_two_faults(void);
extern void test_mapping_write_after_read_results_in_one_fault(void);
extern void test_mapping_read_after_read_no_faults(void);
extern void test_mapping_write_after_write_no_faults(void);
extern void test_mapping_one_fault_per_page_read(void);
extern void test_mapping_two_faults_per_page_write(void);
extern void test_mapping_mixed_faults(void);
extern void test_mapping_write_is_carried_through_to_file(void);
extern void test_mapping_multiple_writes_fill_ram(void);
extern void test_mapping_multiple_writes_do_not_overwrite_ram(void);
extern void test_swap_out(void);
extern void test_initial_readonly_page_is_swapped_out(void);
extern void test_swap_in_faults(void);
extern void test_swap_in(void);
extern void test_clean_page_is_not_swapped_out(void);
extern void test_vm_alloc_multiple_arguments_ok(void);
extern void test_vm_alloc_multiple_get_faults(void);

#endif /* _VMSIM_TEST_H_ */
