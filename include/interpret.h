#ifndef __INTERPRET_H__
#define __INTERPRET_H__
#include "mb.h"

namespace mipsshell
{
	/* A simple interpreter that
	 * may continue cycling the CPU or gives access to some
	 * system calls
	 */
	bool interpret(char * line, mips_tools::mb * mb_ptr);
}

#endif