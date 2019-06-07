#define _CRT_SECURE_NO_WARNINGS	// for MSVC
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
	FILE * inst_file = NULL;
	fprintf(stdout, "MIPS Tools Developmental Version\n");

	// First get the active file in which to get instructions from
	if(argc >= 2)
	{
		for(int i = 1; i < argc; i++)
		{
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
				if((i+1) < argc) inst_file = fopen(argv[i+1], "r");
			}
		}

		fprintf(stdout, "Starting in batch mode...\n");
		mipsshell::INTERACTIVE = false;

		if(!mipsshell::HAS_INPUT)
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

	mips_tools::mb MB_IN(mips_tools::STANDARD, 16, mipsshell::SUSPEND);
	MB_IN.reset();
	mips_tools::mb * MB_IN_PTR = &MB_IN;
	if(mipsshell::ASM_MODE) mipsshell::mtsstream::asmout = new mipsshell::asm_ostream("a.bin");

	while(true)
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
			signal(SIGINT, mipsshell::Enter_Interactive);
			MB_IN.dc_on();
		}
	}



	fclose(inst_file);
	delete mipsshell::mtsstream::asmout;
	return 0;
}