#include <cstring>
#include "mb.h"
#include "cpu.h"
#include "sc_cpu.h"

namespace mips_tools
{
	void mb::dc_on()
	{
		// Initialize registers and main memory to 0 values
		this->mb_cpu->rst();

		// Start fetch decoding and executing until hlt is received
		while(this->mb_cpu->cycle() == false);
	}

	size_t mb::get_mmem_size()
	{
		return this->mb_mmem->get_size();
	}

	void mb::reset()
	{
		this->mb_cpu->rst();
		memset(this->mb_mmem->begin(), 0, this->get_mmem_size());
	}

	void mb::step()
	{
		this->mb_cpu->cycle();
	}

	cpu& mb::get_cpu()
	{
		return *(this->mb_cpu);
	}

	mb::mb(cpu_t ct, int mt): cpu_type(ct), mmem_type(mt)
	{
		size_t s = 1 << mt;
		this->mb_mmem = new mmem(s);
		this->mb_cpu = new sc_cpu;
	}

	mb::~mb()
	{
		delete (this->mb_mmem);
		delete (this->mb_cpu);
	}
} 