#include <memory>
#include "basic_assert.h"

#ifndef WIN32
int main()
{
#else
void test_assert()
{
#endif
	asserts::assertEquals<int>(6, 6);
	asserts::assertNotEquals<int>(7, 5);
	asserts::assertGreaterThan<int>(8, 1);
	asserts::assertLessThan<int>(-10, -3);
	asserts::assertNotZero<int>(1);
	
	std::auto_ptr<char> cp(new char);
	asserts::assertNotNullPtr<void*>(cp.get());
	asserts::assertIsNullPtr<void*>(nullptr);
}