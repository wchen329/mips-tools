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
		fu(new mips_single_fetch_unit_32(m, pc)),
		decodingunit(new mips_decoding_unit_32(fu)),
		rf(new RegisterFile_32_32_2_1(bclk)),
		ALUSrcMux(new Mux_2_1),
		MemToRegMux(new Mux_2_1),
		RegDstMux(new Mux_2_1),
		malu(new mips_alu_32),
		mbr(new mips_branch_resolver_32),
		pc_adder(new nnaryIntAdder32),
		constant_zero(new Register_32),
		constant_four(new Register_32)
	{
		// Connect the PC
		bclk.connect(pc);

		// Connect the Register File
		bclk.connect(rf);
		bclk.connect(constant_zero);
		bclk.connect(constant_four);

		// Connect the PC to the Branch Unit
		pc->connect_input(mbr);
		constant_zero->force_current_state(0);
		constant_four->force_current_state(4);

		// ... And the Branch Unit to the correct signals
		mbr->connect_input(constant_zero);
		pc_adder->connect_input(pc);
		pc_adder->connect_input(constant_four);
		mbr->connect_input(pc_adder);

		// Connect the addressing ports from decoding unit to RF
		rf->get_nth_read_addr_port(0)->connect_input(this->decodingunit->get_bus_rs_out());
		rf->get_nth_read_addr_port(1)->connect_input(this->decodingunit->get_bus_rt_out());

		// Connect write enable signal
		rf->get_nth_write_enable(0)->connect_input(this->decodingunit->get_RegWrite_out());

		// Connect regdst mux
		RegDstMux->connect_input(this->decodingunit->get_RegDst_out());
		RegDstMux->connect_input(this->decodingunit->get_bus_rt_out());
		RegDstMux->connect_input(this->decodingunit->get_bus_rd_out());
		rf->get_nth_write_addr_port(0)->connect_input(RegDstMux);

		// Connect final data to RF write port
		rf->get_nth_write_port(0)->connect_input(MemToRegMux);

		// Connect ALU Src Mux
		this->ALUSrcMux->connect_input(this->decodingunit->get_ALUSrc_out());
		this->ALUSrcMux->connect_input(this->rf->get_nth_read_port(1));
		this->ALUSrcMux->connect_input(this->decodingunit->get_bus_imm_out());

		// Connect ALU bits input: 0 is ALUOp, 1 is funct, 2 is data_1 and 3 is data_2
		malu->connect_input(this->decodingunit->get_ALUOp_out());
		malu->connect_input(this->decodingunit->get_bus_funct_out());
		malu->connect_input(this->rf->get_nth_read_port(0));
		malu->connect_input(this->ALUSrcMux);
		
		// Connect mem to reg mux (write back)
		MemToRegMux->connect_input(this->decodingunit->get_MemToReg_out());
		MemToRegMux->connect_input(this->malu);
		rf->get_nth_write_port(0)->connect_input(MemToRegMux);

	}
}
