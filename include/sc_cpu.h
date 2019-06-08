#ifndef __SC_CPU_H__
#define __SC_CPU_H__
#include "diag_cpu.h"
#include "reg_32.h"
#include "primitives.h"
#include "mmem.h"
#include "mips.h"

/* Interface for the single cycle CPU implementation
 */
namespace mips_tools
{

	class sc_cpu : public diag_cpu
	{
		public:
			void rst(); // "async" reset
			bool cycle(); // advance the processor a cycle
			BW_32 get_reg_data(int index) { return this->registers[index].get_data(); }
			int get_reg_count() { return REG_COUNT; }
			BW_32 get_PC() { return this->pc.get_data(); }
			BW_32 encode(int rs, int rt, int rd, int funct, int imm, opcode op);
			double get_clk_T() { return this -> clk_T ; }
			void ghost_cycle();
			sc_cpu(mmem & m) : mm(m), clk_T(0.0000002) {  }
		protected:
			char mem_req_load(int index); // sends a load memory request from CPU to MMEM. The ind is the offset from address 0x0
			void mem_req_write(char data, int index); // sends a write memory request from CPU To MMEM. The ind is the offset from address 0x0
			static const int REG_COUNT = 32;
			double clk_T;
			reg_32 registers[REG_COUNT];
			reg_32 pc;
			mmem & mm;
	};
}
#endif