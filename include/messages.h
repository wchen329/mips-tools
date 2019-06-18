#ifndef __MESSAGES_H__
#define __MESSAGES_H__
#include <string>

namespace mipsshell
{
	extern const std::string HELP;	// help message
	extern const std::string HELP_HELP;
	extern const std::string HELP_BREAKPOINT;
	extern const std::string HELP_CPUOPTS;
	extern const std::string HELP_CYCLE;
	extern const std::string HELP_EXIT;
	extern const std::string HELP_MEM;
	extern const std::string HELP_RST;
	extern const std::string HELP_STATE;
	extern const std::string HELP_TIME;
	extern const std::string HELP_RUN;
	extern char * BAD_COMMAND;  // bad command message
	extern char * BAD_FORMAT;	// bad format
	extern char * NON_DIAG_CPU;	// non diagnostic cpu warning
}

#endif