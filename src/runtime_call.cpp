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
		fprintf(inst.getOutputStream(), "[Breakpoint]\n");
		bool HAS_SPECIFIER = false;
		for(size_t a = 0; a < args.size(); a++)
		{
			if(args[a] == "-l")
			{
				HAS_SPECIFIER = true;
				if(args.size() > (a + 1))
				{
					errno = 0;
					long n = strtol(args[a + 1].c_str(), nullptr, 10);
					if(errno != 0)
					{
						throw mips_tools::mt_exception();
					}
					
					inst.add_program_breakpoint(n);
				}

				else throw mips_tools::mt_exception();
			}

			if(args[a] == "-c")
			{
				HAS_SPECIFIER = true;
				if(args.size() > (a + 1))
				{
					errno = 0;
					long n = strtol(args[a + 1].c_str(), nullptr, 10);
					if(errno != 0)
					{
						throw mips_tools::mt_exception();
					}

					inst.add_microarch_breakpoint(n);
					fprintf(inst.getOutputStream(), "Breakpoint set at cycle %ld\n", n);
				}

				else throw mips_tools::mt_exception();
			}
		}

		if(!HAS_SPECIFIER)
		{
			fprintf(inst.getOutputStream(), "Usage: .breakpoint -c [cycle_number] or .breakpoint -l [line_number]\n");
		}
	}

	void cycle(std::vector<std::string> & args, Shell& inst)
	{
		inst.GetMotherboard().step();
	}

	void cpuopts(std::vector<std::string> & args, Shell& inst)
	{
		mips_tools::cpu& c = inst.GetMotherboard().get_cpu();
		mips_tools::diag_cpu & dcpu = dynamic_cast<mips_tools::diag_cpu&>(c);
		
		std::vector<mips_tools::NameDescPair>& v = dcpu.get_CPU_options();

		if(args.size() <= 1)
		{
			fprintf(inst.getOutputStream(), "[CPU Specific Options]\n");
			if(v.empty())
			{
				fprintf(inst.getOutputStream(), "No CPU options found.\n");
			}

			else
			{
				for(size_t s = 0; s < v.size(); s++)
				{
					fprintf(inst.getOutputStream(), "%s - %s\n", v[s].getName().c_str(), v[s].getDescription().c_str());
				}
			}

			fprintf(inst.getOutputStream(), "To execute an option just enter .cpuopts [option 1] ... [option n] into the shell\n");
		}

		else
		{
			dcpu.exec_CPU_option(args);
		}
	}

	void exit(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(inst.getOutputStream(), "Simulation terminating...\n");
		inst.SetState(Shell::KILLED);
	}

	void help(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(inst.getOutputStream(), "[Help]\n");

		if(args.size() <= 1)
		{
			fprintf(inst.getOutputStream(), HELP.c_str());
			return;
		}

		else
		{
			std::string msg = "Unknown help topic. The command may not exist, or may be undocumented.\n";

			if(args[1] == ".breakpoint")
			{
				msg = HELP_BREAKPOINT;
			}

			if(args[1] == ".exit")
			{
				msg = HELP_EXIT;
			}
			
			if(args[1] == ".rst")
			{
				msg = HELP_RST;
			}

			if(args[1] == ".time")
			{
				msg = HELP_TIME;
			}

			fprintf(inst.getOutputStream(), msg.c_str());
		}
	}

	void pci(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(inst.getOutputStream(), "[PCI Bus Emulation]\n");
		fprintf(inst.getOutputStream(), "Not yet implemented\n");
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
		fprintf(inst.getOutputStream(), "[Memory Information]\n");

		mips_tools::mb& cmp = inst.GetMotherboard();
		

		// No args specified just print the memory size (in bytes)
		if(args.size() <= 1)
		{
			fprintf(inst.getOutputStream(), "Main memory size: %d bytes\n", cmp.get_mmem_size());
		}

		// Otherwise print memory specific to indicies
		for(size_t itr = 1; itr < args.size(); itr++)
		{
			mips_tools::range r = mips_tools::range(args[itr]);

			for(mips_tools::range_iterator itr_2 = r.begin(); itr_2 != r.end(); itr_2++)
			{
				if(*itr_2 >= inst.GetMotherboard().get_mmem_size() || *itr_2 < 0)
				{
					throw mips_tools::mem_oob_exception();
				}

				fprintf(inst.getOutputStream(), "Mem[%d]: %d\n", *itr_2, inst.GetMotherboard().DMA_read(*itr_2));
			}
		}
	}

	void power(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(inst.getOutputStream(), "[Power Usage Statistics]\n");
		fprintf(inst.getOutputStream(), "Not yet implemented\n");
	}

	void run(std::vector<std::string> & args, Shell& inst)
	{
		INTERACTIVE = false;
		SUSPEND = false;
	}

	void sound(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(inst.getOutputStream(), "[Soundcard Emulation]\n");
		fprintf(inst.getOutputStream(), "Not yet implemented\n");
	}

	void state(std::vector<std::string> & args, Shell& inst)
	{
		
		fprintf(inst.getOutputStream(), "[Register State Information]\n");

		mips_tools::mb& cmp = inst.GetMotherboard();
		mips_tools::diag_cpu& dcpu = dynamic_cast<mips_tools::diag_cpu&>(cmp.get_cpu());
		int reg_count = dcpu.get_reg_count();
		mips_tools::BW_32 pc_val = dcpu.get_PC();
		
		mips_tools::ISA& isa = dcpu.get_ISA();
		
		// No args specified print out every register
		if(args.size() <= 1)
		{
			fprintf(inst.getOutputStream(), "PC: %d\n", pc_val);

			for(int r = 0; r < reg_count; r++)
			{
				fprintf(inst.getOutputStream(), "%s:\t%d\n", isa.get_reg_name(r).c_str(), dcpu.get_reg_data(r));
			}
		}

		else
		{
			for(size_t itr = 1; itr < args.size(); itr++)
			{
				mips_tools::range r(args[itr]);

				for(mips_tools::range_iterator ritr = r.begin(); ritr != r.end(); ritr++)
				{
					if(*ritr < 0 || *ritr >= dcpu.get_reg_count())
					{
						throw mips_tools::reg_oob_exception();
					}
					fprintf(inst.getOutputStream(), "%s:\t%d\n", isa.get_reg_name(*ritr).c_str(), dcpu.get_reg_data(*ritr));
				}
			}
		}
	}

	void time(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(inst.getOutputStream(), "[Processor Timing Information]\n");
		unsigned long n = inst.GetMotherboard().get_cycles();
		fprintf(inst.getOutputStream(), "Cycle Count: %d cycles\n", n);

		// Print it out in terms of smallest units
		fprintf(inst.getOutputStream(), "Time Elapsed:\n");
		unsigned long long total_time = inst.GetMotherboard().get_time().getPS();
		unsigned long long total_days = inst.GetMotherboard().get_time().getDays();
		
		if(total_time == 0)
		{
			fprintf(inst.getOutputStream(), "0s\n");
		}
		
		unsigned short picoseconds = (total_time) % 1000;
		total_time = total_time / 1000;

		unsigned short nanoseconds = (total_time) % 1000;
		total_time = total_time / 1000;

		unsigned short microseconds = (total_time) % 1000;
		total_time = total_time / 1000;

		unsigned short milliseconds = (total_time) % 1000;
		total_time = total_time / 1000;

		unsigned short seconds = (total_time) % 60;
		total_time = total_time / 60;

		unsigned short minutes = (total_time) % 60;
		total_time = total_time / 60;

		unsigned short hours = total_time;

		if(total_days > 0)
		{
			fprintf(inst.getOutputStream(), "%lld days\n", total_days);
		}

		if(hours > 0)
		{
			fprintf(inst.getOutputStream(), "%d hrs\n", hours);
		}

		if(minutes > 0)
		{
			fprintf(inst.getOutputStream(), "%d min\n", minutes);
		}

		if(seconds > 0)
		{
			fprintf(inst.getOutputStream(), "%d s\n", seconds);
		}

		if(milliseconds > 0)
		{
			fprintf(inst.getOutputStream(), "%d ms\n", milliseconds);
		}

		if(microseconds > 0)
		{
			fprintf(inst.getOutputStream(), "%d micros\n", microseconds);
		}

		if(nanoseconds > 0)
		{
			fprintf(inst.getOutputStream(), "%d ns\n", nanoseconds);
		}

		if(picoseconds > 0)
		{
			fprintf(inst.getOutputStream(), "%d ps\n", picoseconds);
		}
	}

	void trace(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(inst.getOutputStream(), "[Special Tracing Options]\n");
		fprintf(inst.getOutputStream(), "Not yet implemented\n");
	}

	void vga(std::vector<std::string> & args, Shell& inst)
	{
		fprintf(inst.getOutputStream(), "[VGA Emulation]\n");
		fprintf(inst.getOutputStream(), "Not yet implemented\n");
	}
}