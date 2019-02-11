#include "messages.h"

/* Various command line messages
 *
 * CURRENTLY: placeholders, but will be replaced by file I/O based on localization
 */
namespace mipsshell
{

	char * HELP = ".exit - exit\n.help - show this help message\n.rst - reset CPU state and mem state\n.state - echo register state\n.mem - show main memory size\n.time - show simulation timing information\n[mips assembly instruction] - execute that instruction\n";			// help message
	char * BAD_COMMAND = "Unknown command. For more information on acceptable commands type \".help\".\n" ;  // bad command message
	char * BAD_FORMAT = "Bad or unsupported register format.\n";
	char * NON_DIAG_CPU = "Warning: The CPU used does not have diagnostic capabilities, such CPUs are unable to receive fetches from interactive mode...\n";
}