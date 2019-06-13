#ifndef __SHELL_H__
#define __SHELL_H__
#include <map>
#include <string>
#include <vector>
#include "mb.h"
#include "syms_table.h"

namespace mipsshell
{

	/* Divides a string based on whitespace, tabs, commas and newlines
	 * Acknowledges escaping \ and quotes
	 */
	std::vector<std::string> chop_string(std::string & input);

	/* A single instance of a Shell
	 * -
	 * The shell allows easy and direct access to utilizing a processor.
	 */
	class Shell
	{
		public:
			void Run();
			void SetArgs(std::vector<std::string> & args) { this->args = args; }
			mips_tools::mb& GetMotherboard() { return *this->motherboard; } // Call this **after** Run
			void SetQuiet() { isQuiet = true; }
			~Shell() { delete motherboard; }
			Shell();

			/* Internal Shell State
			 *
			 */
			enum Shell_State
			{
				EMBRYO = 0,
				RUNNING = 1,
				KILLED = 2,
				SLEEPING = 3
			};

			void SetState(Shell_State s){ this->state = s; }

		private:
			Shell& operator=(const Shell&);
			Shell(const Shell&);
			std::vector<std::string> args;
			mips_tools::mb * motherboard;
			bool isQuiet;

			Shell_State state;
			
			// Runtime Directives, run through the shell
			std::vector<std::string> input_file_symbols;
			std::map<mips_tools::BW_32, int> input_file_hash_table;
			std::map<std::string, void(*)(std::vector<std::string>&, Shell& shell)> directives;
			mips_tools::syms_table jump_syms;
			mips_tools::syms_table directive_syms;
			bool assemble(const char * line, mips_tools::mb * mb_ptr, mips_tools::BW_32 baseAddress);
			void execute_runtime_directive(std::vector<std::string>& args_list);
	};
}

#endif