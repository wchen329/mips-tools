#ifndef __SUPERSCALAR_H__
#define __SUPERSCALAR_H__

#include <queue>
#include <vector>
#include "sc_cpu.h"

namespace mips_tools

/* Register Map
 * A general register map usable for superscalar processors
 */
class register_map
{

	public:
		/* preg_count - physical register count
		 * vreg_count - "virtual" or ISA register count
		 */
		register_map(int preg_count, vreg_count) : preg_num(preg_count), vreg_num(vreg_count)
								rmap(preg_count)
		{
			for(size_t cont = 0; cont < rmap.size(); cont++)
			{
				rmap[cont] = -1; // -1 means "not currently mapped"
			}
		}

		int getMappingToPReg(int vregNum) { return this->rmap[vregNum]; }
		int setMappingToPReg(int vregNum, int pregNum) { this->rmap[vregNum] = pregNum; }
		int clearMapping(int vregNum) { this->rmap[vregNum] = -1; }

	private:
		int preg_num;
		int vreg_num;
		std::vector<int> rmap;
};

/* MIPS R10K Superscalar Processor Clone
 *
 */
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
		std::queue<int> reg_free_list;	// free list for denoting usable registers
};

#endif
