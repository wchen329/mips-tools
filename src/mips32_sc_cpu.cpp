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
		MIPS_32::format fm;
		
		// Fetch
		
		int32_t inst_word = 0;
		BW_32 inst_word_addr = this -> pc.get_data();
		this -> pc.set_data(static_cast<uint32_t>((inst_word_addr.AsUInt32() + 4) % this->mm.get_size()));
		BW_32 inst_part = BW_32(
			this->mem_req_load(inst_word_addr.AsUInt32()),
			this->mem_req_load(inst_word_addr.AsUInt32() + 1),
			this->mem_req_load(inst_word_addr.AsUInt32() + 2),
			this->mem_req_load(inst_word_addr.AsUInt32() + 3)
		);

		inst_word = inst_part.AsInt32();

		// Decode

		MIPS_32::opcode op; 
		int rs; 
		int rt;
		int rd;
		MIPS_32::funct func;
		int32_t shamt;
		int32_t imm;
		MIPS_32::mips_decoding_unit_32 decoding_unit;
		decoding_unit.decode(	inst_word,
								fm,
								op,
								rs,
								rt,
								rd,
								func,
								shamt,
								imm		);

		bool reg_we = true; // find write enable
		int r_write = 0;

		// Execute
		BW_32 reg_wdata = 0;
		BW_32 branch_addr = imm << 2;

		// find correct format based on opcode
		switch(fm)
		{
			case MIPS_32::R:
				
				// Eventually replace these with template functions
				switch(func)
				{
					case MIPS_32::ADD:
						reg_wdata = (this->registers[rs].get_data().AsInt32() + this->registers[rt].get_data().AsInt32());
						r_write = rd;
						break;
					case MIPS_32::ADDU:
						reg_wdata = (this->registers[rs].get_data().AsUInt32() + this->registers[rt].get_data().AsUInt32());
						r_write = rd;
						break;
					case MIPS_32::JR:
						this->pc.set_data(this->registers[rs].get_data().AsInt32());
						break;
					case MIPS_32::OR:
						reg_wdata = (this->registers[rs] | this->registers[rt]).get_data().AsInt32();
						r_write = rd;
						break;
					case MIPS_32::NOR:
						reg_wdata = ~(this->registers[rs] | this->registers[rt]).get_data().AsInt32();
						r_write = rd;
						break;
					case MIPS_32::AND: 
						reg_wdata = (this->registers[rs] & this->registers[rt]).get_data().AsInt32();
						r_write = rd;
						break;
					case MIPS_32::SLL:
						reg_wdata = this->registers[rs].get_data().AsInt32() << shamt;
						r_write = rd;
						break;
					case MIPS_32::SRL:
						reg_wdata = this->registers[rs].get_data().AsInt32() >> shamt;
						reg_wdata = (reg_wdata.AsInt32() & ((1 << (32 - shamt)) - 1)); // make it a logical shift
						r_write = rd;
						break;
					case MIPS_32::SLT:
						reg_wdata = this->registers[rs].get_data().AsInt32() < this->registers[rt].get_data().AsInt32() ? 1 : 0;
						r_write = rd;
						break;
					case MIPS_32::SLTU:
						reg_wdata = this->registers[rs].get_data().AsUInt32() < this->registers[rt].get_data().AsUInt32() ? 1 : 0;
						r_write = rd;
						break;
					case MIPS_32::SUB:
						reg_wdata = (this->registers[rs].get_data().AsInt32() - this->registers[rt].get_data().AsInt32());
						r_write = rd;
						break;
					case MIPS_32::SUBU:
						reg_wdata = (this->registers[rs].get_data().AsUInt32() - this->registers[rt].get_data().AsUInt32());
						r_write = rd;
						break;
				}

				break;
				
			case MIPS_32::I:
				switch(op)
				{
					case MIPS_32::ADDI:
						reg_wdata = this->registers[rs].get_data().AsInt32() + imm;
						r_write = rt;
						break;
					case MIPS_32::ADDIU:
						reg_wdata = this->registers[rs].get_data().AsUInt32() + static_cast<unsigned int>(imm);
						r_write = rt;
						break;
					case MIPS_32::BEQ:
						reg_we = false;
						if(this->registers[rs].get_data().AsInt32() == this->registers[rt].get_data().AsInt32())
						{
							BW_32 curr_pc = this->get_PC();
							this->pc.set_data(curr_pc.AsInt32() + branch_addr.AsInt32());
						}

						break;
					case MIPS_32::BNE:
						reg_we = false;
						if(this->registers[rs].get_data().AsInt32() != this->registers[rt].get_data().AsInt32())
						{
							BW_32 curr_pc = this->get_PC();
							this->pc.set_data(curr_pc.AsInt32() + branch_addr.AsInt32());
						}
						break;
					case MIPS_32::ORI:
						reg_wdata = this->registers[rs].get_data().AsInt32() | imm;
						r_write = rt;
						break;
					case MIPS_32::ANDI:
						reg_wdata = this->registers[rs].get_data().AsInt32() & imm;
						r_write = rt;
						break;
					case MIPS_32::XORI:
						reg_wdata = this->registers[rs].get_data().AsInt32() ^ imm;
						r_write = rt;
						break;
					case MIPS_32::LBU:
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
						break;
				}
				break;

				case MIPS_32::J:
				{
					int32_t jump_mask = ~((1 << 28) - 1);

					switch(op)
					{
						case MIPS_32::JUMP:
							pc.set_data((pc.get_data().AsInt32() & jump_mask) | (imm << 2));
							break;
						case MIPS_32::JAL:
							this->registers[31].set_data(pc.get_data().AsInt32() + 4); // Add 4, since it has already been incremented once
							pc.set_data((pc.get_data().AsInt32() & jump_mask) | (imm << 2));
							break;
					}
				}
				break;
		}

		// Write Back
		if(reg_we && r_write != 0)
		{
			this->registers[r_write].set_data(reg_wdata);
		}
		
		// There are no no-ops in the single cycle CPU, so assume that every cycle one valid instruction is fetched and executed (for now, until MEM delay is implemented)
		++this->comcount;

		return true;
	}

	void mips32_sc_cpu::rst()
	{
		for(int i = 0; i < 32; i++)
		{
			this->registers[i].set_data(0);
		}

		this->pc.set_data(0);
	}

	void mips32_sc_cpu::mem_req_write(byte_8b data, int index)
	{
		this->mm[index % this->mm.get_size()] = data;
	}

	byte_8b mips32_sc_cpu::mem_req_load(int index)
	{
		return this->mm[index % this->mm.get_size()];
	}
}
