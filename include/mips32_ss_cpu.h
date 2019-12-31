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
#ifndef __SUPERSCALAR_H__
#define __SUPERSCALAR_H__

#include <queue>
#include <vector>
#include "primitives.h"
#include "priscas_global.h"
#include "mips32_sc_cpu.h"
#include "mips.h"
#include "reg_32.h"
#include "reg_pipeline.h"

namespace priscas
{

	/* Register Map
	 * A general register map usable for superscalar processors
	 */
	class register_map
	{

		public:
			/* preg_count - physical register count
			 * vreg_count - "virtual" or ISA register count
			 */
			register_map(int preg_count, int vreg_count) : preg_num(preg_count), vreg_num(vreg_count),
				rmap(preg_count)
			{
				for(size_t cont = 0; cont < rmap.size(); cont++)
				{
					rmap[cont] = -1; // -1 means "not currently mapped"
				}
			}

			int getMappingToPReg(int vregNum) { return this->rmap[vregNum]; }
			int setMappingToPReg(int vregNum, int pregNum) { this->rmap[vregNum] = pregNum; }
			int clearMapping(int vregNum) { this->rmap[vregNum] = -1; }

		private:
			int preg_num;
			int vreg_num;
			std::vector<int> rmap;
	};

	/* MIPS R10K Superscalar Processor Clone
	 *
	 */
	class r10k_superscalar : public mips32_sc_cpu
	{

		public:
			void rst();
			bool cycle();
			void exec_CPU_option(std::vector<std::string> &);
                
			/* R10K Superscalar constructor
			 * To make this type of processor some parameters should be specified...
			 * -
			 * inst_buffer_entries - How many instructions will this
			 * 	superscalar processor be able to hold at a time?
			 * (default): 1 entry
			 * -
			 * phys_reg_count - How many physical registers does this
			 * processor have?
			 * (default): 10 registers
			 * -
			 * funit_count - How many functional units does this processor have?
			 * (default): 3 functional units
			 * -
			 * dispatch_buffer_entries - How many instructions can be in dispatch at once?
			 * (default): 5 entries
			 * -
			 * reorder_buf_entries - reorder buffer size
			 * (default): 5 entries
			 */
			r10k_superscalar(mmem& m, int inst_buffer_entries = 1, int phys_reg_count = 10,
				int funit_count = 3, int dispatch_buffer_entries = 5, int reorder_buf_entries = 5); 
		
		private:
			
			std::vector<BW_32> inst_buffer; // variable sized instruction buffer
			std::vector<decode_ex_plr> dispatch_buffer;	// collection of states ready for dispatch
			std::vector<decode_ex_plr> reservation_stations; // collection of states ready for issue AFTER register renaming
			std::vector<reg_32> register_file;	// collection of PHYSICAL registers
			std::queue<int> reg_free_list;	// free list for denoting usable registers
			std::queue<int> reorder_buffer;	// 
	};
}
#endif
