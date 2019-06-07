#ifndef __INTERPRET_H__
#define __INTERPRET_H__
#include "mb.h"
#include "syms_table.h"

namespace mipsshell
{
	extern mips_tools::syms_table debug_table;

	/*enum op
	{
		ADD,
		SUB,
		SUBU,
		ADDI,
		ADDIU,
		AND,
		ANDI,
		OR,
		ORI,
		DUMMY, // separator can be used for more flexible comparisons
		SYS_ECHO,
		SYS_HELP
	};*/

	/* A simple interpreter that
	 * may continue cycling the CPU or gives access to some
	 * system calls
	 */
	bool interpret(const char * line, mips_tools::mb * mb_ptr);
}

#endif