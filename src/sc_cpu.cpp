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
#include "sc_cpu.h"

namespace mips_tools
{

	bool sc_cpu::cycle()
	{
		format fm;
		
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

		opcode op; 
		int rs; 
		int rt;
		int rd;
		funct func;
		int32_t shamt;
		int32_t imm;
		mips_decoding_unit_32 decoding_unit;
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
			case R:
				
				// Eventually replace these with template functions
				switch(func)
				{
					case ADD:
						reg_wdata = (this->registers[rs].get_data().AsInt32() + this->registers[rt].get_data().AsInt32());
						r_write = rd;
						break;
					case ADDU:
						reg_wdata = (this->registers[rs].get_data().AsUInt32() + this->registers[rt].get_data().AsUInt32());
						r_write = rd;
						break;
					case JR:
						this->pc.set_data(this->registers[rs].get_data().AsInt32());
						break;
					case OR:
						reg_wdata = (this->registers[rs] | this->registers[rt]).get_data().AsInt32();
						r_write = rd;
						break;
					case NOR:
						reg_wdata = ~(this->registers[rs] | this->registers[rt]).get_data().AsInt32();
						r_write = rd;
						break;
					case AND: 
						reg_wdata = (this->registers[rs] & this->registers[rt]).get_data().AsInt32();
						r_write = rd;
						break;
					case SLL:
						reg_wdata = this->registers[rs].get_data().AsInt32() << shamt;
						r_write = rd;
						break;
					case SRL:
						reg_wdata = this->registers[rs].get_data().AsInt32() >> shamt;
						reg_wdata = (reg_wdata.AsInt32() & ((1 << (32 - shamt)) - 1)); // make it a logical shift
						r_write = rd;
						break;
					case SLT:
						reg_wdata = this->registers[rs].get_data().AsInt32() < this->registers[rt].get_data().AsInt32() ? 1 : 0;
						r_write = rd;
						break;
					case SLTU:
						reg_wdata = this->registers[rs].get_data().AsUInt32() < this->registers[rt].get_data().AsUInt32() ? 1 : 0;
						r_write = rd;
						break;
					case SUB:
						reg_wdata = (this->registers[rs].get_data().AsInt32() - this->registers[rt].get_data().AsInt32());
						r_write = rd;
						break;
					case SUBU:
						reg_wdata = (this->registers[rs].get_data().AsUInt32() - this->registers[rt].get_data().AsUInt32());
						r_write = rd;
						break;
				}

				break;
				
			case I:
				switch(op)
				{
					case ADDI:
						reg_wdata = this->registers[rs].get_data().AsInt32() + imm;
						r_write = rt;
						break;
					case ADDIU:
						reg_wdata = this->registers[rs].get_data().AsUInt32() + static_cast<unsigned int>(imm);
						r_write = rt;
						break;
					case BEQ:
						reg_we = false;
						if(this->registers[rs].get_data().AsInt32() == this->registers[rt].get_data().AsInt32())
						{
							BW_32 curr_pc = this->get_PC();
							this->pc.set_data(curr_pc.AsInt32() + branch_addr.AsInt32());
						}

						break;
					case BNE:
						reg_we = false;
						if(this->registers[rs].get_data().AsInt32() != this->registers[rt].get_data().AsInt32())
						{
							BW_32 curr_pc = this->get_PC();
							this->pc.set_data(curr_pc.AsInt32() + branch_addr.AsInt32());
						}
						break;
					case ORI:
						reg_wdata = this->registers[rs].get_data().AsInt32() | imm;
						r_write = rt;
						break;
					case ANDI:
						reg_wdata = this->registers[rs].get_data().AsInt32() & imm;
						r_write = rt;
						break;
					case XORI:
						reg_wdata = this->registers[rs].get_data().AsInt32() ^ imm;
						r_write = rt;
						break;
					case LBU:
						{
						byte_8b l_word_p_1 = this->mem_req_load(imm + this->registers[rs].get_data().AsInt32());
						BW_32 load_write = 0;
						load_write = load_write.AsUInt32() | (l_word_p_1);
						reg_wdata = load_write;
						r_write = rt;
						}
						break;
					case LHU:
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
					case LW:
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
					case SB:
						{
						char s_word_p_1 = (this->registers[rt].get_data().AsInt32() & ((1 << 8) - 1));
						this->mem_req_write(s_word_p_1, this->registers[rs].get_data().AsInt32() + imm);
						reg_we = false;
						}
						break;
					case SLTI:
						reg_wdata = this->registers[rs].get_data().AsInt32() < imm ? 1 : 0;
						r_write = rt;
						break;
					case SLTIU:
						reg_wdata = this->registers[rs].get_data().AsUInt32() < static_cast<uint32_t>(imm) ? 1 : 0;
						r_write = rt;
						break;
					case SH:
						{
						byte_8b s_word_p_1 = (this->registers[rt].get_data().AsInt32() & ((1 << 8) - 1));
						byte_8b s_word_p_2 = ((this->registers[rt].get_data().AsInt32() >> 8) & ((1 << 8) - 1));
						this->mem_req_write(s_word_p_1, this->registers[rs].get_data().AsInt32() + imm);
						this->mem_req_write(s_word_p_2, this->registers[rs].get_data().AsInt32() + 1 + imm);
						reg_we = false;
						}
						break;
					case SW:
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

				case J:
				{
					int32_t jump_mask = ~((1 << 28) - 1);

					switch(op)
					{
						case JUMP:
							pc.set_data((pc.get_data().AsInt32() & jump_mask) | (imm << 2));
							break;
						case JAL:
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
		

		return true;
	}

	void sc_cpu::rst()
	{
		for(int i = 0; i < 32; i++)
		{
			this->registers[i].set_data(0);
		}

		this->pc.set_data(0);
	}

	void sc_cpu::mem_req_write(byte_8b data, int index)
	{
		this->mm[index % this->mm.get_size()] = data;
	}

	byte_8b sc_cpu::mem_req_load(int index)
	{
		return this->mm[index % this->mm.get_size()];
	}

	void sc_cpu::ghost_cycle()
	{
		this->pc.set_data(this->pc.get_data().AsUInt32() + 4);
	}
}
