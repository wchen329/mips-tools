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
#ifndef __DIAG_CPU_H__
#define __DIAG_CPU_H__
#include "cpu.h"
#include "cpu_option.h"
#include "debug_view.h"
#include "mips.h"
#include "primitives.h"
#include "priscas_global.h"

namespace priscas
{
	typedef uint64_t InstCount; // Instruction Count

	// CPU with Diagnostic Functions
	class LINK_DE diag_cpu : public cpu
	{
		public:

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
				 * Virtual destructor. Allows manual destructor for other diagnostic CPUs in general.
				 */
				virtual ~diag_cpu() {}
	};
}
#endif
