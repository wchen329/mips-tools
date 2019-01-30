#define _CRT_SECURE_NO_WARNINGS	// for MSVC
#include <cstdio>
#include <cstdlib>
#include "mb.h"
#include "interpret.h"
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
	FILE * inst_file;
	fprintf(stdout, "MIPS Tools Developmental Version\n");

	// First get the active file in which to get instructions from
	if(argc >= 2)
	{
		fprintf(stdout, "Starting in batch mode...\n");
		mipsshell::INTERACTIVE = false;
		inst_file = fopen(argv[1], "r");
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
		fprintf(stdout, "------------------------------------\n");
		fprintf(stdout, "- Routine calls used with . prefix -\n");
		fprintf(stdout, "- Otherwise interpreted as instr.  -\n");
		fprintf(stdout, "------------------------------------\n");
	}

	mips_tools::mb MB_IN(mips_tools::STANDARD, 16);
	MB_IN.reset();
	mips_tools::mb * MB_IN_PTR = &MB_IN;

	while(true)
	{
		char buf[100];
		if(mipsshell::INTERACTIVE) fprintf(stdout, ">> ");
		if(fgets(buf, 100, inst_file) == NULL) break;
		if(mipsshell::interpret(buf, MB_IN_PTR)) break;
		
	}

	fclose(inst_file);
	return 0;
}