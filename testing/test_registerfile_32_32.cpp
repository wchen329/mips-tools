#include "basic_assert.h"
#include "rtl_cpu_common.h"

/* test_registerfile_32_32
 * Test the 32 registers, each 32-bits each
 * (single cycle indexing, )
 */
void test_registerfile_32_32()
{
	using namespace priscas;

	// Clock
	Clock c1;
	mPHDL_Execution_Engine see(new PHDL_Sequential_Execution_Engine);

	// Stimuli
	mRegister_32 raddr_1(new Register_32);
	mRegister_32 raddr_2(new Register_32);
	mRegister_32 waddr_1(new Register_32);
	mNode rdata_1(new Node);
	mNode rdata_2(new Node);
	mRegister_32 wdata_1(new Register_32);

	// Register File
	std::shared_ptr<UniformRegisterFile<BW_32, 32, 2, 1>> rf(new UniformRegisterFile<BW_32, 32, 2, 1>(c1));

	// Connect read port related
	rf->get_nth_read_addr_port(0)->connect_input(raddr_1);
	rf->get_nth_read_addr_port(1)->connect_input(raddr_2);
	rdata_1->connect_input(rf->get_nth_read_port(0));
	rdata_2->connect_input(rf->get_nth_read_port(1));

	// Connect write port related
	rf->get_nth_write_port(0)->connect_input(wdata_1);
	rf->get_nth_write_addr_port(0)->connect_input(waddr_1);

	// Reset rf
	rf->rst();

	// Connect rf and stimuli to clock
	c1.connect(rf);
	c1.connect(raddr_1);
	c1.connect(raddr_2);
	c1.connect(waddr_1);
	c1.connect(wdata_1);
	c1.setExecutionEngine(see);

	// Drive the write wire with 5, and all other wires with 0
	waddr_1->force_current_state(BW_32(5));
	wdata_1->force_current_state(BW_32(5));
	raddr_1->force_current_state(BW_32(0));
	raddr_2->force_current_state(BW_32(0));
	c1.cycle();
	see->start();

	// Confirm both reads are 0, and that wrote 5
	assertEquals(0, rdata_1->get_Drive_Output().AsInt32());
	assertEquals(0, rdata_2->get_Drive_Output().AsInt32());
	assertEquals(5, rf->read_reg(5).AsInt32());

	// Now read from register 5 on the first read port
	raddr_1->force_current_state(BW_32(5));
	c1.cycle();
	see->start();

	// Confirm that port 1 read 5 from 5.
	assertEquals(5, rf->read_reg(5).AsInt32());
	assertEquals(5, rdata_1->get_Drive_Output().AsInt32());
}