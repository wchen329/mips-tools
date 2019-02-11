#include "primitives.h"

namespace mips_tools
{
	BW_32_T::BW_32_T(char b_0, char b_1, char b_2, char b_3)
	{
		char * w_ptr = (char*)&w;
		*w_ptr = b_0;
		*(w_ptr + 1) = b_1;
		*(w_ptr + 2) = b_2;
		*(w_ptr + 3) = b_3;
		long tmp = w;
	}
}