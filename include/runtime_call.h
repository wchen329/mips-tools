#ifndef __RUNTIME_CALL_H__
#define __RUNTIME_CALL_H__
#include "diag_cpu.h"
#include "mem.h"

namespace mipsshell
{
	void dot_mem(mips_tools::mem * memory);

	void dot_state(mips_tools::diag_cpu * dcpu);
}

#endif