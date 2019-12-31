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
#ifndef __REG_PIPELINE_H__
#define __REG_PIPELINE_H__
#include "mips.h"
#include "priscas_global.h"

namespace priscas
{
	/* DE-EX
	 * Pipeline Register
	 */
	class decode_ex_plr
	{
		public:
			void load(	BW_32 data_rs,
						BW_32 data_rt,
						MIPS_32::funct func,
						BW_32 shamt,
						BW_32 imm,
						MIPS_32::opcode op,
						bool regWE,
						bool memWE,
						bool memRE,
						int rs,
						int rt,
						int rd );
			void get(	BW_32& data_rs,
						BW_32& data_rt,
						MIPS_32::funct& func,
						BW_32& shamt,
						BW_32& imm,
						MIPS_32::opcode& op,
						bool& regWE,
						bool& memWE,
						bool& memRE,
						int& rs,
						int& rt,
						int& rd );
		private:
			BW_32 data_rs;
			BW_32 data_rt;
			MIPS_32::funct func;
			BW_32 shamt;
			BW_32 imm;
			MIPS_32::opcode op;
			bool regWE;
			bool memWE;
			bool memRE;
			int rs;
			int rt;
			int rd;
	};

	/* EX-MEM
	 * Pipeline Register
	 */
	class ex_mem_plr
	{
		public:
			void load(	BW_32 data_ALU,
						BW_32 data_rs,
						BW_32 data_rt,
						MIPS_32::opcode op,
						bool regWE,
						bool memWE,
						bool memRE,
						int rs,
						int rt,
						int rd );
			void get(	BW_32& data_ALU,
						BW_32& data_rs,
						BW_32& data_rt,
						MIPS_32::opcode& op,
						bool& regWE,
						bool& memWE,
						bool& memRE,
						int& rs,
						int& rt,
						int& rd );
		private:
			BW_32 data_ALU;
			BW_32 data_rs;
			BW_32 data_rt;
			MIPS_32::opcode op;
			bool regWE;
			bool memWE;
			bool memRE;
			int rs;
			int rt;
			int rd;
	};

	/* MEM-WB
	 * Pipeline Register
	 */
	class mem_wb_plr
	{
		public:
			void load(	BW_32 save_data,
						bool regWE,
						int r_save_num );
			void get(	BW_32& save_data,
						bool& regWE,
						int& r_save_num );
		private:
			BW_32 save_data;
			bool regWE;
			int r_save_num;
	};
}

#endif
