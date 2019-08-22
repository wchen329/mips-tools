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
#ifndef __SC_CPU_H__
#define __SC_CPU_H__
#include <memory>
#include "diag_cpu.h"
#include "reg_32.h"
#include "primitives.h"
#include "mmem.h"
#include "mips.h"
#include "mt_exception.h"
#include "priscas_global.h"

/* Interface for the single cycle CPU implementation
 */
namespace priscas
{

	class sc_cpu : public diag_cpu
	{
		public:
			void rst(); // "async" reset
			bool cycle(); // advance the processor a cycle
			BW_32& get_reg_data(int index) { return this->registers[index].get_data(); }
			unsigned get_reg_count() { return REG_COUNT; }
			BW_32& get_PC() { return this->pc.get_data(); }
			long get_clk_T() { return this -> clk_T ; }
			void ghost_cycle();
			sc_cpu(mmem & m) : mm(m), clk_T(200000) {  }
			ISA& get_ISA() { return this->isa; }
			std::vector<CPU_Option>& get_CPU_options() { return this->cpu_opts; }
			void exec_CPU_option(std::vector<NameValueStringPair> &) {}
			virtual std::vector<DebugView*>& get_DebugViews() { return this->debug_views; }
		protected:
			std::vector<CPU_Option> cpu_opts;
			byte_8b mem_req_load(int index); // sends a load memory request from CPU to MMEM. The ind is the offset from address 0x0
			void mem_req_write(byte_8b data, int index); // sends a write memory request from CPU To MMEM. The ind is the offset from address 0x0
			static const int REG_COUNT = 32;
			long clk_T;
			reg_32 registers[REG_COUNT];
			reg_32 pc;
			mmem & mm;
		private:
			std::vector<DebugView*> debug_views;
			sc_cpu(sc_cpu&);
			sc_cpu operator=(sc_cpu&);
			MIPS_32 isa;
	};
}
#endif
