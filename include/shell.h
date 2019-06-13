#ifndef __SHELL_H__
#define __SHELL_H__
#include <string>
#include <vector>
#include "mb.h"

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
			Shell() : motherboard(nullptr), isQuiet(false) { }
			~Shell() { delete motherboard; }
		private:
			Shell& operator=(const Shell&);
			Shell(const Shell&);
			std::vector<std::string> args;
			mips_tools::mb * motherboard;
			bool isQuiet;
	};
}

#endif