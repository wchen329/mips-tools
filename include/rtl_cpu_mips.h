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
#ifndef __RTL_CPU_MIPS_H__
#define __RTL_CPU_MIPS_H__
#include "mips.h"
#include "mmem.h"
#include "phdl.h"
#include "rtl_cpu_common.h"

/* Provides some very basic RTLBranch facilities
 * for MIPS processors
 */
namespace priscas
{
	class RTLB_mips32_sc : protected RTLB_basic_sc
	{
		public:
			RTLB_mips32_sc(
					mmem min
			) :
				m(min),
				RTLB_basic_sc(1)
			{}

		protected:
			void fetch();
			void decode();
			void execute();

		private:
			// Temporaries
			mmem& m;
			BW_32 inst;
			BW_32 curr_pc;
			MIPS_32::format fm;
			MIPS_32::opcode op; 
			int rs; 
			int rt;
			int rd;
			MIPS_32::funct func;
			int32_t shamt;
			int32_t imm;

			// Potential candidates for new PC
			BW_32 jr_pc;
			BW_32 next_pc;
			BW_32 pc_plus_4;
			BW_32 branch_target;

			// PC Ports
			Node pc_read_bus;
			Node pc_write_bus;

			// RF Read Ports
			Node reg_file_read_addr_1_bus; // TODO: shrink these ports down to the actual 5-bit (or something that looks like it)
			Node reg_file_read_addr_2_bus;
			Node reg_file_read_data_1_bus;
			Node reg_file_read_data_2_bus;

			// RF Write Ports
			Node reg_file_write_addr_bus;
			Node reg_file_write_data_bus;
	};
}

#endif