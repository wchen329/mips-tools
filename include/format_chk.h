#ifndef __FORMAT_CHK_H__
#define __FORMAT_CHK_H__

#include "interpret.h"

namespace mipsshell
{
	/* Checks if an instruction is I formatted.
	 */
	bool i_inst(op operation);

	/* Checks if an instruction is R formatted.
	 */
	bool r_inst(op operation);
}

#endif