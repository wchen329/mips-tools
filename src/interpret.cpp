/* Interpreter implementation
 *
 */
#define _CRT_SECURE_NO_WARNINGS // MSVC
#include <cstdio>
#include <cstring>
#include <exception>
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
	bool Shell::assemble(const char * line, mips_tools::mb * mb_ptr, mips_tools::BW_32 baseAddress)
	{

		mips_tools::opcode current_op = mips_tools::SYS_RES;
		mips_tools::funct f_code = mips_tools::NONE;
		std::auto_ptr<char> ap(new char [strlen(line) + 1]);
		strcpy(ap.get(), line);

		int length = strlen(ap.get());

		bool all_whitespace_or_tab = true;

		char * start_with_no_whitespace = ap.get();

		// Look for comments, snip out everything after comment sign (#)
		for(int step = 0; step < length; step++)
		{
			if(ap.get()[step] != '\t' && ap.get()[step] != ' ' && ap.get()[step] != '\n')
			{
				all_whitespace_or_tab = false;
			}

			else
			{
				if(all_whitespace_or_tab)
				{
					start_with_no_whitespace = ap.get() + step + 1;
				}
			}

			if(ap.get()[step] == '#')
			{
				ap.get()[step] = '\0';
				break;
			}
		}

		if(all_whitespace_or_tab)
			return false;

		char * working_set = strtok(start_with_no_whitespace, " ");
		int round = 0;
		// Round: round corresponds to the position of the string in the instruction word
		// For example in (add $s0, $s1, $s2) add is round 0, $s0 is round 1, $s1 is round 2, $s2 is round 3)
		
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

		while(working_set != NULL)
		{
			interpretop op = mipsshell::PROC_INST;
			int len = strlen(working_set);
			if(len < 1) continue;

			if(working_set[len-1] == '\n' || working_set[len-1] == ',') working_set[len - 1] = NULL; // remove newline


			// now compare and execute. If running in batch mode with the -a option, the program will act as an assembler and produce a file with -o

			try
			{

				switch(round)
				{
				case 0:
						// This comparison will be optimized and placed in a separate routine, ultimately
						if(!strcmp("add", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::ADD; }
						else if(!strcmp("addi", working_set)) { current_op = mips_tools::ADDI; }
						else if(!strcmp("beq", working_set)) { current_op = mips_tools::BEQ; }
						else if(!strcmp("bne", working_set)) { current_op = mips_tools::BNE; }
						else if(!strcmp("sub", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SUB; }
						else if(!strcmp("and", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::AND; }
						else if(!strcmp("andi", working_set)) { current_op = mips_tools::ANDI; }
						else if(!strcmp("or", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::OR; }	
						else if(!strcmp("ori", working_set)) { current_op = mips_tools::ORI; }	
						else if(!strcmp("nor", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::NOR; }	
						else if(!strcmp("xori", working_set)) { current_op = mips_tools::XORI; }
						else if(!strcmp("lbu", working_set)) { current_op = mips_tools::LBU; }
						else if(!strcmp("lhu", working_set)) { current_op = mips_tools::LHU; }
						else if(!strcmp("lw", working_set)) { current_op = mips_tools::LW; }
						else if(!strcmp("sb", working_set)) { current_op = mips_tools::SB; }
						else if(!strcmp("sh", working_set)) { current_op = mips_tools::SH; }
						else if(!strcmp("sw", working_set)) { current_op = mips_tools::SW; }
						else if(!strcmp("sll", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SLL; }
						else if(!strcmp("srl", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SRL; }
						else if(!strcmp("slt", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SLT; }	
						else if(!strcmp("slti", working_set)) { current_op = mips_tools::SLTI;}
						else if(!strcmp("sltiu", working_set)) { current_op = mips_tools::SLTIU; }	
						else if(!strcmp("sltu", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SLTU; }	
						else if(!strcmp("j", working_set)) { current_op = mips_tools::JUMP;}
						else if(!strcmp("jal", working_set)) { current_op = mips_tools::JAL;}	
						else if(!strcmp("jr", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::JR;}
						else
						{
							fprintf(stdout, BAD_COMMAND);
							return false;
						}

						break;
				
					case 1:
						if(r_inst(current_op))
						{
							if(f_code == mips_tools::JR)
							{
								if((rs = mips_tools::friendly_to_numerical(working_set)) <= mips_tools::INVALID)
								rs = get_reg_num(working_set);
							}

							else
							{
								if((rd = mips_tools::friendly_to_numerical(working_set)) <= mips_tools::INVALID)
								rd = get_reg_num(working_set);
							}
						}

						else if(i_inst(current_op))
						{
							// later, check for branches
							if((rt = mips_tools::friendly_to_numerical(working_set)) <= mips_tools::INVALID)
							rt = get_reg_num(working_set);
						}

						else if(j_inst(current_op))
						{
							try
							{
								imm = get_imm(working_set);
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
									mips_tools::BW_32 label_PC = this->jump_syms.lookup_from_sym(std::string(working_set));
									imm = (label_PC >> 2);
								}

								catch(std::out_of_range&)
								{
									throw new badformat_err();
								}
							}
						}
	
						else { fprintf(stdout, "%s:", working_set); fprintf(stdout, BAD_COMMAND); return false;}
						break;
					
					case 2:
						if(r_inst(current_op))
						{
							if (f_code != mips_tools::JR)
							{
								if((rs = mips_tools::friendly_to_numerical(working_set)) <= mips_tools::INVALID)
								rs = get_reg_num(working_set);
							}
						}
						
						else if(i_inst(current_op))
						{
							if(mem_inst(current_op))
							{
								bool left_parenth = false; bool right_parenth = false;
								std::string wc = (std::string)working_set;
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
								if((rs = mips_tools::friendly_to_numerical(working_set)) <= mips_tools::INVALID)
								rs = get_reg_num(working_set);
							}
						}
						else if(j_inst(current_op)){}
						else { fprintf(stdout, BAD_COMMAND); return false;}
						break;
			
					case 3:
						if(r_inst(current_op))
						{
							if(f_code != mips_tools::JR)
							{
								if(shift_inst(f_code))
								{
									imm = get_imm(working_set);
								}

								else
								{
											
									if((rt = mips_tools::friendly_to_numerical(working_set)) <= mips_tools::INVALID)
										rt = get_reg_num(working_set);
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
								imm = get_imm(working_set);
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
									mips_tools::BW_32 label_PC = this->jump_syms.lookup_from_sym(std::string(working_set));
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
						break;
					
					default:
						throw new badformat_err();
						return false;
				}

				round++;
				working_set = strtok(NULL, " ");

			}

			catch(parser_err * e)
			{
				e->err_pr();
				delete e;
				return false;
			}

			catch(mips_tools::mt_exception * e)
			{
				fprintf(stdout, "An internal system error has occurred...\n");
				delete e;
				return false;
			}
		}

		// Check for insufficient arguments
		if( (!j_inst(current_op) && ((current_op != mips_tools::SYS_RES && round != 4 && !mem_inst(current_op) && f_code != mips_tools::JR)
									|| (mem_inst(current_op) && round != 3))) || j_inst(current_op) && round < 2 || (f_code == mips_tools::JR && round != 2))
			{ fprintf(stdout, "Expected more arguments, specification incomplete.\n"); return false; }

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

	void mipsshell::set_op_and_f(const char* mnemonic, mips_tools::opcode& op, mips_tools::funct& f)
	{

	}
}