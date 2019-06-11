#ifndef __SHELL_H__
#define __SHELL_H__
#include <string>
#include <vector>
#include "mb.h"

namespace mipsshell
{
	/* A single instance of a Shell
	 * -
	 * The shell allows easy and direct access to utilizing a processor.
	 */
	class Shell
	{
		public:
			void Run();
			void SetArgs(std::vector<std::string> & args) { this->args = args; }
			mips_tools::mb& GetMotherboard() { return this->motherboard; } // Call this **after** Run
		private:
			std::vector<std::string> args;
			mips_tools::mb motherboard;
	};
}

#endif