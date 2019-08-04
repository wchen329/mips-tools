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
//   You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DIAG_CPU_H__
#define __DIAG_CPU_H__
#include <vector>
#include "cpu.h"
#include "mips.h"
#include "primitives.h"

namespace mips_tools
{

	// CPU with Diagnostic Functions
	class diag_cpu : public cpu
	{
		public:
				// still need to do: change BW_32 to a generic
				virtual BW_32& get_reg_data(int index) = 0;
				virtual int get_reg_count() = 0;
				virtual void ghost_cycle() = 0;	// ghost cycle- essentially cycling with a NO-OP, advances the PC.
				virtual BW_32& get_PC() = 0;	// get current PC value
				virtual std::vector<NameDescPair>& get_CPU_options() = 0;
				virtual void exec_CPU_option(std::vector<std::string> &) = 0;	// execute a special CPU specific option
	};
}
#endif
