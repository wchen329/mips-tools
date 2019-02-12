/* Declarations for various
 * shell namespace global state variables.
 *
 * Also mb declaration.
 *
 */
#include "states.h"

namespace mipsshell
{
	bool ASM_MODE = false;		// acting in assembler mode? true if is, false if not
	bool DEBUG_MODE = false;
	bool INTERACTIVE = false;	// in interactive mode, true if is, false if not
	bool HAS_INPUT = false;
}