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
#include "mips32_sc_cpu.h"

namespace priscas
{

	bool mips32_sc_cpu::cycle()
	{
		++this->comcount;
		return true;
	}

	void mips32_sc_cpu::rst()
	{
		this->pc->force_current_state(0);
		this->rf->rst();
	}

	void mips32_sc_cpu::mem_req_write(byte_8b data, int index)
	{
		this->mm[index % this->mm.get_size()] = data;
	}

	byte_8b mips32_sc_cpu::mem_req_load(int index)
	{
		return this->mm[index % this->mm.get_size()];
	}

	mips32_sc_cpu::mips32_sc_cpu(mmem & m, Clock& bclk) :
		mm(m),
		pc(new Register_32),
		comcount(0),
		mips32_cpu("Generic MIPS-32 Single Cycle", 200000),
		fu(new mips_single_fetch_unit_32(mm, pc)),
		decodingunit(new mips_decoding_unit_32(fu)),
		rf(new RegisterFile_32_32_2_1(bclk)),
		ALUSrcMux(new Mux_2_1)
	{
		// Connect the PC
		bclk.connect(pc);

		// Connect the Register File
		bclk.connect(rf);

		// Connect the addressing ports from decoding unit to RF
		rf->get_nth_read_addr_port(0)->connect_input(this->decodingunit->get_bus_rs_out());
		rf->get_nth_read_addr_port(1)->connect_input(this->decodingunit->get_bus_rt_out());

		// Connecct ALU mux to the register file
	}
}
