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
#ifndef __ISA_H__
#define __ISA_H__

/* An abstract class describing
 * an interface for an Instruction Set Architecture
 *
 * wchen329
 */
#include <memory>
#include <string>
#include <vector>
#include "syms_table.h"

namespace mips_tools
{
	namespace ISA_Attrib
	{
		enum addrmode
		{
			ADDR_BYTE,
			ADDR_WORD
		};

		enum endian
		{
			CPU_LITTLE_ENDIAN,
			CPU_BIG_ENDIAN
		};
	}

	class ISA
	{
		public:
			virtual unsigned get_reg_count() = 0;			// The amount of registers in this architecture
			virtual unsigned get_address_bit_width() = 0;	// bit width of a PHYSICAL address / PC
			virtual std::string get_reg_name(int id) = 0;	// given a register ID give the "friendly name" of the register
			virtual int get_reg_id(std::string&) = 0;		// similarly, get the register ID given the "friendly name" of the register
			virtual int get_register_bit_width(int id) = 0;	// get the bit width of register of ID
			virtual ISA_Attrib::addrmode get_addressing_mode() = 0;		// get addressing mode
			virtual ISA_Attrib::endian get_endian() = 0;				// get endian
			virtual std::shared_ptr<BW> assemble(std::vector<std::string>& args, BW& baseAddress, syms_table& jump_syms) = 0;
	};
}
#endif
