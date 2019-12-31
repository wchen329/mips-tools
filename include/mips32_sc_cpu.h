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
#include "phdl.h"
#include "reg_32.h"
#include "primitives.h"
#include "mmem.h"
#include "mips.h"
#include "mips_cpu.h"
#include "mt_exception.h"
#include "priscas_global.h"

/* Interface for the single cycle CPU implementation
 */
namespace priscas
{

	class mips32_sc_cpu : public mips32_cpu
	{
		public:
			
			/* rst()
			 * Asynchronous reset
			 */
			void rst();
			
			/* cycle()
			 * Update debugging probes.
			 */
			bool cycle();
			
			/* BW_32& get_reg_data(int index)
			 * Get the current state of the index'th register
			 */
			BW_32& get_reg_data(int index) { return this->RegisterFile[index].get_current_state(); }
			
			/* BW_32& get_PC()
			 * Get current state of the PC
			 */
			BW_32& get_PC() { return this->pc.get_current_state(); }
			
			/* Constructor.
			 * Allows CPU to read from/write to meomry
			 * and also sets up timing with any clocks.
			 */
			mips32_sc_cpu(mmem & m, BaseClock& bclk) :
				mm(m),
				comcount(0),
				mips32_cpu("Generic MIPS-32 Single Cycle", 200000)
			{}

			virtual CPU_ControlPanel& get_CPU_options() { return this->cp; }

			virtual std::vector<DebugView*>& get_DebugViews() { return this->debug_views; }
			
			virtual ~mips32_sc_cpu() {}
			
			virtual InstCount get_InstCommitCount() { return comcount; }
	
			byte_8b mem_req_load(int index); // sends a load memory request from CPU to MMEM. The ind is the offset from address 0x0
			
			void mem_req_write(byte_8b data, int index); // sends a write memory request from CPU To MMEM. The ind is the offset from address 0x0

		private:
			
			// Disabled copy constructor
			mips32_sc_cpu(mips32_sc_cpu&);
			mips32_sc_cpu operator=(mips32_sc_cpu&);

			CPU_ControlPanel cp;

			std::vector<DebugView*> debug_views;
			
			// Statistics
			InstCount comcount; // amount of instructions committed (since last reset)

			/* Non-Memory CPU State 
			 */
			static const int REG_COUNT = 32;
			Register_32 RegisterFile[REG_COUNT];
			Register_32 pc;

			/* Memory state
			 * (as reference)
			 */
			mmem & mm;
	};
}
#endif
