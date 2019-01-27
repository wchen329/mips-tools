#include "sc_cpu.h"

namespace mips_tools
{

	bool sc_cpu::cycle()
	{
		format fm;

		// Fetch
		BW_32 inst_word_addr = this -> pc.get_data();
		this -> pc.set_data(inst_word_addr + 4);
		BW_32 inst_word = 0; // get instr word

		// Decode

		// -Masks-
		BW_32 opcode_mask = (~(1 << 26)) + 1;
		BW_32 rs_mask = ~( ((~(1 << 26)) + 1) | ((1 << 21) - 1));
		BW_32 rt_mask = ~( ((~(1 << 20)) + 1) | ((1 << 16) - 1));
		BW_32 rd_mask = ~( ((~(1 << 15)) + 1) | ((1 << 11) - 1));
		BW_32 funct_mask = 63;
		BW_32 imm_mask = (1 << 16) - 1;
		BW_32 addr_mask = (1 << 26) - 1;

		// - Actual values
		BW_32 opcode = (opcode_mask & inst_word) >> 26;
		BW_32 rs = (rs_mask & inst_word) >> 21;
		BW_32 rt = (rt_mask & inst_word) >> 16;
		BW_32 rd = (rd_mask & inst_word) >> 11;
		BW_32 funct = (funct_mask & inst_word);

		if(opcode == 0) fm = R;
		else if(opcode >= 8 && opcode <= 15) fm = I;
		else fm = R;

		bool reg_we = true; // find write enable

		// Execute
		BW_32 reg_wdata = 0;

		// find correct format based on opcode
		switch(fm)
		{
			case R:
				
				if(funct == 32)
				{
					reg_wdata = rs + rt;
				}

				break;
			case I:
				break;
		}

		// Write Back
		if(reg_we && rd != 0)
		{
			this->registers[rd].set_data(reg_wdata);
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
}