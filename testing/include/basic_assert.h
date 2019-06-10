#ifndef __BASIC_ASSERT_H__
#define __BASIC_ASSERT_H__
#include <cassert>

	#define assertEquals(arg_1, arg_2) assert(arg_1 == arg_2);
	#define assertNotEquals(arg_1, arg_2) assert(arg_1 != arg_2);
	#define assertLessThan(arg_1, arg_2) assert(arg_1 < arg_2);
	#define assertGreaterThan(arg_1, arg_2) assert(arg_1 > arg_2);
	#define assertNotZero(arg_1) assert(arg_1 != 0);
	#define assertIsNullPtr(arg_1) assert(arg_1 == nullptr);
	#define assertNotNullPtr(arg_1) assert(arg_1 != nullptr);

#endif