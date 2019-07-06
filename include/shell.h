#ifndef __SHELL_H__
#define __SHELL_H__
#include <set>
#include <map>
#include <string>
#include <vector>
#include "mb.h"
#include "syms_table.h"
#include "streams.h"

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
			
			void WriteToOutput(std::string& o);
			void WriteToError(std::string& e);
			std::string& ReadFromInput();

			void add_program_breakpoint(unsigned long line);
			void add_microarch_breakpoint(unsigned long cycle) { this->microarch_breakpoints.insert(std::pair<unsigned long, bool>(cycle, true)); }
			void setOutputTextStream(priscas_io::text_stream & ts) { this->tw_output = &ts; }
			void setErrorTextStream(priscas_io::text_stream & ts) { this->tw_error = &ts; }
			void setInputTextStream(priscas_io::text_stream & ts) { this->tw_input = &ts; }
			void setNoConsoleOutput(bool torf) { this->NoConsoleOutput = torf; }
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
			bool NoConsoleOutput;
			priscas_io::text_stream * tw_error;
			priscas_io::text_stream * tw_output;
			priscas_io::text_stream * tw_input;
			std::string rd_buffer;
			Shell& operator=(const Shell&);
			Shell(const Shell&);
			std::vector<std::string> args;
			mips_tools::mb * motherboard;
			bool isQuiet;

			Shell_State state;

			// Runtime Directives, run through the shell
			std::map<mips_tools::BW_32, unsigned long> program_breakpoints;
			std::map<mips_tools::BW_32, unsigned long> PC_to_line_number;
			std::map<unsigned long, mips_tools::BW_32> line_number_to_PC;
			std::map<mips_tools::BW_32, std::string> PC_to_line_string;
			std::map<unsigned long, bool> microarch_breakpoints;
			std::map<std::string, void(*)(std::vector<std::string>&, Shell& shell)> directives;
			mips_tools::syms_table jump_syms;
			mips_tools::syms_table directive_syms;
			void execute_runtime_directive(std::vector<std::string>& args_list);
			bool has_ma_break_at(unsigned long line){ return this->microarch_breakpoints.count(line) > 0; }
			bool has_prog_break_at(unsigned long line){ return this->program_breakpoints.count(line) > 0; }
	};
}

#endif