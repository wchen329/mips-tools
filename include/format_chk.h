#ifndef __FORMAT_CHK_H__
#define __FORMAT_CHK_H__

#include "interpret.h"
#include "diag_cpu.h"

namespace mipsshell
{
	// From a register specifier, i.e. %so get an integer representation
	int get_reg_num(const char *);
	
	// From a immediate string, get an immediate value.
	int get_imm(const char *);

}

#endif