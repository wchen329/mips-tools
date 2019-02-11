#include "sc_cpu.h"
#include "mmem.h"
#include "mt_exception.h"

namespace mips_tools
{

	bool sc_cpu::cycle()
	{
		format fm;

		// Fetch
		BW_32 inst_word = 0;

		// Override in the case of force fetch
		if(forced_inst.get_data() != 0)
		{
			inst_word = this -> forced_inst.get_data(); // get instr word
			forced_inst.set_data(0);
		}

		else
		{

			BW_32 inst_word_addr = this -> pc.get_data();
			this -> pc.set_data(inst_word_addr + 4);
			BW_32_T inst_part = BW_32_T(
			this->mem_req_load(inst_word_addr),
			this->mem_req_load(inst_word_addr + 1),
			this->mem_req_load(inst_word_addr + 2),
			this->mem_req_load(inst_word_addr + 3)
			);
			inst_word = inst_part.as_BW_32(); // change
		}

		// Decode

		// -Masks-
		BW_32 opcode_mask = (~(1 << 26)) + 1;
		BW_32 rs_mask = ~( ((~(1 << 26)) + 1) | ((1 << 21) - 1));
		BW_32 rt_mask = ~( ((~(1 << 21)) + 1) | ((1 << 16) - 1));
		BW_32 rd_mask = ~( ((~(1 << 16)) + 1) | ((1 << 11) - 1));
		BW_32 funct_mask = 63;
		BW_32 imm_mask = (1 << 16) - 1;
		BW_32 addr_mask = (1 << 26) - 1;

		// - Actual values
		BW_32 op = ((opcode_mask & inst_word) >> 26) & ((1 << 6) - 1);
		BW_32 rs = (rs_mask & inst_word) >> 21;
		BW_32 rt = (rt_mask & inst_word) >> 16;
		BW_32 rd = (rd_mask & inst_word) >> 11;
		BW_32 funct = (funct_mask & inst_word);
		BW_32 imm = (imm_mask & inst_word) | ((~(inst_word & (1 << 15)) + 1) ); // make it signed

		if(op == R_FORMAT) fm = R;
		else fm = I;

		bool reg_we = true; // find write enable
		int r_write = 0;

		// Loads and Stores
		char l_word_p_1; char l_word_p_2; BW_32 load_write; char s_word_p_1; char s_word_p_2;

		// Execute
		BW_32 reg_wdata = 0;

		// find correct format based on opcode
		switch(fm)
		{
			case R:
				
				// Eventually replace these with template functions
				switch(funct)
				{
					case ADD:
						reg_wdata = (this->registers[rs] + this->registers[rt]).get_data();
						r_write = rd;
						break;
					case OR:
						reg_wdata = (this->registers[rs] | this->registers[rt]).get_data();
						r_write = rd;
						break;
					case AND: 
						reg_wdata = (this->registers[rs] & this->registers[rt]).get_data();
						r_write = rd;
						break;
					case SUB:
						reg_wdata = (this->registers[rs] - this->registers[rt]).get_data();
						r_write = rd;
						break;
				}

				break;
				
			case I:
				switch(op)
				{
					case ADDI:
						reg_wdata = this->registers[rs].get_data() + imm;
						r_write = rt;
						break;
					case ORI:
						reg_wdata = this->registers[rs].get_data() | imm;
						r_write = rt;
						break;
					case ANDI:
						reg_wdata = this->registers[rs].get_data() & imm;
						r_write = rt;
						break;
					case XORI:
						reg_wdata = this->registers[rs].get_data() ^ imm;
						r_write = rt;
						break;
					case LW:
						l_word_p_1 = this->mem_req_load(imm + this->registers[rs].get_data());
						l_word_p_2 = this->mem_req_load(imm + this->registers[rs].get_data() + 1);
						load_write = 0;
						load_write += l_word_p_1;
						load_write += (l_word_p_2 << 8);
						reg_wdata = load_write;
						r_write = rt;
						break;
					case SW:
						s_word_p_1 = (this->registers[rt].get_data() & ((1 << 8) - 1));
						s_word_p_2 = ((this->registers[rt].get_data() & ((1 << 16) - 1)) - s_word_p_1) >> 8;
						this->mem_req_write(s_word_p_1, this->registers[rs].get_data() + imm);
						this->mem_req_write(s_word_p_2, this->registers[rs].get_data() + 1 + imm);
						reg_we = false;
						break;
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

	BW_32 sc_cpu::encode(int rs, int rt, int rd, int funct, int imm_shamt_jaddr, opcode op)
	{
		BW_32 w = 0;

		if(r_inst(op))
		{
			w = (w | (funct & ((1 << 6) - 1)  ));
			w = (w | ((rd & ((1 << 5) - 1) ) << 11 ));
			w = (w | ((rt & ((1 << 5) - 1) ) << 16 ));
			w = (w | ((rs & ((1 << 5) - 1) ) << 21 ));
			w = (w | ((op & ((1 << 6) - 1) ) << 26  ));
		}

		if(i_inst(op))
		{
			w = (w | (imm_shamt_jaddr & ((1 << 16) - 1)));
			w = (w | ((rt & ((1 << 5) - 1) ) << 16  ));
			w = (w | ((rs & ((1 << 5) - 1) ) << 21  ));
			w = (w | ((op & ((1 << 6) - 1) ) << 26  ));
		}

		return w;
	}

	void sc_cpu::rst()
	{
		for(int i = 0; i < 32; i++)
		{
			this->registers[i].set_data(0);
		}

		this->pc.set_data(0);
		this->forced_inst.set_data(0);
	}

	void sc_cpu::mem_req_write(char data, int index)
	{
		if(mm.get_size() <= index) throw new mt_exception;

		this->mm[index] = data;
	}

	char sc_cpu::mem_req_load(int index)
	{
		if(mm.get_size() <= index) throw new mt_exception;
		return this->mm[index];
	}
}