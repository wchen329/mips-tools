#ifndef __DIAG_CPU_H__
#define __DIAG_CPU_H__
#include <vector>
#include "cpu.h"
#include "mips.h"
#include "primitives.h"

namespace mips_tools
{

	// CPU with Diagnostic Functions
	class diag_cpu : public cpu
	{
		public:
				// still need to do: change BW_32 to a generic
				virtual BW_32& get_reg_data(int index) = 0;
				virtual int get_reg_count() = 0;
				virtual void ghost_cycle() = 0;	// ghost cycle- essentially cycling with a NO-OP, advances the PC.
				virtual BW_32& get_PC() = 0;	// get current PC value
				virtual std::vector<NameDescPair>& get_CPU_options() = 0;
				virtual void exec_CPU_option(std::vector<std::string> &) = 0;	// execute a special CPU specific option
	};
}
#endif