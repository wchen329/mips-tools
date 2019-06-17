#include "basic_assert.h"
#include "mmem.h"
#include "sc_cpu.h"
#include <cstdio>

void test_sc_cpu_ops()
{
	using namespace mips_tools;
	mmem cpu_mem;
	sc_cpu cpu(cpu_mem);
	cpu.rst();
	BW_32 pc = cpu.get_PC();
	assertEquals(pc, 0);
	cpu.cycle();
	pc = cpu.get_PC();
	assertEquals(pc, 4);
	BW_32 inst = cpu.encode(0, 5, -1, NONE, 100, ADDI);	// $5 <- $0 + 100
	BW_32_T bbb(inst);
	cpu_mem[pc] = bbb.b_0();
	cpu_mem[pc + 1] = bbb.b_1();
	cpu_mem[pc + 2] = bbb.b_2();
	cpu_mem[pc + 3] = bbb.b_3();
	cpu.cycle();
	pc = cpu.get_PC();
	bbb = BW_32_T(cpu.encode(0, 5, -1, NONE, 16, SW)); // sw $5, $0(16)
	cpu_mem[pc] = bbb.b_0();
	cpu_mem[pc + 1] = bbb.b_1();
	cpu_mem[pc + 2] = bbb.b_2();
	cpu_mem[pc + 3] = bbb.b_3();
	cpu.cycle();
	BW_32_T bbb_2 = BW_32_T(cpu_mem[16], cpu_mem[17], cpu_mem[18], cpu_mem[19]);
	BW_32 cmp = 100;
	fflush(stdout);
	assertEquals(bbb_2.as_BW_32(), cmp);
}