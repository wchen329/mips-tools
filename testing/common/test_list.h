#ifndef __TEST_LIST_H__
#define __TEST_LIST_H__

// Basic tests
void test_assert();
void test_register_32();
void test_shell_comments();
void test_shell_multidirectsymbol();

// Single CPU - MIPS32
void test_sc_cpu_core_instructions();
void test_sc_cpu_loop();
void test_sc_cpu_ops();
void test_sc_cpu_registers();
void test_sc_cpu_subroutine();

// Five Stage Pipeline - MIPS32
void test_mips32fsp_load_to_use();
void test_mips32fsp_registers();
void test_mips32fsp_core_instructions();
void test_mips32fsp_subroutine();
void test_mips32fsp_nostall();
void test_mips32fsp_forwarding_basic();

#endif
