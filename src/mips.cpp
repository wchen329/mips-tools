#include <cstring>
#include "mips.h"

namespace mips_tools
{
	int friendly_to_numerical(const char * fr_name)
	{
		int len = strlen(fr_name);
		if(len < 2) return INVALID;

		REGISTERS reg_val
			=
			// Can optimize based off of 
			fr_name[1] == 'a' ?
				!strcmp("$a0", fr_name) ? $a0 :
				!strcmp("$a1", fr_name) ? $a1 :
				!strcmp("$a2", fr_name) ? $a2 :
				!strcmp("$a3", fr_name) ? $a3 :
				!strcmp("$at", fr_name) ? $at : INVALID
			:

			fr_name[1] == 'f' ?
				!strcmp("$fp", fr_name) ? $fp : INVALID
			:

			fr_name[1] == 'g' ?
				!strcmp("$gp", fr_name) ? $gp : INVALID
			:

			fr_name[1] == 'k' ?
				!strcmp("$k0", fr_name) ? $k0 :
				!strcmp("$k1", fr_name) ? $k1 : INVALID
			:

			fr_name[1] == 'r' ?
				!strcmp("$ra", fr_name) ? $ra : INVALID
			:

			fr_name[1] == 's' ?
				!strcmp("$s0", fr_name) ? $s0 :
				!strcmp("$s1", fr_name) ? $s1 :
				!strcmp("$s2", fr_name) ? $s2 :
				!strcmp("$s3", fr_name) ? $s3 :
				!strcmp("$s4", fr_name) ? $s4 :
				!strcmp("$s5", fr_name) ? $s5 :
				!strcmp("$s6", fr_name) ? $s6 :
				!strcmp("$s7", fr_name) ? $s7 : INVALID
			:

			fr_name[1] == 't' ?
				!strcmp("$t0", fr_name) ? $t0 :
				!strcmp("$t1", fr_name) ? $t1 :
				!strcmp("$t2", fr_name) ? $t2 :
				!strcmp("$t3", fr_name) ? $t3 :
				!strcmp("$t4", fr_name) ? $t4 :
				!strcmp("$t5", fr_name) ? $t5 :
				!strcmp("$t6", fr_name) ? $t6 :
				!strcmp("$t7", fr_name) ? $t7 : INVALID
			:

			fr_name[1] == 'v' ?
				!strcmp("$v0", fr_name) ? $v0 :
				!strcmp("$v1", fr_name) ? $v1 : INVALID
			:
			fr_name[1] == 'z' ?
				!strcmp("$zero", fr_name) ? $zero : INVALID
			: INVALID;

		return reg_val;
	}

	bool r_inst(opcode operation)
	{
		return
		
			operation == R_FORMAT ? true :
			false ;
	}

	bool i_inst(opcode operation)
	{
		return
			operation == ADDI ? true :
			operation == ADDIU ? true:
			operation == ANDI ? true :
			operation == ORI ? true :
			operation == XORI ? true :
			operation == LB ? true :
			operation == LBU ? true :
			operation == LH ? true :
			operation == LHU ? true :
			operation == LUI ? true :
			operation == LW ? true :
			operation == LWL ? true :
			operation == SB ? true :
			operation == SH ? true :
			operation == SW ? true :
			operation == BEQ ? true :
			operation == BNE ? true :
			operation == BLEZ ? true :
			operation == BGTZ ? true :
			operation == SLTI ? true :
			operation == SLTIU ? true :
			operation == SWL ? true : false ;
	}

	bool j_inst(opcode operation)
	{
		return
			operation == JUMP ? true :
			operation == JAL ? true: false;
	}

	bool mem_inst(opcode operation)
	{
		return
			(operation == LW || operation == SW || operation == SB || operation == LB || operation == LH || operation == SH )?
			true : false;
	}

	bool shift_inst(funct f)
	{
		return
			f == SLL ? true :
			f == SRL ? true :
			false;
	}

	bool jorb_inst(opcode operation)
	{
		// First check jumps
		bool is_jump = j_inst(operation);

		bool is_branch =
			operation == BEQ ? true :
			operation == BNE ? true :
			operation == BLEZ ? true :
			operation == BGTZ ? true : false;

		return is_jump || is_branch;
	}

	BW_32 generic_mips32_encode(int rs, int rt, int rd, int funct, int imm_shamt_jaddr, opcode op)
	{
		BW_32 w = 0;

		if(r_inst(op))
		{
			w = (w | (funct & ((1 << 6) - 1)  ));
			w = (w | ((imm_shamt_jaddr & ((1 << 5) - 1) ) << 6 ));
			w = (w | ((rd & ((1 << 5) - 1) ) << 11 ));
			w = (w | ((rt & ((1 << 5) - 1) ) << 16 ));
			w = (w | ((rs & ((1 << 5) - 1) ) << 21 ));
			w = (w | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		if(i_inst(op))
		{
			w = (w | (imm_shamt_jaddr & ((1 << 16) - 1)));
			w = (w | ((rt & ((1 << 5) - 1) ) << 16 ));
			w = (w | ((rs & ((1 << 5) - 1) ) << 21 ));
			w = (w | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		if(j_inst(op))
		{
			w = (w | (imm_shamt_jaddr & ((1 << 26) - 1)));
			w = (w | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		return w;
	}

	BW_32 offset_to_address_br(BW_32 current, BW_32 target)
	{
		BW_32 ret = target - current;
		ret = ret - 4;
		ret = (ret >> 2);
		return ret;
	}

	template<class in_t>
	in_t mips_alu<in_t>::execute(ALU::ALUOp, in_t arg1, in_t arg2, bool unsigned_op)
	{
		in_t ret;

		switch(ALUOp)
		{
			case ALU::ADD:
				ret = arg1 + arg2;
				break;

			case ALU::SUB:
				ret = arg1 - arg2;
				break;

			case ALU::SLL:
				ret = arg1 << arg2;
				break;
			case ALU::SRL:
				ret = arg1 >> arg2;
				break;
			default:
				throw new mt_exception();
		}

		return ret;
	}

	void mips_decoding_unit_32::decode(	const BW_32 inst_word,
										format& fm,
										BW_32& op,
										BW_32& rs,
										BW_32& rt,
										BW_32& rd,
										BW_32& funct,
										BW_32& shamt,
										BW_32& imm )
	{
		// -Masks-
		BW_32 opcode_mask = (~(1 << 26)) + 1;
		BW_32 rs_mask = ~( ((~(1 << 26)) + 1) | ((1 << 21) - 1));
		BW_32 rt_mask = ~( ((~(1 << 21)) + 1) | ((1 << 16) - 1));
		BW_32 rd_mask = ~( ((~(1 << 16)) + 1) | ((1 << 11) - 1));
		BW_32 funct_mask = 63;
		BW_32 shamt_mask = (1 << 11) - 1 - funct_mask;
		BW_32 imm_mask = (1 << 16) - 1;
		BW_32 addr_mask = (1 << 26) - 1;

		// - Actual values
		op = ((opcode_mask & inst_word) >> 26) & ((1 << 6) - 1);
		rs = (rs_mask & inst_word) >> 21;
		rt = (rt_mask & inst_word) >> 16;
		rd = (rd_mask & inst_word) >> 11;
		funct = (funct_mask & inst_word);
		shamt = (shamt_mask & inst_word) >> 6;
		imm = (imm_mask & inst_word) | ((~(inst_word & (1 << 15)) + 1) ); // make it signed

		if(op == R_FORMAT) fm = R;
		else if(j_inst(static_cast<opcode>(op))) fm = J;
		else fm = I;
	}
}
