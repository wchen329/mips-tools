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
#ifndef __CPU_H__
#define __CPU_H__
#include "ISA.h"

namespace mips_tools
{

	/* Contains interface information concerning making a
	 * CPU object, abstract class
	 *
	 * wchen329
	 */
	class cpu
	{
		public:
			virtual void rst() = 0;		// zeroes out the processor state
			virtual bool cycle() = 0;	// advance clock period by one cycle 
										// returns a boolean TRUE if HLT has been fetched, FALSE if HLT has not (or an EOF)
			virtual long get_clk_T() = 0; // get clock period in picoseconds
			virtual ISA& get_ISA() = 0;		// get ISA
	};

}
#endif
