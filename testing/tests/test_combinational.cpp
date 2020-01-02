#include "basic_assert.h"
#include "phdl.h"
#include "rtl_cpu_common.h"

/* 
 * Test for Small Combinational Circuits
 * (integration test with Sequential Circuits)
 */
void test_combinational()
{
	using namespace priscas;
	
	Clock clk_1;		// Base clock

	mRegister_32 r1(new Register_32);
	mRegister_32 r2(new Register_32);
	mPrimitiveAnd ang(new PrimitiveAnd);

	r1->connect_input(ang);
	r2->connect_input(ang);
	ang->connect_input(r1);
	ang->connect_input(r2);

	r1->force_current_state(10);
	r2->force_current_state(3);

	// Connect clock signals
	clk_1.connect(r1);
	clk_1.connect(r2);

	mPHDL_Execution_Engine eng(new PHDL_Sequential_Execution_Engine);
	clk_1.setExecutionEngine(eng);

	// Cycle 1
	BW_32 data_1;
	BW_32 data_2;

	clk_1.base_cycle();
	eng->start();

	data_1 = **r1;
	data_2 = **r2;
	assertEquals(data_1.AsInt32(), 2); // State update
	assertEquals(data_2.AsInt32(), 2); // State update
}