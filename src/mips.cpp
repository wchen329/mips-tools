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
#include "mips.h"

namespace priscas
{
	int MIPS_32::friendly_to_numerical(const char * fr_name)
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
				!strcmp("$s7", fr_name) ? $s7 :
				!strcmp("$sp", fr_name) ? $sp : INVALID
			:

			fr_name[1] == 't' ?
				!strcmp("$t0", fr_name) ? $t0 :
				!strcmp("$t1", fr_name) ? $t1 :
				!strcmp("$t2", fr_name) ? $t2 :
				!strcmp("$t3", fr_name) ? $t3 :
				!strcmp("$t4", fr_name) ? $t4 :
				!strcmp("$t5", fr_name) ? $t5 :
				!strcmp("$t6", fr_name) ? $t6 :
				!strcmp("$t7", fr_name) ? $t7 :
				!strcmp("$t8", fr_name) ? $t8 :
				!strcmp("$t9", fr_name) ? $t9 : INVALID
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

	UPString MIPS_32::get_reg_name(int id) const
	{
		UPString name =
			id == 0 ? "$zero" :
			id == 1 ? "$at" :
			id == 2 ? "$v0" :
			id == 3 ? "$v1" :
			id == 4 ? "$a0" :
			id == 5 ? "$a1" :
			id == 6 ? "$a2" :
			id == 7 ? "$a3" :
			id == 8 ? "$t0" :
			id == 9 ? "$t1" :
			id == 10 ? "$t2" :
			id == 11 ? "$t3" :
			id == 12 ? "$t4" :
			id == 13 ? "$t5" :
			id == 14 ? "$t6" :
			id == 15 ? "$t7" :
			id == 16 ? "$s0" :
			id == 17 ? "$s1" :
			id == 18 ? "$s2" :
			id == 19 ? "$s3" :
			id == 20 ? "$s4" :
			id == 21 ? "$s5" :
			id == 22 ? "$s6" :
			id == 23 ? "$s7" :
			id == 24 ? "$t8" :
			id == 25 ? "$t9" :
			id == 26 ? "$k0" :
			id == 27 ? "$k1" :
			id == 28 ? "$gp" :
			id == 29 ? "$sp" :
			id == 30 ? "$fp" :
			id == 31 ? "$ra" : "";
		
		if(name == "")
		{
			throw reg_oob_exception();
		}
		
		return name;
	}

	bool MIPS_32::r_inst(opcode operation)
	{
		return
		
			operation == R_FORMAT ? true :
			false ;
	}

	bool MIPS_32::i_inst(opcode operation)
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

	bool MIPS_32::j_inst(opcode operation)
	{
		return
			operation == JUMP ? true :
			operation == JAL ? true: false;
	}

	bool MIPS_32::mem_inst(opcode operation)
	{
		return
			(mem_write_inst(operation) || mem_read_inst(operation))?
			true : false;
	}

	bool MIPS_32::mem_write_inst(opcode operation)
	{
		return
			(operation == SW || operation == SB || operation == SH )?
			true : false;
	}

	bool MIPS_32::mem_read_inst(opcode operation)
	{
		return
			(operation == LW || operation == LBU || operation == LHU )?
			true : false;
	}

	bool MIPS_32::reg_write_inst(opcode operation, funct func)
	{
		return
			(mem_read_inst(operation)) || (operation == R_FORMAT && func != JR) || (operation == ADDI) || (operation == ORI)
			|| (operation == ANDI) || (operation == XORI) || (operation == SLTI) || (operation == SLTIU) || (operation == ADDIU) || (operation == JAL);
	}

	bool MIPS_32::shift_inst(funct f)
	{
		return
			f == SLL ? true :
			f == SRL ? true :
			false;
	}

	bool MIPS_32::jorb_inst(opcode operation, funct fcode)
	{
		// First check jumps
		bool is_jump = j_inst(operation);

		bool is_jr = operation == R_FORMAT && fcode == JR;

		bool is_branch =
			operation == BEQ ? true :
			operation == BNE ? true :
			operation == BLEZ ? true :
			operation == BGTZ ? true : false;

		return is_jump || is_branch || is_jr;
	}

	BW_32 MIPS_32::generic_mips32_encode(int rs, int rt, int rd, int funct, int imm_shamt_jaddr, opcode op)
	{
		BW_32 w = 0;

		if(r_inst(op))
		{
			w = (w.AsInt32() | (funct & ((1 << 6) - 1)  ));
			w = (w.AsInt32() | ((imm_shamt_jaddr & ((1 << 5) - 1) ) << 6 ));
			w = (w.AsInt32() | ((rd & ((1 << 5) - 1) ) << 11 ));
			w = (w.AsInt32() | ((rt & ((1 << 5) - 1) ) << 16 ));
			w = (w.AsInt32() | ((rs & ((1 << 5) - 1) ) << 21 ));
			w = (w.AsInt32() | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		if(i_inst(op))
		{
			w = (w.AsInt32() | (imm_shamt_jaddr & ((1 << 16) - 1)));
			w = (w.AsInt32() | ((rt & ((1 << 5) - 1) ) << 16 ));
			w = (w.AsInt32() | ((rs & ((1 << 5) - 1) ) << 21 ));
			w = (w.AsInt32() | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		if(j_inst(op))
		{
			w = (w.AsInt32() | (imm_shamt_jaddr & ((1 << 26) - 1)));
			w = (w.AsInt32() | ((op & ((1 << 6) - 1) ) << 26 ));
		}

		return w;
	}

	BW_32 MIPS_32::offset_to_address_br(BW_32 current, BW_32 target)
	{
		BW_32 ret = target.AsInt32() - current.AsInt32();
		ret = ret.AsInt32() - 4;
		ret = (ret.AsInt32() >> 2);
		return ret;
	}

	void MIPS_32::mips_decoding_unit_32::decode(	BW_32 inst,
										format& fm,
										opcode& op,
										int& rs,
										int& rt,
										int& rd,
										funct& func,
										int32_t& shamt,
										int32_t& imm )
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
		op = static_cast<opcode>(((opcode_mask & inst_word) >> 26) & ((1 << 6) - 1));

		// Set a mode based on OP
		if(op == R_FORMAT) fm = R;
		else if(j_inst(static_cast<opcode>(op))) fm = J;
		else fm = I;

		// Then decode!
		rs = (rs_mask & inst_word) >> 21;
		rt = (rt_mask & inst_word) >> 16;
		rd = (rd_mask & inst_word) >> 11;
		func = static_cast<funct>((funct_mask & inst_word));
		shamt = (shamt_mask & inst_word) >> 6;
		imm = fm == I	? (imm_mask_i & inst_word) | (~(inst_word & (1 << 15)) + 1)
						: (addr_mask & inst_word) | (~(inst_word & (1 << 25)) + 1); // make it signed
	}

		// Main interpretation routine
	mBW MIPS_32::assemble(const Arg_Vec& args, const BW& baseAddress, syms_table& jump_syms) const
	{
		if(args.size() < 1)
			return std::shared_ptr<BW>(new BW_32());

		opcode current_op = SYS_RES;
		funct f_code = NONE;

		int rs = 0;
		int rt = 0;
		int rd = 0;
		int imm = 0;

		// Mnemonic resolution
		
		if("add" == args[0]) { current_op = R_FORMAT; f_code = ADD; }
		else if("addiu" == args[0]) { current_op = ADDIU; }
		else if("addu" == args[0]) { current_op = R_FORMAT; f_code = ADDU; }
		else if("addi" == args[0]) { current_op = ADDI; }
		else if("beq" == args[0]) { current_op = BEQ; }
		else if("bne" == args[0]) { current_op = BNE; }
		else if("sub" == args[0]) { current_op = R_FORMAT; f_code = SUB; }
		else if("and" == args[0]) { current_op = R_FORMAT; f_code = AND; }
		else if("andi" == args[0]) { current_op = ANDI; }
		else if("or" == args[0]) { current_op = R_FORMAT; f_code = OR; }	
		else if("ori" == args[0]) { current_op = ORI; }	
		else if("nor" == args[0]) { current_op = R_FORMAT; f_code = NOR; }	
		else if("xori" ==  args[0]) { current_op = XORI; }
		else if("lbu" == args[0]) { current_op = LBU; }
		else if("lhu" == args[0]) { current_op = LHU; }
		else if("lw" == args[0]) { current_op = LW; }
		else if("sb" == args[0]) { current_op = SB; }
		else if("sh" == args[0]) { current_op = SH; }
		else if("sw" == args[0]) { current_op = SW; }
		else if("sll" == args[0]) { current_op = R_FORMAT; f_code = SLL; }
		else if("srl" == args[0]) { current_op = R_FORMAT; f_code = SRL; }
		else if("slt" == args[0]) { current_op = R_FORMAT; f_code = SLT; }	
		else if("slti" == args[0]) { current_op = SLTI;}
		else if("sltiu" == args[0]) { current_op = SLTIU; }	
		else if("sltu" == args[0]) { current_op = R_FORMAT; f_code = SLTU; }
		else if("subu" == args[0]) { current_op = R_FORMAT; f_code = SUBU; }
		else if("j" == args[0]) { current_op = JUMP;}
		else if("jal" == args[0]) { current_op = JAL;}	
		else if("jr" == args[0]) { current_op = R_FORMAT; f_code = JR;}
		else
		{
			throw mt_bad_mnemonic();
		}

		// Check for insufficient arguments
		if(args.size() >= 1)
		{
			if	(
					(r_inst(current_op) && args.size() != 4 && f_code != JR) ||
					(r_inst(current_op) && args.size() != 2 && f_code == JR) ||
					(i_inst(current_op) && args.size() != 4 && !mem_inst(current_op)) ||
					(i_inst(current_op) && args.size() != 3 && mem_inst(current_op)) ||
					(j_inst(current_op) && args.size() != 2)				
				)
			{
				throw mt_asm_bad_arg_count();
			}

			// Now first argument parsing
			if(r_inst(current_op))
			{
					if(f_code == JR)
					{
						if((rs = friendly_to_numerical(args[1].c_str())) <= INVALID)
						rs = get_reg_num(args[1].c_str());
					}

					else
					{
						if((rd = friendly_to_numerical(args[1].c_str())) <= INVALID)
						rd = get_reg_num(args[1].c_str());
					}
			}

			else if(i_inst(current_op))
			{
				// later, check for branches
				if((rt = friendly_to_numerical(args[1].c_str())) <= INVALID)
				rt = get_reg_num(args[1].c_str());
			}

			else if(j_inst(current_op))
			{
				if(jump_syms.has(args[1]))
				{
					BW_32 label_PC = static_cast<int32_t>(jump_syms.lookup_from_sym(std::string(args[1].c_str())));
					imm = (label_PC.AsInt32() >> 2);
				}

				else
				{
					imm = get_imm(args[1].c_str());
				}
			}
	
			else
			{
				mt_bad_mnemonic();
			} 
		}

		// Second Argument Parsing
		
		if(args.size() > 2)
		{
			if(r_inst(current_op))
			{
				if (f_code != JR)
				{
					if((rs = friendly_to_numerical(args[2].c_str())) <= INVALID)
						rs = get_reg_num(args[2].c_str());
				}
			}
						
			else if(i_inst(current_op))
			{
				if(mem_inst(current_op))
				{
					bool left_parenth = false; bool right_parenth = false;
					std::string wc = args[2];
					std::string imm_s = std::string();
					std::string reg = std::string();

					for(size_t i = 0; i < wc.length(); i++)
					{
						if(wc[i] == '(') { left_parenth = true; continue; }
						if(wc[i] == ')') { right_parenth = true; continue; }

						if(left_parenth)
						{
							reg.push_back(wc[i]);
						}

						else
						{
							imm_s.push_back(wc[i]);
						}
					}

					if(!right_parenth || !left_parenth) throw mt_unmatched_parenthesis();
					if((rs = friendly_to_numerical(reg.c_str())) <= INVALID) rs = get_reg_num(reg.c_str());
					imm = get_imm(imm_s.c_str());
								
				}

				else
				{
					// later, MUST check for branches
					if((rs = friendly_to_numerical(args[2].c_str())) <= INVALID)
					rs = get_reg_num(args[2].c_str());
				}
			}

			else if(j_inst(current_op)){}
		}

		if(args.size() > 3)
		{
			// Third Argument Parsing
			if(r_inst(current_op))
			{
				if(f_code != JR)
				{
					if(shift_inst(f_code))
					{
						imm = get_imm(args[3].c_str());
					}

					else
					{	
						if((rt = friendly_to_numerical(args[3].c_str())) <= INVALID)
							rt = get_reg_num(args[3].c_str());
					}
				}
			}
						
			else if(i_inst(current_op))
			{

				if(jump_syms.has(args[3]))
				{
					BW_32 addr = baseAddress.AsInt32();
					BW_32 label_PC = static_cast<uint32_t>(jump_syms.lookup_from_sym(UPString(args[3].c_str())));
					imm = offset_to_address_br(addr, label_PC).AsInt32();
				}

				else
				{
					imm = get_imm(args[3].c_str());
				}
			}

			else if(j_inst(current_op)){}
		}

		// Pass the values of rs, rt, rd to the processor's encoding function
		BW_32 inst = generic_mips32_encode(rs, rt, rd, f_code, imm, current_op);

		return mBW(new BW_32(inst));
	}

		// Returns register number corresponding with argument if any
	// Returns -1 if invalid or out of range
	int MIPS_32::get_reg_num(const char * reg_str)
	{
		std::vector<char> numbers;
		int len = strlen(reg_str);
		if(len <= 1) throw priscas::mt_bad_imm();
		if(reg_str[0] != '$') throw priscas::mt_parse_unexpected("$", reg_str);
		for(int i = 1; i < len; i++)
		{
			if(reg_str[i] >= '0' && reg_str[i] <= '9')
			{
				numbers.push_back(reg_str[i]);
			}

			else throw priscas::mt_bad_reg_format();
		}

		int num = -1;

		if(numbers.empty()) throw priscas::mt_bad_reg_format();
		else
		{
			char * num_str = new char[numbers.size()];

			int k = 0;
			for(std::vector<char>::iterator itr = numbers.begin(); itr < numbers.end(); itr++)
			{
				num_str[k] = *itr;
				k++;
			}
			num = atoi(num_str);
			delete[] num_str;
		}

		return num;
	}

	// Returns immediate value if valid
	int MIPS_32::get_imm(const char * str)
	{
		int len = strlen(str);
		
		for(int i = 0; i < len; i++)
		{
			if(str[i] < '0' || str[i] > '9')
			{
				if(i == 0 && str[i] != '-')
					throw priscas::mt_bad_imm();
			}
		}

		return atoi(str);
	}
}
