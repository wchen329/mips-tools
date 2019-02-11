#include <cstdio>
#include "runtime_call.h"

namespace mipsshell
{
	void dot_mem(mips_tools::mem * memory)
	{
		if(memory != nullptr)
		{

		}
	}

	void dot_state(mips_tools::diag_cpu * dcpu)
	{
		if(dcpu != nullptr)
		{
				fprintf(stdout, "REGISTER STATE:\n");
				fprintf(stdout, "$PC = %d\n", dcpu->get_PC() );
				for(int i = 0; i < 32; i++) fprintf(stdout, "$%d = %d\n", i, dcpu->get_reg_data(i));
		}
	}

}