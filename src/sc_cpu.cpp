#include "sc_cpu.h"

namespace mips_tools
{

	bool sc_cpu::cycle()
	{
		format fm;

		// Fetch
		BW_32 inst_word_addr = this -> pc.get_data();
		this -> pc.set_data(inst_word_addr + 4);
		BW_32 inst_word = this -> current_inst.get_data(); // get instr word

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
		BW_32 op = (opcode_mask & inst_word) >> 26;
		BW_32 rs = (rs_mask & inst_word) >> 21;
		BW_32 rt = (rt_mask & inst_word) >> 16;
		BW_32 rd = (rd_mask & inst_word) >> 11;
		BW_32 funct = (funct_mask & inst_word);
		BW_32 imm = (imm_mask & inst_word);

		if(op == R_FORMAT) fm = R;
		else if(op >= 8 && op <= 15) fm = I;
		else fm = R;

		bool reg_we = true; // find write enable
		int r_write = 0;

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

	void sc_cpu::encode(int rs, int rt, int rd, int funct, int imm, opcode op)
	{
		BW_32 w = 0;

		if(r_inst(op))
		{
			w = (w | (funct & ((1 << 7) - 1)  ));
			w = (w | ((rd & ((1 << 6) - 1) ) << 11 ));
			w = (w | ((rt & ((1 << 6) - 1) ) << 16 ));
			w = (w | ((rs & ((1 << 6) - 1) ) << 21 ));
			w = (w | ((op & ((1 << 7) - 1) ) << 26  ));
		}

		if(i_inst(op))
		{
			w = (w | (imm & ((1 << 16) - 1)   ));
			w = (w | ((rt & ((1 << 6) - 1) ) << 16  ));
			w = (w | ((rs & ((1 << 6) - 1) ) << 21  ));
			w = (w | ((op & ((1 << 7) - 1) ) << 26  ));
		}

		this->force_fetch(w);
	}

	void sc_cpu::rst()
	{
		for(int i = 0; i < 32; i++)
		{
			this->registers[i].set_data(0);
		}

		this->pc.set_data(0);
	}

}