#include "basic_assert.h"
#include "primitives.h"
#include "reg_32.h"

#ifndef WIN32
int main()
{
#else
void test_register_32()
{
#endif
	using namespace mips_tools;
	reg_32 r;
	r.set_data(150);
	BW_32 value = r.get_data();
	asserts::assertEquals<BW_32>(value, 150);
}