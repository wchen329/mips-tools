//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#include "mb.h"

namespace priscas
{
	void mb::dc_on()
	{
		// Initialize registers and main memory to 0 values
		this->mb_cpu->rst();
	}

	size_t mb::get_mmem_size()
	{
		return this->mb_mmem->get_size();
	}

	void mb::reset()
	{
		this->cycle_ct = 0;
		this->sim_time = CPUTime();
		this->mb_cpu->rst();
		memset(this->mb_mmem->begin(), 0, this->get_mmem_size());
	}

	void mb::step()
	{
		// Step the base clock
		this->base_clock.cycle();

		// First we step each additional clock on the motherboard
		for(Clock_Vec::iterator cvi = clock_signals.begin(); cvi != clock_signals.end(); ++cvi)
		{
			cvi->base_cycle();
		}
		
		// Then we evaluate the cycle, using the execution engine.
		this->execeng->start();

		// Then we execute further "cycling" in the cpu.
		// Please note that this is a tad different from clock cycling
		// This is for diagnostic stuff which may require the CPU to be
		// in a known good state.
		// TODO: change this to cpu->debug_cycle()
		this->mb_cpu->cycle();

		// Increase cycle count
		this->cycle_ct++;

		// Record the time spent in the simulator world
		this->sim_time += mb_cpu -> get_clk_T();
	}

	char mb::DMA_read(int addr)
	{
		int real_addr = addr % this->get_mmem_size();
		return mb_mmem->begin()[real_addr];
	}

	void mb::DMA_write(char w, int addr)
	{
		int real_addr = addr % this->get_mmem_size();
		this->mb_mmem->begin()[real_addr] = w;
	}

	cpu& mb::get_cpu()
	{
		return *(this->mb_cpu);
	}

	mb::mb(cpu_t ct, int mt) : cpu_type(ct), mmem_type(mt), sim_time(CPUTime()), cycle_ct(0), execeng(new PHDL_Sequential_Execution_Engine)
	{
		
		size_t s = 1 << mt;
		this->mb_mmem = new mmem(s);
		switch(ct)
		{
			case STANDARD:
				this->mb_cpu = new mips32_sc_cpu(*this->mb_mmem, this->base_clock);
				break;
			case FIVE_P:
				this->mb_cpu = new mips32_sc_cpu(*this->mb_mmem, this->base_clock);
				break;
			case SUPERSCALAR:
				this->mb_cpu = new r10k_superscalar(*this->mb_mmem, this->base_clock);
				break;
		}

		// Register the execution engine with the Clock signals
		this->base_clock.setExecutionEngine(execeng);
	}

	mb::~mb()
	{
		delete (this->mb_mmem);
		delete (this->mb_cpu);
	}
} 
