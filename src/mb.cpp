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

	mb::mb(cpu_t ct, int mt): cpu_type(ct), mmem_type(mt) 
	{
		
	}
} 