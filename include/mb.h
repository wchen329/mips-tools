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
#ifndef __MB_H__
#define __MB_H__
#include <cstring>
#include <memory>
#include "cpu.h"
#include "mmem.h"
#include "primitives.h"
#include "priscas_global.h"
#include "osi.h"
#include "cpu_time.h"
#include "pipeline.h"
#include "sc_cpu.h"
#include "superscalar.h"

namespace priscas
{
	/* CPU Types
	 *
	 *
	 */
	enum cpu_t
	{
			STANDARD = 0,
			FIVE_P = 1,
			SUPERSCALAR = 2,
			CUSTOM = 3
	};

	/* Motherboard
	 *
	 * Representation of a computer with a single CPU and a single main memory.
	 *
	 */
	class DLE mb
	{
		public:
			void dc_on();	// power on
			void reset();
			void step();	// step the processor a cycle
			void DMA_write(char data, int addr);	// direct memory access write
			char DMA_read(int addr);	// direct memory access read
			cpu & get_cpu();		// return cpu reference
			mmem & get_mmem() { return *this->mb_mmem; }		// return main memory reference
			unsigned long get_cycles() { return this->cycle_ct; };		// return current simulation cycles elapsed
			CPUTime get_time() { return this->sim_time; } // return the current simulation time elapsed
			size_t get_mmem_size();	// get main memory size in BYTES

			mb(cpu_t ct = STANDARD, int mt = 16);
			~mb();

		private:
			cpu * mb_cpu;	// single socket cpu
			mmem * mb_mmem;  // single main memory
			cpu_t cpu_type;	// types of MIPS processor
			int mmem_type;	// bit length of address space
			CPUTime sim_time;	// current simulation time in seconds
			unsigned long cycle_ct;	// cycle count
			
	};
}
#endif
