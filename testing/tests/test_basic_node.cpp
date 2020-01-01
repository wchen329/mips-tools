#include "basic_assert.h"
#include "phdl.h"

/* Unit Test
 * for Node
 * (integration test with Register_32)
 */
void test_basic_node()
{
	using namespace priscas;

	// Test with basic node
	// Simply two register feeding 
	// each other
	// -> r1 - [ n1] -> r2 ----
	// |                      |
	// ---------[ n2] ---------
	
	// Registers
	Register_32 r1;
	r1.force_current_state(50);
	mDrivable r1_out(new Bus_32());
	mDrivable r1_in(new Bus_32());

	Register_32 r2;
	r2.force_current_state(100);
	mDrivable r2_out(new Bus_32());
	mDrivable r2_in(new Bus_32());

	// Nodes
	Node n1;
	n1.connect(r1_out);
	n1.connect(r2_in);
	Node n2;
	n2.connect(r2_out);
	n2.connect(r1_out);

	// Cycle 0: r1 == 50, r2 == 100
	const BW_32& dbg_r1 = r1.get_current_state();
	const BW_32& dbg_r2 = r2.get_current_state();
	assertEquals(dbg_r1.AsInt32(), 50);
	assertEquals(dbg_r2.AsInt32(), 100);

	// Cycle 1: r1 = 100, r2 = 50;
	r1.prologue(); r2.prologue(); // Perform prologue of each
	r1.epilogue(); r2.epilogue(); // Then perform epilogue
	assertEquals(dbg_r1.AsInt32(), 100);
	assertEquals(dbg_r2.AsInt32(), 50);

}