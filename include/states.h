#ifndef __STATES_H__
#define __STATES_H__

namespace mipsshell
{
	extern bool ASM_MODE;		// acting in assembler mode? true if is, false if not
	extern bool DEBUG_MODE;		// acting in debug mode? true if is, false if now
	extern bool INTERACTIVE;	// in interactive mode, true if is, false if not
	extern bool HAS_INPUT;		// no input file
	extern bool INPUT_SPECIFIED;
	extern bool SUSPEND;
}

#endif