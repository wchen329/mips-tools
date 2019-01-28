/* Interpreter implementation
 *
 */
#define _CRT_SECURE_NO_WARNINGS // MSVC
#include <cstdio>
#include <cstring>
#include <vector>
#include "mb.h"
#include "messages.h"
#include "mips.h"
#include "mt_exception.h"
#include "interpret.h"

namespace mipsshell
{

	// Returns register number corresponding with argument if any
	// Returns -1 if invalid or out of range
	int get_reg_num(char * reg_str)
	{
		std::vector<char> numbers;
		int len = strlen(reg_str);
		if(len <= 1) return -1;
		for(int i = 1; i < len; i++)
		{
			if(reg_str[i] >= '0' && reg_str[i] <= '9')
			{
				numbers.push_back(reg_str[i]);
			}

			else if(reg_str[i] == ',' && i == len - 1) continue;

			else return -1;
		}

		int num = -1;

		if(numbers.empty()) return -1;
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



	// Main interpretation routine
	bool interpret(char * line, mips_tools::mb * mb_ptr)
	{
		using namespace mips_tools;  // may be changed later to allow scoping changes

		mips_tools::opcode current_op = SYS_RES;

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
			switch(round)
			{
				case 0:
					if(!strcmp(".exit", working_set)) return true;
					else if(!strcmp(".help", working_set)) { fprintf(stdout, HELP); }
					else if(!strcmp(".echo", working_set)) { for(int i = 0; i < 32; i++) fprintf(stdout, "$%d = %d\n", i, mb_ptr->get_cpu().get_reg_data(i)); }
					else if(!strcmp("add", working_set)) { current_op = R_FORMAT; }
					else if(!strcmp("addi", working_set)) { current_op = ADDI; }
					else if(!strcmp("sub", working_set)) { current_op = R_FORMAT; }
					else if(!strcmp("and", working_set)) { current_op = R_FORMAT; }
					else if(!strcmp("andi", working_set)) { current_op = ANDI; }
					else if(!strcmp("or", working_set)) { current_op = R_FORMAT; }	
					else if(!strcmp("ori", working_set)) { current_op = ORI; }	
					else { fprintf(stdout, BAD_COMMAND); return false;}
					break;
				
				case 1:
					if(r_inst(current_op))
					{
						rd = get_reg_num(working_set);
						if(rd < 0) { fprintf(stdout, BAD_FORMAT); return false; }
					}

					else if(i_inst(current_op))
					{
						// later, check for branches
						rt = get_reg_num(working_set);
						if(rt < 0) { fprintf(stdout, BAD_FORMAT); return false; }
					}

					else { fprintf(stdout, BAD_COMMAND); return false;}
					break;
				
				case 2:
					if(r_inst(current_op))
					{
						rs = get_reg_num(working_set);
						if(rs < 0) { fprintf(stdout, BAD_FORMAT); return false; }
					}
					
					else if(i_inst(current_op))
					{
						// later, MUST check for branches
						rs = get_reg_num(working_set);
						if(rs < 0) { fprintf(stdout, BAD_FORMAT); return false; }
					}

					else { fprintf(stdout, BAD_COMMAND); return false;}
					break;
			
				case 3:
					if(r_inst(current_op))
					{
						rt = get_reg_num(working_set);
						if(rt < 0) { fprintf(stdout, BAD_FORMAT); return false; }
					}
					
					else if(i_inst(current_op))
					{
						// later, check for branches
						imm = atoi(working_set);

						// to implement, check for bad immediates
					}

					else { fprintf(stdout, BAD_COMMAND); return false;}
					break;
				
				default:
					fprintf(stdout, BAD_FORMAT); return false;
					return false;
			}

			round++;
			working_set = strtok(NULL, " ");

			// Add check for empty instructions
		}

		if(current_op == SYS_RES) return false;

		// Depending on instruction type, place each corresponding field where needed
		
		// Pass the values of rs, rt, rd to the processor's encoding function
		try
		{
			mb_ptr ->get_cpu().encode(rs, rt, rd, 32, imm, current_op);
		}

		catch(mt_exception)
		{

		}

		// Call an execution routine explicity
		mb_ptr ->get_cpu().cycle();

		return false;
	}
}