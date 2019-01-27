#ifndef __REG_32_H__
#define __REG_32_H__

/* 32 bit register
 */
#include "primitives.h"

namespace mips_tools
{

	class reg_32
	{
		public:
			void set_data(BW_32 data) { this -> data = data; }
			BW_32 get_data() { return this -> data; }

		private:
			BW_32 data;
	};

	reg_32 operator+(reg_32 r_1, reg_32 r_2);
}

#endif