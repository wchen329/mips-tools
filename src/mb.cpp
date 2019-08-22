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

		// Start fetch decoding and executing until hlt is received
		while(!this->suspend)
			this->mb_cpu->cycle();
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
		this->mb_cpu->cycle();
		this->cycle_ct++;
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

	mb::mb(cpu_t ct, int mt, const bool& suspend) : cpu_type(ct), mmem_type(mt), sim_time(CPUTime()), cycle_ct(0), suspend(suspend)
	{
		size_t s = 1 << mt;
		this->mb_mmem = new mmem(s);
		switch(ct)
		{
			case STANDARD:
				this->mb_cpu = new sc_cpu(*this->mb_mmem);
				break;
			case FIVE_P:
				this->mb_cpu = new fsp_cpu(*this->mb_mmem);
				break;
			case SUPERSCALAR:
				this->mb_cpu = new r10k_superscalar(*this->mb_mmem);
				break;
		}
	}

	mb::~mb()
	{
		delete (this->mb_mmem);
		delete (this->mb_cpu);
	}
} 
