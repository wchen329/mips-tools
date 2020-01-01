#include "basic_assert.h"
#include "phdl.h"

/* Unit Test
 * for Node
 * (integration test with Register_32)
 */
void test_basic_node()
{
	using namespace priscas;

	// Registers

	// Test with implicit node.
	// That is, connect r11 directly to r12 and r12 to r11
	//
	// --> r11 --> r12 -v
	// ^-----------------
	mRegister_32 r11 = mRegister_32(new Register_32);
	mRegister_32 r12 = mRegister_32(new Register_32);
	r11->force_current_state(50);
	r12->force_current_state(100);

	r11->connect_input(r12);
	r12->connect_input(r11);

	r11->prologue(); r12->prologue();
	r11->epilogue(); r12->epilogue();
	const BW_32& dbg_r11 = r11->get_current_state();
	const BW_32& dbg_r12 = r12->get_current_state();
	assertEquals(dbg_r11.AsInt32(), 100);
	assertEquals(dbg_r12.AsInt32(), 50);

	// Test with basic node
	// Simply two register feeding 
	// each other
	// -> r1 - [ n1] -> r2 ----
	// |                      |
	// ---------[ n2] ---------
	mRegister_32 r1 = mRegister_32(new Register_32);
	mRegister_32 r2 = mRegister_32(new Register_32);

	mNode n1 = mNode(new Node());
	mNode n2 = mNode(new Node());

	// Set registers
	r1->force_current_state(50);
	r2->force_current_state(100);

	// Nodes
	n1->connect_input(r1);
	r2->connect_input(n1);
	n2->connect_input(r2);
	r1->connect_input(n2);

	// Cycle 0: r1 == 50, r2 == 100
	const BW_32& dbg_r1 = r1->get_current_state();
	const BW_32& dbg_r2 = r2->get_current_state();
	assertEquals(dbg_r1.AsInt32(), 50);
	assertEquals(dbg_r2.AsInt32(), 100);

	// Cycle 1: r1 = 100, r2 = 50;
	r1->prologue(); r2->prologue(); // Perform prologue of each
	n1->drive(); n2->drive();
	r1->epilogue(); r2->epilogue(); // Then perform epilogue
	assertEquals(dbg_r1.AsInt32(), 100);
	assertEquals(dbg_r2.AsInt32(), 50);

}