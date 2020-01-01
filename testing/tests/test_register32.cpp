#include "basic_assert.h"
#include "primitives.h"

/* Register_32
 * Unit Test
 */
void test_register_32()
{
	using namespace priscas;
	
	// Test immediate.
	BW_32 value;

	// Basic state tests.
	Register_32 r;
	value = *r;
	assertEquals(value.AsInt32(), 0); // Test that register is auto initialized.

	// Test that undriven register implicitly holds state
	r.prologue();
	value = *r;
	assertEquals(value.AsInt32(), 0);
	r.epilogue();
	value = *r;
	assertEquals(value.AsInt32(), 0);

	// Okay, now lets implement a basic logic, with the register
	// taking in some value, having a cycle change, and seeing that the value changes only when needed.
	r <= 100; // behavorial set
	value = *r;
	assertEquals(value.AsInt32(), 0);
	r.epilogue();
	value = *r;
	assertEquals(value.AsInt32(), 100);

}