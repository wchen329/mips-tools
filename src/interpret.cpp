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

namespace mips_tools
{

	// Main interpretation routine
	std::shared_ptr<BW> MIPS_32::assemble(std::vector<std::string>& args, BW& baseAddress, syms_table& jump_syms)
	{
		if(args.size() < 1)
			return std::shared_ptr<BW>(new BW_32());

		mips_tools::opcode current_op = mips_tools::SYS_RES;
		mips_tools::funct f_code = mips_tools::NONE;

		int rs = 0;
		int rt = 0;
		int rd = 0;
		int imm = 0;

		// Mnemonic resolution
		
		if("add" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::ADD; }
		else if("addiu" == args[0]) { current_op = mips_tools::ADDIU; }
		else if("addu" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::ADDU; }
		else if("addi" == args[0]) { current_op = mips_tools::ADDI; }
		else if("beq" == args[0]) { current_op = mips_tools::BEQ; }
		else if("bne" == args[0]) { current_op = mips_tools::BNE; }
		else if("sub" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SUB; }
		else if("and" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::AND; }
		else if("andi" == args[0]) { current_op = mips_tools::ANDI; }
		else if("or" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::OR; }	
		else if("ori" == args[0]) { current_op = mips_tools::ORI; }	
		else if("nor" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::NOR; }	
		else if("xori" ==  args[0]) { current_op = mips_tools::XORI; }
		else if("lbu" == args[0]) { current_op = mips_tools::LBU; }
		else if("lhu" == args[0]) { current_op = mips_tools::LHU; }
		else if("lw" == args[0]) { current_op = mips_tools::LW; }
		else if("sb" == args[0]) { current_op = mips_tools::SB; }
		else if("sh" == args[0]) { current_op = mips_tools::SH; }
		else if("sw" == args[0]) { current_op = mips_tools::SW; }
		else if("sll" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SLL; }
		else if("srl" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SRL; }
		else if("slt" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SLT; }	
		else if("slti" == args[0]) { current_op = mips_tools::SLTI;}
		else if("sltiu" == args[0]) { current_op = mips_tools::SLTIU; }	
		else if("sltu" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SLTU; }	
		else if("j" == args[0]) { current_op = mips_tools::JUMP;}
		else if("jal" == args[0]) { current_op = mips_tools::JAL;}	
		else if("jr" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::JR;}
		else
		{
			throw mt_bad_mnemonic();
		}

		// Check for insufficient arguments
		if(args.size() >= 1)
		{
			if	(
					(r_inst(current_op) && args.size() != 4 && f_code != mips_tools::JR) ||
					(r_inst(current_op) && args.size() != 2 && f_code == mips_tools::JR) ||
					(i_inst(current_op) && args.size() != 4 && !mem_inst(current_op)) ||
					(i_inst(current_op) && args.size() != 3 && mem_inst(current_op)) ||
					(j_inst(current_op) && args.size() != 2)				
				)
			{
				throw mips_tools::mt_asm_bad_arg_count();
			}

			// Now first argument parsing
			if(r_inst(current_op))
			{
					if(f_code == mips_tools::JR)
					{
						if((rs = mips_tools::friendly_to_numerical(args[1].c_str())) <= mips_tools::INVALID)
						rs = mipsshell::get_reg_num(args[1].c_str());
					}

					else
					{
						if((rd = mips_tools::friendly_to_numerical(args[1].c_str())) <= mips_tools::INVALID)
						rd = mipsshell::get_reg_num(args[1].c_str());
					}
			}

			else if(i_inst(current_op))
			{
				// later, check for branches
				if((rt = mips_tools::friendly_to_numerical(args[1].c_str())) <= mips_tools::INVALID)
				rt = mipsshell::get_reg_num(args[1].c_str());
			}

			else if(j_inst(current_op))
			{
				if(jump_syms.has(args[1]))
				{
					mips_tools::BW_32 label_PC = static_cast<int32_t>(jump_syms.lookup_from_sym(std::string(args[1].c_str())));
					imm = (label_PC.AsInt32() >> 2);
				}

				else
				{
					imm = mipsshell::get_imm(args[1].c_str());
				}
			}
	
			else
			{
				mips_tools::mt_bad_mnemonic();
			} 
		}

		// Second Argument Parsing
		
		if(args.size() > 2)
		{
			if(r_inst(current_op))
			{
				if (f_code != mips_tools::JR)
				{
					if((rs = mips_tools::friendly_to_numerical(args[2].c_str())) <= mips_tools::INVALID)
						rs = mipsshell::get_reg_num(args[2].c_str());
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
					for(int i = 0; i < wc.length(); i++)
					{
						if(wc[i] == '(') { left_parenth = true; continue; }
						if(wc[i] == ')') { right_parenth = true; continue; }

						if(left_parenth)
						{
							imm_s.push_back(wc[i]);
						}

						else
						{
							reg.push_back(wc[i]);
						}
					}

					if(!right_parenth || !left_parenth) throw mt_unmatched_parenthesis();
					if((rs = mips_tools::friendly_to_numerical(reg.c_str())) <= mips_tools::INVALID) rs = mipsshell::get_reg_num(reg.c_str());
					imm = mipsshell::get_imm(imm_s.c_str());
								
				}

				else
				{
					// later, MUST check for branches
					if((rs = mips_tools::friendly_to_numerical(args[2].c_str())) <= mips_tools::INVALID)
					rs = mipsshell::get_reg_num(args[2].c_str());
				}
			}

			else if(j_inst(current_op)){}
		}

		if(args.size() > 3)
		{
			// Third Argument Parsing
			if(r_inst(current_op))
			{
				if(f_code != mips_tools::JR)
				{
					if(shift_inst(f_code))
					{
						imm = mipsshell::get_imm(args[3].c_str());
					}

					else
					{	
						if((rt = mips_tools::friendly_to_numerical(args[3].c_str())) <= mips_tools::INVALID)
							rt = mipsshell::get_reg_num(args[3].c_str());
					}
				}
			}
						
			else if(i_inst(current_op))
			{

				if(jump_syms.has(args[3]))
				{
					mips_tools::BW_32 addr = baseAddress.AsInt32();
					mips_tools::BW_32 label_PC = static_cast<uint32_t>(jump_syms.lookup_from_sym(std::string(args[3].c_str())));
					imm = mips_tools::offset_to_address_br(addr, label_PC).AsInt32();
				}

				else
				{
					imm = mipsshell::get_imm(args[3].c_str());
				}
			}

			else if(j_inst(current_op)){}
		}

		// Pass the values of rs, rt, rd to the processor's encoding function
		BW_32 inst = generic_mips32_encode(rs, rt, rd, f_code, imm, current_op);

		return std::shared_ptr<BW>(new BW_32(inst));
	}
}
