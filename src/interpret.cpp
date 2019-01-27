/* Interpreter implementation
 *
 */
#define _CRT_SECURE_NO_WARNINGS // MSVC
#include <cstdio>
#include <cstring>
#include "mb.h"
#include "messages.h"
#include "interpret.h"

namespace mipsshell
{
	enum op
	{
		SYS_ECHO,
		SYS_HELP
	};

	bool interpret(char * line, mips_tools::mb * mb_ptr)
	{
		op current_op;

		char * working_set = strtok(line, " ");
		int round = 0;
		// Round: round corresponds to the position of the string in the instruction word
		// For example in (add $s0, $s1, $s2) add is round 0, $s0 is round 1, $s1 is round 2, $s2 is round 3)

		while(working_set != NULL)
		{
			int len = strlen(working_set);
			if(len < 1) continue;

			working_set[len - 1] = NULL; // remove newline

			// now compare and execute. If running in batch mode with the -a option, the program will act as an assembler and produce a file with -o
			switch(round){
				case 0:
					if(!strcmp(".exit", working_set)) return true;
					else if(!strcmp(".help", working_set)) { fprintf(stdout, HELP); current_op = SYS_HELP; }
					else if(!strcmp(".echo", working_set)) { for(int i = 0; i < 32; i++) fprintf(stdout, "$%d = %d\n", i, mb_ptr->get_cpu().get_reg_data(i)); current_op = SYS_ECHO; }
					else { fprintf(stdout, BAD_COMMAND); return false;}
					break;
				case 1:
					{ fprintf(stdout, BAD_COMMAND); return false;}
					break;
				case 2:
					{ fprintf(stdout, BAD_COMMAND); return false;}
					break;
				case 3:
					{ fprintf(stdout, BAD_COMMAND); return false;}
					break;
				default:
					{ fprintf(stdout, BAD_COMMAND); return false;}
					return false;
			}

			round++;
			working_set = strtok(NULL, " ");
		}

		return false;
	}
}