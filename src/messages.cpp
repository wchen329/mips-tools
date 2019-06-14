#include "messages.h"

/* Various command line messages
 *
 * CURRENTLY: placeholders, but will be replaced by file I/O based on localization
 */
namespace mipsshell
{
	const std::string HELP =	std::string(".breakpoint - set breakpoints which will bring up the interactive prompt, that can be used for debugging\n") +
								std::string(".cycle - advance the processor a cycle given the current PC\n") +
								std::string(".exit - exit\n") +
								std::string(".help - show this help message or get help with commands\n") +
								std::string(".rst - reset CPU state and mem state\n") +
								std::string(".state - echo register state\n") +
								std::string(".mem - show main memory size\n") +
								std::string(".time - show simulation timing information\n") +
								std::string("[assembly instruction] - execute that instruction, write it to the PC location and cycle\n") +
								std::string("For more help on specific commands use \"help [command]\" (for example .help .rst)\n");
	char * BAD_COMMAND = "Unknown command. For more information on acceptable commands type \".help\".\n" ;  // bad command message
	char * BAD_FORMAT = "Bad or unsupported register format.\n";
	char * NON_DIAG_CPU = "Warning: The CPU used does not have diagnostic capabilities, such CPUs are unable to receive fetches from interactive mode...\n";
	const std::string HELP_BREAKPOINT =	std::string(".breakpoint\n") +
										std::string("Insert a breakpoint into the program corresponding to a line number or a cycle\n") +
										std::string("Execution will be stopped before the corresponding cycle has passed in execution.\n") +
										std::string("Options:\n") +
										std::string("-l [line_number]: insert a breakpoint at a line number. This can only used with an input file specified\n") +
										std::string("-c [cycle number]: break when a certain cycle count is reached (but not yet executed)\n");
	const std::string HELP_EXIT =	std::string(".exit\n") +
									std::string("Use this to exit program...\n");
	const std::string HELP_RST =	std::string(".rst\n") +
									std::string("Use this to reset certain components of the machine\n") +
									std::string("Options:\n") +
									std::string("[no args] - reset both the CPU state and mem state. This is the same as -a (see below).\n") +
									std::string("-c: reset the CPU state\n") +
									std::string("-m: zero out main memory (not yet implemented)\n") +
									std::string("-a: reset both the CPU state and mem state\n");
	const std::string HELP_TIME =	std::string(".time\n") +
									std::string("Print out information about timing information\n");
}