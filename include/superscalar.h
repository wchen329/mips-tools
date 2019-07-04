#ifndef __SUPERSCALAR_H__
#define __SUPERSCALAR_H__

#include <vector>
#include "sc_cpu.h"

namespace mips_tools

class r10k_superscalar : sc_cpu
{

	public:
		void rst();
		void cycle();
		void exec_CPU_option(std::vector<std::string> &);
                
		/* R10K Superscalar constructor
		 * To make this type of processor some parameters should be specified...
		 * -
		 * inst_buffer_entries - How many instructions will this
		 * 	superscalar processor be able to hold at a time?
		 * (default): 1 entry
		 * -
		 * phys_reg_count - How many physical registers does this
		 * processor have?
		 * (default): 10 registers
		 * -
		 * funit_count - How many functional units does this processor have?
		 * (default): 3 functional units
		 *
		 */
		r10k_superscalar(int inst_buffer_entries = 1, int phys_reg_count = 10,
			funit_count = 3); 
		
	private:
		std::vector<BW_32> inst_buffer; // variable sized instruction buffer	
		
};

#endif
