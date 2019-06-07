#include <cstdio>
#include "runtime_call.h"
#include "states.h"

namespace mipsshell
{

	void Enter_Interactive(int a)
	{
		INTERACTIVE = true;
		SUSPEND = true;
	}

	void dot_mem(mips_tools::mem * memory)
	{
		if(memory != nullptr)
		{

		}
	}

	void dot_rst(mips_tools::mb * mb_ptr)
	{
		mb_ptr -> reset();
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

	void dot_time(mips_tools::mb * mb_ptr)
	{
		if(mb_ptr != nullptr)
		{
				fprintf(stdout, "TIMING INFORMATION::\n");
				fprintf(stdout, "Clock Period = %f s\n", mb_ptr->get_cpu().get_clk_T() );
				fprintf(stdout, "Current Cycle Count = %d cycles\n", mb_ptr->get_cycles());
				fprintf(stdout, "Current Time Elapsed = %f s\n", mb_ptr->get_time());
		}
	}
}