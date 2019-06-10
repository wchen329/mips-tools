#ifndef __BASIC_ASSERT_H__
#define __BASIC_ASSERT_H__
#include <cassert>

namespace asserts
{
	template <class C_WITH_EQ> void assertEquals(C_WITH_EQ arg_1, C_WITH_EQ arg_2)
	{
		assert(arg_1 == arg_2);
	}

	template <class C_WITH_NOT_EQ> void assertNotEquals(C_WITH_NOT_EQ arg_1, C_WITH_NOT_EQ arg_2)
	{
		assert(arg_1 != arg_2);
	}

	template <class C_WITH_LT> void assertLessThan(C_WITH_LT arg_1, C_WITH_LT arg_2)
	{
		assert(arg_1 < arg_2);
	}

	template <class C_WITH_GT> void assertGreaterThan(C_WITH_GT arg_1, C_WITH_GT arg_2)
	{
		assert(arg_1 > arg_2);
	}

	template <class C_CAN_BE_ZERO> void assertNotZero(C_CAN_BE_ZERO arg_1)
	{
		assert(arg_1 != 0);
	}

	template <class C_CAN_BE_NULLPTR> void assertIsNullPtr(C_CAN_BE_NULLPTR arg_1)
	{
		assert(arg_1 == nullptr);
	}

	template <class C_CAN_BE_NULLPTR> void assertNotNullPtr(C_CAN_BE_NULLPTR arg_1)
	{
		assert(arg_1 != nullptr);
	}
}

#endif