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
#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "mips.h"
#include "reg_32.h"
#include "reg_pipeline.h"
#include "sc_cpu.h"
namespace mips_tools
{
	/* A 5-stage (FS)
	 * MIPS R4000 clone pipelined processor (P)
	 *
	 * wchen329
	 */
	class fsp_cpu : public sc_cpu
	{
		public:
			bool cycle();
			void rst();
			fsp_cpu(mmem & m);
			void exec_CPU_option(std::vector<std::string> &);
		private:
			reg_32 fetch_plr;
			decode_ex_plr de_plr;
			ex_mem_plr em_plr;
			mem_wb_plr mw_plr;
			void flush_fetch_plr() { this->fetch_plr.set_data(0); }
			void flush_em_plr() { this->em_plr.load(0,0,0,static_cast<opcode>(0),false,false,false,0,0,0); }
	};
}

#endif
