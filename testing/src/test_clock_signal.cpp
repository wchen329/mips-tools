#include "basic_assert.h"
#include "primitives.h"
#include "phdl.h"

/* Clock
 * Basic integration test
 */
void test_clock_signal()
{
	using namespace priscas;

	Clock clk_1;		// Base clock
	Clock clk_2(1, 0);	// Clock with offset of 1 (fires every other base clock)

	mRegister_32 r1(new Register_32);
	mRegister_32 r2(new Register_32);
	mRegister_32 r3(new Register_32);

	r1->connect_input(r2);
	r2->connect_input(r3);
	r3->connect_input(r1);

	r1->force_current_state(500);
	r2->force_current_state(1000);
	r3->force_current_state(2000);

	// Connect clock signals
	clk_1.connect(r1);
	clk_1.connect(r3);
	clk_1.connect(r2); // Note: R2 is updated at a clock frequency twice as slow as r1 and r3

	mPHDL_Execution_Engine eng(new PHDL_Sequential_Execution_Engine);
	clk_1.setExecutionEngine(eng);

	// Cycle 1
	BW_32 data_1;
	BW_32 data_2;
	BW_32 data_3;

	clk_1.base_cycle();
	data_1 = **r1;
	data_2 = **r2;
	data_3 = **r3;
	assertEquals(data_1.AsInt32(), 1000); // State update
	assertEquals(data_2.AsInt32(), 2000); // State update
	assertEquals(data_3.AsInt32(), 500); // State update

	// Cycle 2
	clk_1.base_cycle();
	data_1 = **r1;
	data_2 = **r2;
	data_3 = **r3;
	assertEquals(data_1.AsInt32(), 2000); // State update
	assertEquals(data_2.AsInt32(), 500); // R2 should of kept its state
	assertEquals(data_3.AsInt32(), 1000); // State update

}