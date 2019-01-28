#ifndef __SC_CPU_H__
#define __SC_CPU_H__
#include "cpu.h"
#include "reg_32.h"
#include "primitives.h"
#include "mips.h"

/* Interface for the single cycle CPU implementation
 */
namespace mips_tools
{

	class sc_cpu : cpu
	{
		public:
			void rst(); // async reset
			void force_fetch(BW_32 codeword) { this -> current_inst.set_data(codeword); };	// force current instruction
			bool cycle();
			BW_32 get_reg_data(int index) { return this->registers[index].get_data(); }
			void encode(int rs, int rt, int rd, int funct, int imm, opcode op);

		private:
			static const int REG_COUNT = 32;
			reg_32 registers[REG_COUNT];
			reg_32 pc;
			reg_32 current_inst;
	};
}
#endif