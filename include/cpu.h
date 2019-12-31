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
#ifndef __CPU_H__
#define __CPU_H__
#include "cpu_option.h"
#include "debug_view.h"
#include "debug_view_simple.h"
#include "ISA.h"
#include "priscas_global.h"

namespace priscas
{
	typedef uint64_t InstCount; // Instruction Count

	/* Contains interface information concerning making a
	 * CPU object, abstract class
	 *
	 * wchen329
	 */
	class LINK_DE cpu
	{
		public:

			/* getName()
			 * IMPLEMENTATION: Return the name of the processor
			 * Return: string
			 *         Processor name as a string
			 */
			virtual const UPString& getName() = 0;

			/* rst()
			 * IMPLEMENTATION: Resets the processor state
			 * (zeroes it all out)
			 * Return: nothing
			 */
			virtual void rst() = 0;

			/* cycle()
			 * IMPLEMENTATION: Run the processor for one cycle
			 * Return:	boolean
			 *			True, indicating the processor sends a shutdown signal, False if not (generally returns false)
			 */
			virtual bool cycle() = 0;

			/* get_clk_T()
			 * IMPLEMENTATION: Retrieve clock period of processor (in picoseconds)
			 * Return:	long
			 *			the clock period of the processor in picoseconds
			 */
			virtual long get_clk_T() = 0;

			/* get_ISA()
			 * IMPLEMENTATION: Retrieve a reference to the ISA which the processor
			 * implements
			 * The ISA is read only.
			 * Return:	ISA reference
			 *			reference to processors ISA
			 */
			virtual const ISA& get_ISA() = 0;

			/* get_reg_data(int index)
			 * IMPLEMENTATION: returns the data at the register specified
			 */
			virtual BW_32& get_reg_data(int index) = 0;

			/* get_PC()
			 * IMPLEMENTATION: return the value of the PC
			 */
			virtual BW_32& get_PC() = 0;
				
			/* CPU_ControlPanel& get_CPU_Options()
			 * IMPLEMENTATION: return the CPU control panel
			 */
			virtual CPU_ControlPanel& get_CPU_options() = 0;
				
			/* get_DebugViews()
			 * IMPLEMENATION: return debug views
			 */
			virtual std::vector<DebugView*>& get_DebugViews() = 0;

			/* get_InstCommitCount()
			 * IMPLEMENTATION: return commit count
			 */
			virtual InstCount get_InstCommitCount() = 0;
			
			/* ~diag_cpu()
			 * Virtual destructor. Allows manual destructor for CPUs in general.
			 */
			virtual ~cpu(){};
	};

}
#endif
