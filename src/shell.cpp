#define _CRT_SECURE_NO_WARNINGS	// for MSVC
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include "cpu.h"
#include "diag_cpu.h"
#include "mb.h"
#include "mtsstream.h"
#include "interpret.h"
#include "runtime_call.h"
#include "states.h"

/* Shell for MIPS Tools
 *
 * The shell has two modes: Interative Mode and Batch Mode
 *
 * Passing in a file as an argument allows for that instruction to be batched.
 *
 * wchen329
 */
int main(int argc, char ** argv)
{
	mips_tools::cpu_t cp = mips_tools::FIVE_P;
	int mem_width = 16;
	FILE * inst_file = NULL;
	fprintf(stdout, "MIPS Tools 0.1 (developmental build)\n");

	// First get the active file in which to get instructions from
	if(argc >= 2)
	{
		for(int i = 1; i < argc; i++)
		{
			if(!strcmp(argv[i], "-h"))
			{
				fprintf(stdout, "Usage options:\n");
				fprintf(stdout, "-h (show this message)\n");
				fprintf(stdout, "-i [file] (execute a text file with MIPS Tools commands and assembly directly using JIT compiling)\n");
				fprintf(stdout, "-m [width] (specify a memory bit width; the total memory available will be 2^[width]\n");
				fprintf(stdout, "-a (active assembler mode, requires the -i option):\n");
				fprintf(stdout, "-c (select a CPU type 0: for single cycle (default) or 1: for Five Stage Pipeline)\n");
				exit(0);
			}

			if(!strcmp(argv[i], "-a"))
			{
				fprintf(stdout, "Assembler mode ENABLED.\n");
				mipsshell::ASM_MODE = true;
				mipsshell::INTERACTIVE = false;
			}

			if(!strcmp(argv[i], "-i"))
			{
				mipsshell::INTERACTIVE = false;
				mipsshell::HAS_INPUT = true;
				if((i+1) < argc)
				{
					inst_file = fopen(argv[i+1], "r");
					mipsshell::INPUT_SPECIFIED = true;
				}
			}

			if(!strcmp(argv[i], "-WIN32_SHELL"))
			{
				mipsshell::INTERACTIVE = false;
				mipsshell::HAS_INPUT = true;
				inst_file = stdin;
				mipsshell::INPUT_SPECIFIED = true;
				mipsshell::WIN_32_GUI = true;
			}

			if(!strcmp(argv[i], "-m"))
			{
				if((i+1) < argc) mem_width = atoi(argv[i+1]);
			}

			if(!strcmp(argv[i], "-c"))
			{
				if((i+1) < argc) cp = static_cast<mips_tools::cpu_t>(atoi(argv[i+1]));
			}
		}
	}

	if(mipsshell::HAS_INPUT)
	{
		if(!mipsshell::WIN_32_GUI)
		{
			fprintf(stdout, "Starting in batch mode...\n");
			mipsshell::INTERACTIVE = false;
		}

		if(!mipsshell::INPUT_SPECIFIED)
		{
			fprintf(stderr, "Error: An input file is required (specified through -i [input file] ) in order to run in batch mode.\n");
			exit(1);
		}

		if(inst_file == NULL)
		{
			fprintf(stderr, "Error: The file specified cannot be opened or doesn't exist.\n");
			exit(1);
		}
	}

	else
	{
		mipsshell::INTERACTIVE = true;
		inst_file = stdin;
		fprintf(stdout, "Starting in interactive mode...\n");
		fprintf(stdout, "Tip: system directives are preceded by a . (for example .help)\n");
	}

	if(mem_width <= 0)
	{
		fprintf(stderr, "Error: An error occurred when trying to read memory width (must be larger than 0 and a natural number).");
		exit(1);
	}

	if(mem_width > 32)
	{
		fprintf(stderr, "Error: Memory size specified is too large (%d bits wide > 20 bits wide)", mem_width);
		exit(1);
	}

	fprintf(stdout, "CPU Type: ");
	switch(cp)
	{
		case mips_tools::STANDARD:
			fprintf(stdout, "Single Cycle\n");
			break;
		case mips_tools::FIVE_P:
			fprintf(stdout, "Five Stage Pipeline\n");
			break;
		default:
			fprintf(stdout, "Invalid CPU type detected. Exiting...\n");
			exit(1);
	}

	mips_tools::mb MB_IN(cp, mem_width, mipsshell::SUSPEND);
	MB_IN.reset();
	mips_tools::mb * MB_IN_PTR = &MB_IN;
	if(mipsshell::ASM_MODE) mipsshell::mtsstream::asmout = new mipsshell::asm_ostream("a.bin");

	fprintf(stdout, "Main Memory size: %d bytes\n", MB_IN.get_mmem_size());

	while(!mipsshell::EXIT_COND)
	{
		char buf[100];
		if(mipsshell::INTERACTIVE)
		{
			fprintf(stdout, ">> ");
		}

		if(mipsshell::INTERACTIVE || mipsshell::ASM_MODE)
		{
			if(fgets(buf, 100, inst_file) == NULL) break;
			if(mipsshell::interpret(buf, MB_IN_PTR)) break;
		}

		if(!mipsshell::INTERACTIVE && !mipsshell::ASM_MODE)
		{
			// If given a batch file, assemble, assign symbols, then run
			if(mipsshell::HAS_INPUT)
			{
				mipsshell::PRE_ASM = true;
				while(fgets(buf, 100, inst_file) != NULL)
				{			
					mipsshell::interpret(buf, MB_IN_PTR);
				}

				MB_IN.get_cpu().rst();
			}

			signal(SIGINT, mipsshell::Enter_Interactive);
			while(!mipsshell::SUSPEND && !mipsshell::EXIT_COND)
			{
				mipsshell::PRE_ASM = false;
				try
				{
					mips_tools::diag_cpu & dcpu = dynamic_cast<mips_tools::diag_cpu&>(MB_IN.get_cpu());
					mips_tools::BW_32 dpc = dcpu.get_PC();
					std::string& runtime_call = mipsshell::debug_table.lookup_from_PC(dpc);
					mipsshell::interpret(runtime_call.c_str(), MB_IN_PTR);
				}
				catch(...)
				{

				}
				
				MB_IN.get_cpu().cycle();
			}
		}
	}



	fclose(inst_file);
	delete mipsshell::mtsstream::asmout;
	return 0;
}