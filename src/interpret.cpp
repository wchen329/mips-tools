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
	mips_tools::syms_table s_table;
	mips_tools::syms_table debug_table;

	// Main interpretation routine
	bool interpret(const char * line, mips_tools::mb * mb_ptr)
	{

		mips_tools::opcode current_op = mips_tools::SYS_RES;
		mips_tools::funct f_code = mips_tools::NONE;
		std::auto_ptr<char> ap(new char [strlen(line) + 1]);
		strcpy(ap.get(), line);

		char * working_set = strtok(ap.get(), " ");
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

						if(!strcmp(".exit", working_set))
						{
							if(!INTERACTIVE && HAS_INPUT && !ASM_MODE && PRE_ASM)
								debug_table.insert(".exit", dcpu->get_PC());
							else
							{		
								exit(0);
							}
						}
						else if(!strcmp(".cycle", working_set)){ mb_ptr -> step(); return false; }	// step the processor on the current PC an instruction
						else if(!strcmp(".help", working_set)) { fprintf(stdout, HELP); }
						else if(!strcmp(".mem", working_set)) { fprintf(stdout, "Main Memory Size: %d bytes\n", mb_ptr->get_mmem_size()); }
						else if(!strcmp(".rst", working_set)) dot_rst(mb_ptr);
						else if(!strcmp(".state", working_set))
						{
							if(!INTERACTIVE && HAS_INPUT && !ASM_MODE && PRE_ASM)
								debug_table.insert(".state", dcpu->get_PC());
							else
							{
								dot_state(dcpu);
							}
						}
						else if(!strcmp(".time", working_set)) dot_time(mb_ptr);
						else if(!strcmp(".run", working_set)) { fprintf(stdout, "Continuing...\n"); mipsshell::INTERACTIVE = false; mipsshell::SUSPEND = false;}
						else if(!strcmp("add", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::ADD; }
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
						else
						{
							bool syms = false;
							int ws_len = strlen(working_set);

							if(ws_len > 0)
							{
								if(working_set[ws_len-1] == ':')
								{
									//Interactive Address Assignment
									if(working_set[0] == '.')
									{
										std::string w(working_set + 1);
										w.pop_back();
										fprintf(stdout, "Assigning symbol... %s <-> PC = %d\n", w.c_str(), dcpu->get_PC());
										s_table.insert(w, dcpu->get_PC());

									}

									// Else non interactive

									else
									{
										op = mipsshell::SYMBOL_ASSIGNMENT;
										round = round - 1;
										
									}

									syms = true;
								}
							}

							if(!syms) fprintf(stdout, BAD_COMMAND); return false;
						}

						break;
				
					case 1:
						if(r_inst(current_op))
						{
							if((rd = mips_tools::friendly_to_numerical(working_set)) <= mips_tools::INVALID)
							rd = get_reg_num(working_set);
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
									mips_tools::BW_32 label_PC = s_table.lookup_from_sym(std::string(working_set));
									imm = mips_tools::offset_to_address(dcpu->get_PC(), label_PC);
								}

								catch(std::out_of_range&)
								{
									throw new badformat_err();
								}
							}
						}
	
						else { fprintf(stdout, BAD_COMMAND); return false;}
						break;
					
					case 2:
						if(r_inst(current_op))
						{
							if((rs = mips_tools::friendly_to_numerical(working_set)) <= mips_tools::INVALID)
							rs = get_reg_num(working_set);
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

						else { fprintf(stdout, BAD_COMMAND); return false;}
						break;
			
					case 3:
						if(r_inst(current_op))
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
									mips_tools::BW_32 label_PC = s_table.lookup_from_sym(std::string(working_set));
									imm = mips_tools::offset_to_address(dcpu->get_PC(), label_PC);
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

			catch(mt_exception * e)
			{
				fprintf(stdout, "An internal system error has occurred...\n");
				delete e;
				return false;
			}
		}

		// Check for insufficient arguments
		if( (!j_inst(current_op) && (current_op && (current_op != mips_tools::SYS_RES && round != 4 && !mem_inst(current_op)) || (mem_inst(current_op) && round != 3))) || j_inst(current_op) && round != 2)
			{ fprintf(stdout, "Expected more arguments, specification incomplete.\n"); return false; }

		// If system call, don't execute in CPU
		if(current_op == mips_tools::SYS_RES) return false;
		
		// Pass the values of rs, rt, rd to the processor's encoding function
		if(dcpu != nullptr)
		{
			mips_tools::BW_32 inst = dcpu -> encode(rs, rt, rd, f_code, imm, current_op);
			mips_tools::BW_32_T inst_part = mips_tools::BW_32_T(inst);

			mb_ptr->DMA_write(inst_part.b_0(), dcpu->get_PC());
			mb_ptr->DMA_write(inst_part.b_1(), dcpu->get_PC() + 1);
			mb_ptr->DMA_write(inst_part.b_2(), dcpu->get_PC() + 2);
			mb_ptr->DMA_write(inst_part.b_3(), dcpu->get_PC() + 3);

			if(ASM_MODE && mtsstream::asmout != nullptr)
			{
				mtsstream::asmout->append(inst);
			}
		}

		// Call an execution routine explicity
		if(INTERACTIVE) mb_ptr -> step();
		else
		{
			dcpu->ghost_cycle();
		}

		return false;
	}
}