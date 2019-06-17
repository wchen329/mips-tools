#define _CRT_SECURE_NO_WARNINGS	// for MSVC
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>
#include <memory>
#include "branding.h"
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

		std::string app_brand = branding::APPLICATION_NAME + " " + branding::APPLICATION_VERSION + " " + branding::APPLICATION_DBG;

		if(!isQuiet)
		{
			fprintf(output, "%s\n", app_brand.c_str());
			fprintf(output, "%s\n", branding::APPLICATION_DESC.c_str());
		}

		// First get the active file in which to get instructions from
		if(argc >= 2)
		{
			for(size_t i = 1; i < argc; i++)
			{
				if(args[i] == "-h")
				{
					fprintf(output, "Usage options:\n");
					fprintf(output, "-h (show this message)\n");
					fprintf(output, "-i [file] (execute a text file with MIPS Tools commands and assembly directly using JIT compiling)\n");
					fprintf(output, "-m [width] (specify a memory bit width; the total memory available will be 2^[width]\n");
					fprintf(output, "-a (active assembler mode, requires the -i option):\n");
					fprintf(output, "-c (select a CPU type 0: for single cycle (default) or 1: for Five Stage Pipeline)\n");
					::exit(0);
				}

				if(args[i] == "-a")
				{
					if(!isQuiet) fprintf(output, "Assembler mode ENABLED.\n");
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
				if(!isQuiet) fprintf(output, "Starting in batch mode...\n");
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
			if(!isQuiet) fprintf(output, "Starting in interactive mode...\n");
			if(!isQuiet) fprintf(output, "Tip: system directives are preceded by a . (for example .help)\n");
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

		if(!isQuiet) fprintf(output, "CPU Type: ");
		switch(cp)
		{
			case mips_tools::STANDARD:
				if(!isQuiet) fprintf(output, "Single Cycle\n");
				break;
			case mips_tools::FIVE_P:
				if(!isQuiet)fprintf(output, "Five Stage Pipeline\n");
				break;
			default:
				if(!isQuiet) fprintf(output, "Invalid CPU type detected. Exiting...\n");
				::exit(1);
		}

		this->motherboard = new mips_tools::mb(cp, mem_width, mipsshell::SUSPEND);
		motherboard->reset();
		mips_tools::mb * MB_IN_PTR = motherboard;
		if(mipsshell::ASM_MODE) mipsshell::mtsstream::asmout = new mipsshell::asm_ostream("a.bin");

		if(!isQuiet) fprintf(output, "Main Memory size: %d bytes\n", motherboard->get_mmem_size());

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
			unsigned long line_number = 0;
			while(fgets(input_f_stream, 254, inst_file) != NULL)
			{
				line_number++;
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

				this->line_number_to_PC.insert(std::pair<unsigned long, mips_tools::BW_32>(line_number, equiv_pc));
				this->PC_to_line_number.insert(std::pair<mips_tools::BW_32, unsigned long>(equiv_pc, line_number));
				this->PC_to_line_string.insert((std::pair<mips_tools::BW_32, std::string>(equiv_pc, current_line)));
				equiv_pc = equiv_pc + 4;
				lines.push_back(current_line);		
			}

			mips_tools::BW_32 asm_pc = 0;

			// Now assemble the rest
			for(size_t itr = 0; itr < lines.size(); itr++)
			{
				std::vector<std::string> asm_args = chop_string(lines[itr]);
				assemble(asm_args, MB_IN_PTR, asm_pc);
				asm_pc += 4;
			}
			
			this->state = SLEEPING;
		}

		inst_file = stdin;

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
			fprintf(output, ">> ");
		}

		if(mipsshell::INTERACTIVE || mipsshell::ASM_MODE)
		{
			mips_tools::diag_cpu & dcpu = dynamic_cast<mips_tools::diag_cpu&>(motherboard->get_cpu());
			if(fgets(buf, 100, inst_file) == NULL) break;
			
			if(buf[0] == '.')
			{
				try
				{
					std::string str_buf(buf);
					std::vector<std::string> chopped = chop_string(str_buf);
					execute_runtime_directive(chopped);
				}
				catch(mips_tools::mt_exception & e)
				{
					fprintf(output, e.get_err().c_str());
				}

				continue;
			}

			std::string buf_str(buf);
			std::vector<std::string> asm_args = chop_string(buf_str);
			if(assemble(asm_args, MB_IN_PTR, dcpu.get_PC())) break;
		}

		if(!mipsshell::INTERACTIVE && !mipsshell::ASM_MODE)
		{

			signal(SIGINT, mipsshell::Enter_Interactive);
			while(this->state != KILLED && !mipsshell::SUSPEND)
			{
				mips_tools::diag_cpu & dcpu = dynamic_cast<mips_tools::diag_cpu&>(motherboard->get_cpu());
				mips_tools::BW_32 dpc = dcpu.get_PC();
				
				if(this->directive_syms.has(dpc))
				{
					std::vector<std::string> cmds = this->directive_syms.lookup_from_PC(dpc);
					
					for(size_t cmdcount = 0; cmdcount < cmds.size(); cmdcount++)
					{
						std::vector<std::string> chopped = chop_string(cmds[cmdcount]);
						execute_runtime_directive(chopped);
					}
				}

				if(this->has_prog_break_at(dcpu.get_PC()))
				{
					mipsshell::SUSPEND = true;
					mipsshell::INTERACTIVE = true;
					unsigned long line_number = this->PC_to_line_number.at(dcpu.get_PC());
					std::string line_str = this->PC_to_line_string.at(dcpu.get_PC());
					fprintf(output, "Breakpoint at line %d hit.\n", line_number);
					fprintf(output, "line %d:\n\t%s\n", line_number, line_str.c_str());
				}

				if(this->has_ma_break_at(motherboard->get_cycles()))
				{
					mipsshell::SUSPEND = true;
					mipsshell::INTERACTIVE = true;
					fprintf(output, "Breakpoint at cycle %d hit.\n", motherboard->get_cycles());
				}

				motherboard->step();
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
					if(commentless_input[ind] == ',' ||  commentless_input[ind] == ' ' || commentless_input[ind] == '\t' || commentless_input[ind] == '\n' || commentless_input[ind] == '\r')
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
		this->output = stdout;
		this->input = stdin;

		// Set up jump table for runtime directives
		this->directives.insert(directive_pair(".breakpoint", mipsshell::breakpoint));
		this->directives.insert(directive_pair(".cycle", mipsshell::cycle));
		this->directives.insert(directive_pair(".exit", mipsshell::exit));
		this->directives.insert(directive_pair(".help", mipsshell::help));
		this->directives.insert(directive_pair(".mem", mipsshell::mem));
		this->directives.insert(directive_pair(".pci", mipsshell::pci));
		this->directives.insert(directive_pair(".cpuopts", mipsshell::cpuopts));
		this->directives.insert(directive_pair(".power", mipsshell::power));
		this->directives.insert(directive_pair(".rst", mipsshell::rst));
		this->directives.insert(directive_pair(".run", mipsshell::run));
		this->directives.insert(directive_pair(".sound", mipsshell::sound));
		this->directives.insert(directive_pair(".state", mipsshell::state));
		this->directives.insert(directive_pair(".trace", mipsshell::trace));
		this->directives.insert(directive_pair(".time", mipsshell::time));
		this->directives.insert(directive_pair(".vga", mipsshell::vga));
	}
	void Shell::add_program_breakpoint(unsigned long line)
	{
		if(this->line_number_to_PC.count(line) > 0)
		{
			mips_tools::BW_32 line_pc = this->line_number_to_PC.at(line);
			this->program_breakpoints.insert(std::pair<mips_tools::BW_32, unsigned long>(line_pc, line));
			fprintf(stdout, "Breakpoint set at line %ld\n", line);
		}

		else
		{
			fprintf(stderr, "Invalid line number. The line number may not be associated with an instruction or may not be in the file.\n");
		}
	}
}