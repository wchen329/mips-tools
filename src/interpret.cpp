/* Interpreter implementation
 *
 */
#define _CRT_SECURE_NO_WARNINGS // MSVC
#include <cstdio>
#include <cstring>
#include "format_chk.h"
#include "mb.h"
#include "messages.h"
#include "mips.h"
#include "mt_exception.h"
#include "interpret.h"
#include "parser_err.h"
#include "runtime_call.h"

namespace mipsshell
{
	// Main interpretation routine
	bool interpret(char * line, mips_tools::mb * mb_ptr)
	{

		mips_tools::opcode current_op = mips_tools::SYS_RES;
		mips_tools::funct f_code = mips_tools::NONE;
		

		char * working_set = strtok(line, " ");
		int round = 0;
		// Round: round corresponds to the position of the string in the instruction word
		// For example in (add $s0, $s1, $s2) add is round 0, $s0 is round 1, $s1 is round 2, $s2 is round 3)
		
		int rs = -1;
		int rt = -1;
		int rd = -1;
		int imm = -1;

		while(working_set != NULL)
		{

			int len = strlen(working_set);
			if(len < 1) continue;

			if(working_set[len-1] == '\n') working_set[len - 1] = NULL; // remove newline


			// now compare and execute. If running in batch mode with the -a option, the program will act as an assembler and produce a file with -o

			try
			{

				switch(round)
				{
				case 0:
						if(!strcmp(".exit", working_set)) return true;
						else if(!strcmp(".help", working_set)) { fprintf(stdout, HELP); }
						else if(!strcmp(".state", working_set)) { for(int i = 0; i < 32; i++) fprintf(stdout, "$%d = %d\n", i, mb_ptr->get_cpu().get_reg_data(i)); }
						else if(!strcmp("add", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::ADD; }
						else if(!strcmp("addi", working_set)) { current_op = mips_tools::ADDI; }
						else if(!strcmp("sub", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::SUB; }
						else if(!strcmp("and", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::AND; }
						else if(!strcmp("andi", working_set)) { current_op = mips_tools::ANDI; }
						else if(!strcmp("or", working_set)) { current_op = mips_tools::R_FORMAT; f_code = mips_tools::OR; }	
						else if(!strcmp("ori", working_set)) { current_op = mips_tools::ORI; }	
						else { fprintf(stdout, BAD_COMMAND); return false;}
						break;
				
					case 1:
						if(r_inst(current_op))
						{
							rd = get_reg_num(working_set);
						}

						else if(i_inst(current_op))
						{
							// later, check for branches
							rt = get_reg_num(working_set);
						}
	
						else { fprintf(stdout, BAD_COMMAND); return false;}
						break;
					
					case 2:
						if(r_inst(current_op))
						{
							rs = get_reg_num(working_set);
						}
						
						else if(i_inst(current_op))
						{
							// later, MUST check for branches
							rs = get_reg_num(working_set);
						}

						else { fprintf(stdout, BAD_COMMAND); return false;}
						break;
			
					case 3:
						if(r_inst(current_op))
						{
							rt = get_reg_num(working_set);
						}
						
						else if(i_inst(current_op))
						{
							// later, check for branches stores and etc.
							imm = get_imm(working_set);

							// to implement, check for bad immediates
						}

						else { fprintf(stdout, BAD_COMMAND); return false;}
						break;
					
					default:
						fprintf(stdout, BAD_FORMAT);
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

		}

		// Check for insufficient arguments
		if(current_op != mips_tools::SYS_RES) if(round != 4) { fprintf(stdout, "Expected more arguments, specification incomplete.\n"); return false; }

		// If system call, don't execute in CPU
		if(current_op == mips_tools::SYS_RES) return false;
		
		// Pass the values of rs, rt, rd to the processor's encoding function
		mb_ptr ->get_cpu().encode(rs, rt, rd, f_code, imm, current_op);

		// Call an execution routine explicity
		mb_ptr ->get_cpu().cycle();

		return false;
	}
}