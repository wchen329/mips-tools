#ifndef __SC_CPU_H__
#define __SC_CPU_H__
#include "cpu.h"
#include "register_32.h"
#include "mips.h"

namespace mips_tools
{

	class sc_cpu : cpu
	{
		public:
			void rst();
			bool cycle();

		private:
			reg_32 registers[32];
	};
}
#endif