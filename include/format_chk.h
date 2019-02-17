#ifndef __FORMAT_CHK_H__
#define __FORMAT_CHK_H__

#include <cstring>
#include <cstdlib>
#include "interpret.h"
#include "diag_cpu.h"

namespace mipsshell
{
	enum interpretop
	{
		PROC_INST,			// instruction for the processor
		MEM,				// .mem
		STATE,				// .state
		SYMBOL_ASSIGNMENT	// symbol assignment
	};

	// From a register specifier, i.e. %so get an integer representation
	int get_reg_num(const char *);
	
	// From a immediate string, get an immediate value.
	int get_imm(const char *);

}

#endif
