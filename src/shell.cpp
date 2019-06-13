#define _CRT_SECURE_NO_WARNINGS	// for MSVC
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <memory>
#include "cpu.h"
#include "diag_cpu.h"
#include "mb.h"
#include "mtsstream.h"
#include "interpret.h"
#include "parser_err.h"
#include "runtime_call.h"
#include "shell.h"
#include "states.h"

namespace mipsshell
{
	/* A mapping from a string into a directive function pointer (must be a member of Shell)
	 */
	typedef std::pair<std::string, void(*)(std::vector<std::string> &, Shell&)> directive_pair;

	/* Shell for MIPS Tools
	 *
	 * The shell has two modes: Interative Mode and Batch Mode
	 *
	 * Passing in a file as an argument allows for that instruction to be batched.
	 *
	 * wchen329
	 */
	void Shell::Run()
	{
		size_t argc = args.size();
		mips_tools::cpu_t cp = mips_tools::STANDARD;
		int mem_width = 16;
		FILE * inst_file = NULL;

		if(!isQuiet)
		fprintf(stdout, "MIPS Tools 0.1 (developmental build)\n");

		// First get the active file in which to get instructions from
		if(argc >= 2)
		{
			for(size_t i = 1; i < argc; i++)
			{
				if(args[i] == "-h")
				{
					fprintf(stdout, "Usage options:\n");
					fprintf(stdout, "-h (show this message)\n");
					fprintf(stdout, "-i [file] (execute a text file with MIPS Tools commands and assembly directly using JIT compiling)\n");
					fprintf(stdout, "-m [width] (specify a memory bit width; the total memory available will be 2^[width]\n");
					fprintf(stdout, "-a (active assembler mode, requires the -i option):\n");
					fprintf(stdout, "-c (select a CPU type 0: for single cycle (default) or 1: for Five Stage Pipeline)\n");
					::exit(0);
				}

				if(args[i] == "-a")
				{
					if(!isQuiet) fprintf(stdout, "Assembler mode ENABLED.\n");
					mipsshell::ASM_MODE = true;
					mipsshell::INTERACTIVE = false;
				}

				if(args[i] == "-i")
				{
					mipsshell::INTERACTIVE = false;
					mipsshell::HAS_INPUT = true;
					if((i+1) < argc)
					{
						inst_file = fopen(args[i+1].c_str(), "r");
						mipsshell::INPUT_SPECIFIED = true;
					}
				}

				if(args[i] == "-WIN32_SHELL")
				{
					mipsshell::INTERACTIVE = false;
					mipsshell::HAS_INPUT = true;
					inst_file = stdin;
					mipsshell::INPUT_SPECIFIED = true;
					mipsshell::WIN_32_GUI = true;
				}

				if(args[i] == "-m")
				{
					if((i+1) < argc) mem_width = atoi(args[i+1].c_str());
				}

				if(args[i] == "-c")
				{
					if((i+1) < argc) cp = static_cast<mips_tools::cpu_t>(atoi(args[i+1].c_str()));
				}
			}
		}

		if(mipsshell::HAS_INPUT)
		{
			if(!mipsshell::WIN_32_GUI)
			{
				if(!isQuiet) fprintf(stdout, "Starting in batch mode...\n");
				mipsshell::INTERACTIVE = false;
			}

			if(!mipsshell::INPUT_SPECIFIED)
			{
				fprintf(stderr, "Error: An input file is required (specified through -i [input file] ) in order to run in batch mode.\n");
				::exit(1);
			}

			if(inst_file == NULL)
			{
				fprintf(stderr, "Error: The file specified cannot be opened or doesn't exist.\n");
				::exit(1);
			}
		}

		else
		{
			mipsshell::INTERACTIVE = true;
			inst_file = stdin;
			if(!isQuiet) fprintf(stdout, "Starting in interactive mode...\n");
			if(!isQuiet) fprintf(stdout, "Tip: system directives are preceded by a . (for example .help)\n");
		}

		if(mem_width <= 0)
		{
			fprintf(stderr, "Error: An error occurred when trying to read memory width (must be larger than 0 and a natural number).");
			::exit(1);
		}

		if(mem_width > 32)
		{
			fprintf(stderr, "Error: Memory size specified is too large (%d bits wide > 20 bits wide)", mem_width);
			::exit(1);
		}

		if(!isQuiet) fprintf(stdout, "CPU Type: ");
		switch(cp)
		{
			case mips_tools::STANDARD:
				if(!isQuiet) fprintf(stdout, "Single Cycle\n");
				break;
			case mips_tools::FIVE_P:
				if(!isQuiet)fprintf(stdout, "Five Stage Pipeline\n");
				break;
			default:
				if(!isQuiet) fprintf(stdout, "Invalid CPU type detected. Exiting...\n");
				::exit(1);
		}

		this->motherboard = new mips_tools::mb(cp, mem_width, mipsshell::SUSPEND);
		motherboard->reset();
		mips_tools::mb * MB_IN_PTR = motherboard;
		if(mipsshell::ASM_MODE) mipsshell::mtsstream::asmout = new mipsshell::asm_ostream("a.bin");

		if(!isQuiet) fprintf(stdout, "Main Memory size: %d bytes\n", motherboard->get_mmem_size());

		/* Actual Execution Portion
		 */


		/* First, if an input file was specified
		 * (1) collect file symbols
		 * (2) map it to memory assemble that file first
		 */
		if(INPUT_SPECIFIED)
		{
			std::vector<std::string> lines;

			mips_tools::BW_32 equiv_pc = 0;
			char input_f_stream[255];
			memset(input_f_stream, 0, sizeof(input_f_stream));
			while(fgets(input_f_stream, 254, inst_file) != NULL)
			{
				std::string current_line = std::string(input_f_stream);
				std::vector<std::string> parts = chop_string(current_line);
	
				// Remove strings that are just whitespace
				if(parts.empty())
					continue;
					
				// Symbol assignment: add the symbol to symbol table
					
				if(parts[0][parts[0].size() - 1] == ':')
				{
					this->jump_syms.insert(parts[0].substr(0, parts[0].size() - 1), equiv_pc);
					continue;
				}

				if(parts[0][0] == '.')
				{
					this->directive_syms.insert(current_line, equiv_pc);
					continue;
				}

				equiv_pc = equiv_pc + 4;
				lines.push_back(current_line);
			}

			mips_tools::BW_32 asm_pc = 0;

			// Now assemble the rest
			for(size_t itr = 0; itr < lines.size(); itr++)
			{
				assemble(lines[itr].c_str(), MB_IN_PTR, asm_pc);
				asm_pc += 4;
			}
			
			this->state = SLEEPING;
		}

		while(this->state != KILLED)
		{

		/* Second, execute.
		 * To execute:
		 * - If the PC can be debugged, then just run it. Ignore "debugging" symbols.
		 * - If the PC can be debugged, then for every PC, execute the runtime directive associated with the PC BEFORE the instruction is fetched
		 *   then run the CPU a cycle, and repeat.
		 * If no input file was specified, then it starts in interactive / debugging mode
		 *
		 */

		char buf[100];
		if(mipsshell::INTERACTIVE)
		{
			fprintf(stdout, ">> ");
		}

		if(mipsshell::INTERACTIVE || mipsshell::ASM_MODE)
		{
			mips_tools::diag_cpu & dcpu = dynamic_cast<mips_tools::diag_cpu&>(motherboard->get_cpu());
			if(fgets(buf, 100, inst_file) == NULL) break;
			if(assemble(buf, MB_IN_PTR, dcpu.get_PC())) break;
		}

		if(!mipsshell::INTERACTIVE && !mipsshell::ASM_MODE)
		{

			signal(SIGINT, mipsshell::Enter_Interactive);
			while(this->state != KILLED)
			{
				mips_tools::diag_cpu & dcpu = dynamic_cast<mips_tools::diag_cpu&>(motherboard->get_cpu());
				mips_tools::BW_32 dpc = dcpu.get_PC();
				
				if(this->directive_syms.has(dpc))
				{
					std::vector<std::string> cmds = this->directive_syms.lookup_from_PC(dpc);
					
					for(size_t cmdcount = 0; cmdcount < cmds.size(); cmdcount++)
						execute_runtime_directive(chop_string(cmds[cmdcount]));
				}

				motherboard->get_cpu().cycle();
			}
		}
	}

		fclose(inst_file);
		delete mipsshell::mtsstream::asmout;
}

	/* Takes an input string and breaks that string into a vector of several
	 * based off of whitespace and tab delineators
	 * Also removes comments
	 * "Also acknowledges " " and ' ' and \ all used for escaping
	 */
	std::vector<std::string> chop_string(std::string & input)
	{
		std::string commentless_input;
		size_t real_end = input.size();
		for(size_t cind = 0; cind < input.size(); cind++)
		{
			if(input[cind] == '#')
			{
				real_end = cind;
				break;
			}
		}

		commentless_input = input.substr(0, real_end);

		std::vector<std::string> str_vec;
		
		std::string built_string = "";

		bool has_escaped = false;
		bool in_quotes = false;

		// Use a linear search
		for(size_t ind = 0; ind < commentless_input.size(); ind++)
		{
			// If no escaping, then perform syntactical checks
			if(!has_escaped)
			{
				// First acknowledge escaping
				if(commentless_input[ind] == '\\')
				{
					has_escaped = true;
					continue;
				}

				// Detect quotations
				if(commentless_input[ind] == '\"' || commentless_input[ind] == '\'')
				{
					in_quotes = !in_quotes;
					continue;
				}

				// Now if not quoted as well, then a comma, whitespace, tab, or newline delineates that argument is done parsing
				if(!in_quotes)
				{
					if(commentless_input[ind] == ',' ||  commentless_input[ind] == ' ' || commentless_input[ind] == '\t' || commentless_input[ind] == '\n')
					{
						// Check: do not add empty strings
						if(built_string != "")
						{
							str_vec.push_back(built_string);
							built_string = "";
						}

						continue;
					}
				}
			}

			built_string += commentless_input[ind];
			has_escaped = false; // no matter what, escaping only escapes one...
		}

		if(has_escaped || in_quotes)
		{
			throw bad_escape_err();
		}

		return str_vec;
	}

	// Set up list of runtime directives
	Shell::Shell() : motherboard(nullptr), isQuiet(false)
	{
		this->state = EMBRYO;

		// Set up jump table for runtime directives
		this->directives.insert(directive_pair(".breakpoint", mipsshell::breakpoint));
		this->directives.insert(directive_pair(".exit", mipsshell::exit));
		this->directives.insert(directive_pair(".help", mipsshell::help));
		this->directives.insert(directive_pair(".mem", mipsshell::mem));
		this->directives.insert(directive_pair(".pci", mipsshell::pci));
		this->directives.insert(directive_pair(".cpuopts", mipsshell::cpuopts));
		this->directives.insert(directive_pair(".power", mipsshell::power));
		this->directives.insert(directive_pair(".rst", mipsshell::rst));
		this->directives.insert(directive_pair(".sound", mipsshell::sound));
		this->directives.insert(directive_pair(".state", mipsshell::state));
		this->directives.insert(directive_pair(".trace", mipsshell::trace));
		this->directives.insert(directive_pair(".time", mipsshell::time));
		this->directives.insert(directive_pair(".vga", mipsshell::vga));
	}
}