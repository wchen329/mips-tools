#include <cstdio>
#include <map>
#include "messages.h"
#include "range.h"
#include "runtime_call.h"
#include "states.h"

namespace mipsshell
{

	void Enter_Interactive(int a)
	{
		INTERACTIVE = true;
		SUSPEND = true;
	}

	void Shell::execute_runtime_directive(std::vector<std::string>& args_list)
	{
		if(args_list.size() < 1)
		{
			throw parser_err();
		}
		// Check first argument, and pass the rest as args for that call
		
		if(this->directives.count(args_list[0]) > 0)
		{
			directives.at(args_list[0])(args_list, *this);
		}
		
		else
		{
			return;
		}
	}

	void breakpoint(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[Breakpoint]\n");
		fprintf(stdout, "Not yet implemented\n");
	}

	void cpuopts(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[CPU Specific Options]\n");
		fprintf(stdout, "Not yet implemented\n");
	}

	void exit(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "Simulation terminating...\n");
		inst.SetState(Shell::KILLED);
	}

	void help(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[Help]\n");
		fprintf(stdout, HELP);
	}

	void pci(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[PCI Bus Emulation]\n");
		fprintf(stdout, "Not yet implemented\n");
	}

	void rst(std::vector<std::string> & args, Shell& inst)
	{
		if(args.size() <= 1)
		{
			inst.GetMotherboard().reset();
		}

		else
		{
			bool RESET_STATE = false;
			bool RESET_MEM = false;
			bool RESET_MACHINE = false;

			for(size_t itr = 1; itr < args.size(); itr++)
			{
				if (args[itr] == "-s") RESET_STATE = true;
				else if(args[itr] == "-m") RESET_MEM = true;
				else if(args[itr] == "-M") RESET_MACHINE = true;
				else continue;
			}

			if(RESET_STATE)
			{
				inst.GetMotherboard().get_cpu().rst();
			}

			if(RESET_MEM)
			{
				// still to be implemented
			}

			if(RESET_MACHINE)
			{
				inst.GetMotherboard().reset();
			}
		}
	}

	void mem(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[Memory Information]\n");

		mips_tools::mb& cmp = inst.GetMotherboard();
		

		// No args specified just print the memory size (in bytes)
		if(args.size() <= 1)
		{
			fprintf(stdout, "Main memory size: %d bytes\n", cmp.get_mmem_size());
		}

		// Otherwise print memory specific to indicies
		for(size_t itr = 1; itr < args.size(); itr++)
		{
			mips_tools::range r = mips_tools::range(args[itr]);

			for(mips_tools::range_iterator itr_2 = r.begin(); itr_2 != r.end(); itr_2++)
			{
				fprintf(stdout, "Mem[%d]: %d\n", *itr_2, inst.GetMotherboard().DMA_read(*itr_2));
			}
		}
	}

	void power(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[Power Usage Statistics]\n");
		fprintf(stdout, "Not yet implemented\n");
	}

	void sound(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[Soundcard Emulation]\n");
		fprintf(stdout, "Not yet implemented\n");
	}

	void state(std::vector<std::string> & args, Shell& inst)
	{
		
		fprintf(stdout, "[Register State Information]\n");

		mips_tools::mb& cmp = inst.GetMotherboard();
		mips_tools::diag_cpu& dcpu = dynamic_cast<mips_tools::diag_cpu&>(cmp.get_cpu());
		int reg_count = dcpu.get_reg_count();
		mips_tools::BW_32 pc_val = dcpu.get_PC();
		

		// No args specified print out every register
		if(args.size() <= 1)
		{
			fprintf(stdout, "PC: %d\n", pc_val);

			for(int r = 0; r < reg_count; r++)
			{
				fprintf(stdout, "REG %d:\t%d\n", r, dcpu.get_reg_data(r));
			}
		}

		else
		{
			for(size_t itr = 1; itr < args.size(); itr++)
			{
				mips_tools::range r(args[itr]);

				for(mips_tools::range_iterator ritr = r.begin(); ritr != r.end(); ritr++)
				{
					fprintf(stdout, "REG %d:\t%d\n", *ritr, dcpu.get_reg_data(*ritr));
				}
			}
		}
	}

	void time(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[Processor Timing Information]\n");
		fprintf(stdout, "Not yet implemented\n");
	}

	void trace(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[Special Tracing Options]\n");
		fprintf(stdout, "Not yet implemented\n");
	}

	void vga(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(stdout, "[VGA Emulation]\n");
		fprintf(stdout, "Not yet implemented\n");
	}
}