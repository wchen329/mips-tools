#include "sc_cpu.h"

namespace mips_tools
{
	bool sc_cpu::cycle()
	{
		return true;
	}

	void sc_cpu::rst()
	{
		for(int i = 0; i < 32; i++)
		{
			this->registers[i].set_data(0);
		}

		this->pc.set_data(0);
	}
}