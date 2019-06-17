/* Interpreter implementation
 *
 */
#define _CRT_SECURE_NO_WARNINGS // MSVC
#include <cstdio>
#include <cstring>
#include <exception>
#include <memory>
#include <string>
#include "states.h"
#include "diag_cpu.h"
#include "format_chk.h"
#include "mb.h"
#include "messages.h"
#include "mips.h"
#include "mtsstream.h"
#include "mt_exception.h"
#include "interpret.h"
#include "parser_err.h"
#include "primitives.h"
#include "runtime_call.h"
#include "syms_table.h"

namespace mipsshell
{

	// Main interpretation routine
	bool Shell::assemble(std::vector<std::string> args, mips_tools::mb * mb_ptr, mips_tools::BW_32 baseAddress)
	{
		if(args.size() < 1)
			return true;

		mips_tools::opcode current_op = mips_tools::SYS_RES;
		mips_tools::funct f_code = mips_tools::NONE;

		int rs = -1;
		int rt = -1;
		int rd = -1;
		int imm = -1;

		mips_tools::diag_cpu * dcpu;
		
		try
		{
			dcpu = dynamic_cast<mips_tools::diag_cpu *>(&(mb_ptr -> get_cpu())); // IMPORTANT: check for and catch bad_cast, still needed	
		}
		catch(std::exception)	// will change to bad_cast
		{
			dcpu = nullptr;
			fprintf(stderr, NON_DIAG_CPU);
		}

		// Mnemonic resolution
		
		if("add" == args[0]) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::ADD; }
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
			fprintf(stdout, BAD_COMMAND);
			throw badformat_err();
		}

		// Check for insufficient arguments
		if(args.size() > 1)
		{
			if( (!j_inst(current_op) && ((current_op != mips_tools::SYS_RES && args.size() != 4 && !mem_inst(current_op) && f_code != mips_tools::JR)
										|| (mem_inst(current_op) && args.size() != 3))) || j_inst(current_op) && args.size() < 2 || (f_code == mips_tools::JR && args.size() != 2))
				{ fprintf(stdout, "Expected more arguments, specification incomplete.\n"); return false; }

			// Now first argument parsing
			if(r_inst(current_op))
			{
					if(f_code == mips_tools::JR)
					{
						if((rs = mips_tools::friendly_to_numerical(args[1].c_str())) <= mips_tools::INVALID)
						rs = get_reg_num(args[1].c_str());
					}

					else
					{
						if((rd = mips_tools::friendly_to_numerical(args[1].c_str())) <= mips_tools::INVALID)
						rd = get_reg_num(args[1].c_str());
					}
			}

			else if(i_inst(current_op))
			{
				// later, check for branches
				if((rt = mips_tools::friendly_to_numerical(args[1].c_str())) <= mips_tools::INVALID)
				rt = get_reg_num(args[1].c_str());
			}

			else if(j_inst(current_op))
			{
				try
				{
					imm = get_imm(args[1].c_str());
				}

				catch(parser_err * e)
				{
					if(!jorb_inst(current_op))
					{
						throw;
					}

					delete e;

					// Otherwise, perceive as a label, try to convert
					try
					{
						mips_tools::BW_32 label_PC = this->jump_syms.lookup_from_sym(std::string(args[1].c_str()));
						imm = (label_PC >> 2);
					}

					catch(std::out_of_range&)
					{
						throw new badformat_err();
					}
				}
			}
	
			else { fprintf(stdout, "%s:", args[1].c_str()); fprintf(stdout, BAD_COMMAND); return false;}
		}

		// Second Argument Parsing
		
		if(args.size() > 2)
		{
			if(r_inst(current_op))
			{
				if (f_code != mips_tools::JR)
				{
					if((rs = mips_tools::friendly_to_numerical(args[2].c_str())) <= mips_tools::INVALID)
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

					if(!right_parenth || !left_parenth) throw new badformat_err();
					if((rs = mips_tools::friendly_to_numerical(reg.c_str())) <= mips_tools::INVALID) rs = get_reg_num(reg.c_str());
					imm = get_imm(imm_s.c_str());
								
				}

				else
				{
					// later, MUST check for branches
					if((rs = mips_tools::friendly_to_numerical(args[2].c_str())) <= mips_tools::INVALID)
					rs = get_reg_num(args[2].c_str());
				}
			}

			else if(j_inst(current_op)){}
			else { fprintf(stdout, BAD_COMMAND); return false;}
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
						imm = get_imm(args[3].c_str());
					}

					else
					{	
						if((rt = mips_tools::friendly_to_numerical(args[3].c_str())) <= mips_tools::INVALID)
							rt = get_reg_num(args[3].c_str());
					}
				}
			}
						
			else if(i_inst(current_op))
			{
							
				if(mem_inst(current_op))
				{
					throw new badformat_err();
				}

				try
				{
					imm = get_imm(args[3].c_str());
				}

				catch(parser_err * e)
				{
					if(!jorb_inst(current_op))
					{
						throw;
					}

					delete e;

					// Otherwise, perceive as a label, try to convert
					try
					{
						mips_tools::BW_32 label_PC = this->jump_syms.lookup_from_sym(std::string(args[3].c_str()));
						imm = mips_tools::offset_to_address_br(baseAddress, label_PC);
					}

					catch(std::out_of_range&)
					{
						throw new badformat_err();
					}
				}
			}

			else if(j_inst(current_op)){}

			else { throw new badformat_err(); }
		}

		// If system call, don't execute in CPU
		if(current_op == mips_tools::SYS_RES) return false;
		
		// Pass the values of rs, rt, rd to the processor's encoding function
		if(dcpu != nullptr)
		{
			mips_tools::BW_32 inst = dcpu -> encode(rs, rt, rd, f_code, imm, current_op);
			mips_tools::BW_32_T inst_part = mips_tools::BW_32_T(inst);

			mb_ptr->DMA_write(inst_part.b_0(), baseAddress);
			mb_ptr->DMA_write(inst_part.b_1(), baseAddress + 1);
			mb_ptr->DMA_write(inst_part.b_2(), baseAddress + 2);
			mb_ptr->DMA_write(inst_part.b_3(), baseAddress + 3);

			if(ASM_MODE && mtsstream::asmout != nullptr)
			{
				mtsstream::asmout->append(inst);
			}
		}

		// Call an execution routine explicity
		if(INTERACTIVE) mb_ptr -> step();

		return false;
	}
}