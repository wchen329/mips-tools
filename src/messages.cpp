#include "messages.h"

/* Various command line messages
 *
 * CURRENTLY: placeholders, but will be replaced by file I/O based on localization
 */
namespace mipsshell
{
	char * HELP = ".exit - exit\n.help - show this help message\n.echo - echo register state\n[mips assembly instruction] - execute that instruction\n";			// help message
	char * BAD_COMMAND = "Unknown command. For more information on acceptable commands type \".help\".\n" ;  // bad command message
}