#ifndef __FORMAT_CHK_H__
#define __FORMAT_CHK_H__

#include "interpret.h"

namespace mipsshell
{
	// From a register specifier, i.e. %so get an integer representation
	int get_reg_num(char *);
	
	// From a immediate string, get an immediate value.
	int get_imm(char *);
}

#endif