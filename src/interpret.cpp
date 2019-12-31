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
#include "interpret.h"

namespace priscas
{

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
}
