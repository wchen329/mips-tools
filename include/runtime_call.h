#ifndef __RUNTIME_CALL_H__
#define __RUNTIME_CALL_H__
#include "diag_cpu.h"
#include "mem.h"
#include "mb.h"

namespace mipsshell
{
	void dot_mem(mips_tools::mem * memory);
	void dot_rst(mips_tools::mb *);
	void dot_state(mips_tools::diag_cpu * dcpu);
}

#endif