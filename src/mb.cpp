#include "mb.h"
#include "cpu.h"
#include "sc_cpu.h"

namespace mips_tools
{
	void mb::dc_on()
	{
		// Initialize registers and main memory to 0 values
		this->mb_cpu.rst();

		// Start fetch decoding and executing until hlt is received
		while(this->mb_cpu.cycle() == false);
	}

	void mb::reset()
	{
		this->mb_cpu.rst();
	}

	void mb::step()
	{
		this->mb_cpu.cycle();
	}

	sc_cpu& mb::get_cpu()
	{
		return this->mb_cpu;
	}

	mb::mb(cpu_t ct, int mt): cpu_type(ct), mmem_type(mt) 
	{
		
	}
} 