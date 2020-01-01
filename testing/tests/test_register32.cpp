#include "basic_assert.h"
#include "primitives.h"

void test_register_32()
{
	using namespace priscas;
	
	// Basic state tests.
	Register_32 r;

	BW_32 value = r.get_current_state();
	assertEquals(value.AsInt32(), 150);
}