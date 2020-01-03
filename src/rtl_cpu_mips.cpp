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
#include "rtl_cpu_mips.h"

namespace priscas
{
	void RTLB_mips32_sc::fetch()
	{
		// Read from the PC
		//curr_pc = pc_read_bus.read();
		
		// Prepare "next" PC (regardless if a branch is taken, PC+4 always occurs)
		pc_plus_4 = (curr_pc.AsInt32() + 4 % m.get_size());

		// Recover instruction from zero cycle memory
		/*inst = BW_32(
			m[curr_pc.AsUInt32()],
			m[curr_pc.AsUInt32() + 1],
			m[curr_pc.AsUInt32() + 2],
			m[curr_pc.AsUInt32() + 3]
		);*/
	}

	void mips_decoding_unit_32::decode(BW_32 inst)
	{
		int32_t inst_word = inst.AsInt32();

		// -Masks-
		int32_t opcode_mask = (~(1 << 26)) + 1;
		int32_t rs_mask = ~( ((~(1 << 26)) + 1) | ((1 << 21) - 1));
		int32_t rt_mask = ~( ((~(1 << 21)) + 1) | ((1 << 16) - 1));
		int32_t rd_mask = ~( ((~(1 << 16)) + 1) | ((1 << 11) - 1));
		int32_t funct_mask = 63;
		int32_t shamt_mask = (1 << 11) - 1 - funct_mask;
		int32_t imm_mask_i = (1 << 16) - 1;
		int32_t addr_mask = (1 << 26) - 1;

		// - Actual values
		MIPS_32::opcode op = static_cast<MIPS_32::opcode>(((opcode_mask & inst_word) >> 26) & ((1 << 6) - 1));
		MIPS_32::format fm;
		int rs;
		int rt;
		int rd;
		MIPS_32::funct func;
		int shamt;
		int imm;

		// Set a mode based on OP
		if(op == MIPS_32::R_FORMAT) fm = MIPS_32::R;
		else if(MIPS_32::j_inst(static_cast<MIPS_32::opcode>(op))) fm = MIPS_32::J;
		else fm = MIPS_32::I;

		// Then decode!
		rs = (rs_mask & inst_word) >> 21;
		rt = (rt_mask & inst_word) >> 16;
		rd = (rd_mask & inst_word) >> 11;
		func = static_cast<MIPS_32::funct>((funct_mask & inst_word));
		shamt = (shamt_mask & inst_word) >> 6;
		imm = fm == MIPS_32::I	? (imm_mask_i & inst_word) | (~(inst_word & (1 << 15)) + 1)
						: (addr_mask & inst_word) | (~(inst_word & (1 << 25)) + 1); // make it signed

	}

	void RTLB_mips32_sc::decode()
	{

		mips_decoding_unit_32 decoding_unit;
		decoding_unit.decode(inst);

		// Set the write enable accordingly
		bool reg_we = MIPS_32::reg_write_inst(op, func);

		// Get the reg destination
		int r_write = op == MIPS_32::JAL ? MIPS_32::getRegDst(rs, rt, op) : 31;

		// Set read / write destinations accordingly
		//this->reg_file_read_addr_1_bus.drive(BW_32(rs));
		//this->reg_file_read_addr_2_bus.drive(BW_32(rt));
		//this->reg_file_write_addr_bus.drive(BW_32(r_write));
	}

	void RTLB_mips32_sc::execute()
	{
		// Execute
		BW_32 reg_wdata = 0;
		BW_32 branch_addr = imm << 2;

		// Set PC source accordingly.
		enum PCSrc_t
		{
			PCPLUS4 = 0,
			BRANCH = 1,
			JR = 2
		};

		PCSrc_t PCSrc = PCPLUS4;

	/*	// find correct format based on opcode
		switch(fm)
		{
			case MIPS_32::R:
				
				// For R instructions:
				//   Port 1: rs
				//   Port 2: rt
				switch(func)
				{
					case MIPS_32::ADD:
						reg_wdata = (this->reg_file_read_data_1_bus.read().AsInt32() + this->reg_file_read_data_2_bus.read().AsInt32());
						break;
					case MIPS_32::ADDU:
						reg_wdata = (this->reg_file_read_data_1_bus.read().AsUInt32() + this->reg_file_read_data_2_bus.read().AsUInt32());
						break;
					case MIPS_32::JR:
						jr_pc = (this->reg_file_read_data_1_bus.read().AsUInt32());
						PCSrc = JR;
						break;
					case MIPS_32::OR:
						reg_wdata = (this->reg_file_read_data_1_bus.read().AsInt32() | this->reg_file_read_data_2_bus.read().AsInt32());
						break;
					case MIPS_32::NOR:
						reg_wdata = ~(this->reg_file_read_data_1_bus.read().AsInt32() | this->reg_file_read_data_2_bus.read().AsInt32());
						break;
					case MIPS_32::AND: 
						reg_wdata = (this->reg_file_read_data_1_bus.read().AsInt32() & this->reg_file_read_data_2_bus.read().AsInt32());
						break;
					case MIPS_32::SLL:
						reg_wdata = this->reg_file_read_data_1_bus.read().AsInt32() << shamt;
						break;
					case MIPS_32::SRL:
						reg_wdata = this->reg_file_read_data_1_bus.read().AsInt32() >> shamt;
						reg_wdata = (reg_wdata.AsInt32() & ((1 << (32 - shamt)) - 1)); // make it a logical shift
						break;
					case MIPS_32::SLT:
						reg_wdata = this->reg_file_read_data_1_bus.read().AsInt32() < this->reg_file_read_data_2_bus.read().AsInt32() ? 1 : 0;
						break;
					case MIPS_32::SLTU:
						reg_wdata = this->reg_file_read_data_1_bus.read().AsUInt32() < this->reg_file_read_data_2_bus.read().AsUInt32() ? 1 : 0;
						break;
					case MIPS_32::SUB:
						reg_wdata = (this->reg_file_read_data_1_bus.read().AsInt32() - this->reg_file_read_data_2_bus.read().AsInt32());
						break;
					case MIPS_32::SUBU:
						reg_wdata = (this->reg_file_read_data_1_bus.read().AsUInt32() - this->reg_file_read_data_2_bus.read().AsUInt32());
						break;
				}

				break;
				
			case MIPS_32::I:
				switch(op)
				{
					case MIPS_32::ADDI:
						reg_wdata = this->reg_file_read_data_1_bus.read().AsInt32() + imm;
						break;
					case MIPS_32::ADDIU:
						reg_wdata = this->reg_file_read_data_1_bus.read().AsUInt32() + imm + static_cast<unsigned int>(imm);
						break;
					case MIPS_32::BEQ:
						if(this->reg_file_read_data_1_bus.read().AsInt32() == this->reg_file_read_data_2_bus.read().AsInt32())
						{
							branch_target = pc_plus_4.AsInt32() + branch_addr.AsInt32();
							PCSrc = BRANCH;
						}

						break;
					case MIPS_32::BNE:
						if(this->reg_file_read_data_1_bus.read().AsInt32() != this->reg_file_read_data_1_bus.read().AsInt32())
						{
							branch_target = (pc_plus_4.AsInt32() + branch_addr.AsInt32());
							PCSrc = BRANCH;
						}
						break;
					case MIPS_32::ORI:
						reg_wdata = this->reg_file_read_data_1_bus.read().AsInt32() | imm;
						break;
					case MIPS_32::ANDI:
						reg_wdata = this->reg_file_read_data_1_bus.read().AsInt32() & imm;
						break;
					case MIPS_32::XORI:
						reg_wdata = this->reg_file_read_data_1_bus.read().AsInt32() ^ imm;
						break;*/
/*					case MIPS_32::LBU:
						{
						byte_8b l_word_p_1 = this->mem_req_load(imm + this->registers[rs].get_data().AsInt32());
						BW_32 load_write = 0;
						load_write = load_write.AsUInt32() | (l_word_p_1);
						reg_wdata = load_write;
						r_write = rt;
						}
						break;
					case MIPS_32::LHU:
						{
						byte_8b l_word_p_1 = this->mem_req_load(imm + this->registers[rs].get_data().AsInt32());
						byte_8b l_word_p_2 = this->mem_req_load(imm + this->registers[rs].get_data().AsInt32() + 1);
						BW_32 load_write = 0;
						load_write.AsUInt32() |= l_word_p_1;
						load_write.AsUInt32() |= (l_word_p_2 << 8);
						reg_wdata = load_write;
						r_write = rt;
						}
						break;
					case MIPS_32::LW:
						{
						byte_8b l_word_p_1 = this->mem_req_load(imm + this->registers[rs].get_data().AsInt32());
						byte_8b l_word_p_2 = this->mem_req_load(imm + this->registers[rs].get_data().AsInt32() + 1);
						byte_8b l_word_p_3 = this->mem_req_load(imm + this->registers[rs].get_data().AsInt32() + 2);
						byte_8b l_word_p_4 = this->mem_req_load(imm + this->registers[rs].get_data().AsInt32() + 3);
						BW_32 load_write = 0;
						load_write.AsInt32() |= l_word_p_1;
						load_write.AsInt32() |= (l_word_p_2 << 8);
						load_write.AsInt32() |= (l_word_p_3 << 16);
						load_write.AsInt32() |= (l_word_p_4 << 24);
						reg_wdata = load_write;
						r_write = rt;
						}
						break;
					case MIPS_32::SB:
						{
						char s_word_p_1 = (this->registers[rt].get_data().AsInt32() & ((1 << 8) - 1));
						this->mem_req_write(s_word_p_1, this->registers[rs].get_data().AsInt32() + imm);
						reg_we = false;
						}
						break;
					case MIPS_32::SLTI:
						reg_wdata = this->registers[rs].get_data().AsInt32() < imm ? 1 : 0;
						r_write = rt;
						break;
					case MIPS_32::SLTIU:
						reg_wdata = this->registers[rs].get_data().AsUInt32() < static_cast<uint32_t>(imm) ? 1 : 0;
						r_write = rt;
						break;
					case MIPS_32::SH:
						{
						byte_8b s_word_p_1 = (this->registers[rt].get_data().AsInt32() & ((1 << 8) - 1));
						byte_8b s_word_p_2 = ((this->registers[rt].get_data().AsInt32() >> 8) & ((1 << 8) - 1));
						this->mem_req_write(s_word_p_1, this->registers[rs].get_data().AsInt32() + imm);
						this->mem_req_write(s_word_p_2, this->registers[rs].get_data().AsInt32() + 1 + imm);
						reg_we = false;
						}
						break;
					case MIPS_32::SW:
						{
						byte_8b s_word_p_1 = (this->registers[rt].get_data().AsInt32() & ((1 << 8) - 1));
						byte_8b s_word_p_2 = ((this->registers[rt].get_data().AsInt32() >> 8) & ((1 << 8) - 1) );
						byte_8b s_word_p_3 = ((this->registers[rt].get_data().AsInt32() >> 16) & ((1 << 8) - 1) );
						byte_8b s_word_p_4 = ((this->registers[rt].get_data().AsInt32() >> 24) & ((1 << 8) - 1) );
						this->mem_req_write(s_word_p_1, this->registers[rs].get_data().AsInt32() + imm);
						this->mem_req_write(s_word_p_2, this->registers[rs].get_data().AsInt32() + 1 + imm);
						this->mem_req_write(s_word_p_3, this->registers[rs].get_data().AsInt32() + 2 + imm);
						this->mem_req_write(s_word_p_4, this->registers[rs].get_data().AsInt32() + 3 + imm);
						reg_we = false;
						}
						break;*/
		//		}
		/*		break;

				case MIPS_32::J:
				{
					int32_t jump_mask = ~((1 << 28) - 1);

					switch(op)
					{
						case MIPS_32::JAL:
							//this->reg_file_write_data_bus.drive(curr_pc.AsInt32() + 8);
						case MIPS_32::JUMP:
							branch_target = ((pc_plus_4.AsInt32() & jump_mask) | (imm << 2));
							PCSrc = BRANCH;
							break;
					}
				}
				break;
		}*/

		// Set next PC
		next_pc =	PCSrc == PCPLUS4 ? pc_plus_4 :
					PCSrc == JR ? jr_pc :
					PCSrc == BRANCH ? branch_target :
					-1;

		//pc_write_bus.drive(next_pc);

		// Todo: maybe separate Write Back and Memory
	}
}