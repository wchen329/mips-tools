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
#include "superscalar.h"

namespace mips_tools
{
	r10k_superscalar::r10k_superscalar(mmem& m, int inst_buffer_entries, int phys_reg_count,
				int funit_count, int dispatch_buffer_entries, int reorder_buf_entries) 
				: sc_cpu(m)
	{
		
	}

	void r10k_superscalar::rst()
	{
	}

	void r10k_superscalar::exec_CPU_option(std::vector<std::string>& args)
	{

	}

	bool r10k_superscalar::cycle()
	{
		/* Retire
		 * If at head of buffer:
		 *     free old register
		 *     free reorder buffer entry
		 *     (potentially) perform memory writes
		 * else
		 *     stall
		 *
		 */

		/* Complete
		 * Write physical register
		 * Send message, that the physical register is ready
		 */


		/* Execute
		 * Do the operation (which might take more than one cycle). When done, proceed to "Complete"
		 */

		/* Issue
		 * If: available functional units > 0 allocate that functional unit, proceed to execute
		 * else: stall
		 */

		/* Dispatch
		 * Stall if: no reservation station available OR
		 *           reorder buffer full
		 *           no physical registers available
		 * else: perform register renaming and allocate accordingly
		 */

		/* Decode
		 * Stall if dispatch buffer full
		 * Figure out operands of each instruction in the fetch buffer
		 * Branch prediction
		 */


		/* Fetch
		 * while (instruction buffer not full)
		 * {
		 * add new instruction into buffer
		 * increment the PC
		 * }
		 */

		return false;
	}
}
