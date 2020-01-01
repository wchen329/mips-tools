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
#include "phdl.h"
#include "mmem.h"
#include "primitives.h"
#include "priscas_global.h"
#include "cpu_time.h"
#include "pipeline.h"
#include "mips32_sc_cpu.h"
#include "mips32_ss_cpu.h"
#include "southbridge.h"

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
	class mb
	{
		public:

			/* void dc_on();
			 * Turn on the motherboard, start the processor
			 */
			LINK_DE void dc_on();

			/* void reset();
			 * Reset the CPU and zero main memory.
			 */
			LINK_DE void reset();

			/* void step();
			 * Step the processor a cycle.
			 */
			LINK_DE void step();

			/* void DMA_Write(...)
			 * Direct memory access write to memory
			 */
			LINK_DE void DMA_write(char data, int addr);	// direct memory access write

			/* void DMA_Read(...)
			 * Direct memory access read to memory
			 */
			LINK_DE char DMA_read(int addr);	// direct memory access read

			/* cpu& get_cpu()
			 * Get the CPU
			 * WARNING: this may be const soon... so don't use this
			 * to modify the cpu
			 */
			LINK_DE cpu & get_cpu();		// return cpu reference

			/* mmem& get_mmem()
			 * Get main memory
			 * WARNING: this may be const soon... so don't use this
			 * to modify memory.
			 */
			LINK_DE mmem & get_mmem() { return *this->mb_mmem; }		// return main memory reference

			/* unsigned long get_cycles()
			 * Get the amount of cycles that have been invoked since the motherboard
			 * powered on
			 */
			LINK_DE unsigned long get_cycles() { return this->cycle_ct; };		// return current simulation cycles elapsed

			/* CPUTime get_time()
			 * Get the amount of time that has passed in the CPU
			 */
			LINK_DE CPUTime get_time() { return this->sim_time; } // return the current simulation time elapsed

			/* size_t get_mmem_size()
			 * Get the size of main memory in bytes.
			 */
			LINK_DE size_t get_mmem_size();	// get main memory size in BYTES

			/* mb(...)
			 * Construct a motherboard
			 * Arguments:
			 *		ct - cpu to use
			 *		mt - the width of memory
			 */
			LINK_DE mb(cpu_t ct = STANDARD, int mt = 16);

			/* ~mb()
			 * Destroy the motherboard
			 */
			LINK_DE ~mb();

		private:
			cpu * mb_cpu; // single CPU
			mmem * mb_mmem;  // single main memory
			cpu_t cpu_type;	// types of MIPS processor
			int mmem_type;	// bit length of address space
			CPUTime sim_time;	// current simulation time in seconds
			unsigned long cycle_ct;	// cycle count
			southbridge io_controller; // southbridge a.k.a I/O Controller
			Clock base_clock; // Base Clock signal
			Clock_Vec clock_signals; // Extra clock signals
			
			
	};

	/* I/O Multiplexer Bus
	 * Conditionally routes memory requests to southbridge or northbridge (main memory)
	 * as required.
	 *
	 * If an ISA doesn't support memory mapped I/O, then this can be set so it ALWAYS sends requests
	 * directly to main memory
	 */
	class io_mux
	{
		public:
			void send_write(unsigned long long address, byte_8b data);
			byte_8b send_read(unsigned long long address);
		private:
			mmem& north;
			southbridge& south;
	};
}
#endif
