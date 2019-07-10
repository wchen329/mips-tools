#include "reg_32.h"
namespace mips_tools
{
	reg_32 operator|(reg_32 r_1, reg_32 r_2)
	{
		reg_32 r_3;
		r_3.set_data(r_1.get_data().AsInt32() | r_2.get_data().AsInt32());
		return r_3;
	}
	reg_32 operator&(reg_32 r_1, reg_32 r_2)
	{
		reg_32 r_3;
		r_3.set_data(r_1.get_data().AsInt32() & r_2.get_data().AsInt32());
		return r_3;
	}

	reg_32 operator^(reg_32 r_1, reg_32 r_2)
	{
		reg_32 r_3;
		r_3.set_data(r_1.get_data().AsInt32() ^ r_2.get_data().AsInt32());
		return r_3;
	}
}