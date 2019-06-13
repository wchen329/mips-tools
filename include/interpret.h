#ifndef __INTERPRET_H__
#define __INTERPRET_H__
#include "mb.h"
#include "syms_table.h"

namespace mipsshell
{
	extern mips_tools::syms_table debug_table;

	void set_op_and_f(const char* mnemonic, mips_tools::opcode& op, mips_tools::funct& f);

}

#endif