#include "basic_assert.h"
#include "mmem.h"
#include "sc_cpu.h"

#ifndef WIN32
int main()
{
#else
void test_sc_cpu_ops()
{
#endif
	using namespace mips_tools;
	mmem cpu_mem;
	sc_cpu cpu(cpu_mem);
	cpu.rst();
	BW_32 pc = cpu.get_PC();
	assertEquals(pc, 0);
	cpu.cycle();
	pc = cpu.get_PC();
	assertEquals(pc, 4);
}