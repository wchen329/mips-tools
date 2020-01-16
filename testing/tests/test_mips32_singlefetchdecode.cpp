#include "basic_assert.h"
#include "phdl.h"
#include "rtl_cpu_mips.h"

/* Basic Fetch/Decode
 * Frontend MIPS-32 Test
 */
void test_mips32_singlefetchdecode()
{
	using namespace priscas;

	// Little Endian: add $5, $6, $7
	BW_32 inst = 0x00c72820;

	Clock c1;
	mPHDL_Execution_Engine execeng(new PHDL_Sequential_Execution_Engine);
	c1.setExecutionEngine(execeng);

	mmem m1;
	mRegister_32 pc(new Register_32);
	mmips_single_fetch_unit_32 msfu(new mips_single_fetch_unit_32(m1, pc));
	mmips_decoding_unit_32 mdu32(new mips_decoding_unit_32(msfu));

	c1.connect(pc);
	m1[0] = inst.get_ByteN(0);
	m1[1] = inst.get_ByteN(1);
	m1[2] = inst.get_ByteN(2);
	m1[3] = inst.get_ByteN(3);

	c1.cycle();
	execeng->start();

	unsigned fetchval = msfu->get_Drive_Output().AsInt32();

	assertEquals(fetchval, 0x00c72820);


	// Test basic control signals (for R-inst)
	int32_t memread_out = mdu32->get_MemRead_out()->get_Drive_Output().AsInt32();
	int32_t memwrite_out = mdu32->get_MemWrite_out()->get_Drive_Output().AsInt32();
	int32_t rd_out = mdu32->get_RegDst_out()->get_Drive_Output().AsInt32();

	assertEquals(memread_out, 0); // Assert: Not mem read
	assertEquals(memwrite_out, 0); // Assert: Not mem write
	assertEquals(rd_out, 5); // Writing to Register 5
}