#ifndef __MB_H__
#define __MB_H__
#include "cpu.h"
#include "sc_cpu.h"
#include "mmem.h"

namespace mips_tools
{
	enum cpu_t
	{
			STANDARD = 0,
			FIVE_P = 1,
			FIVE_P_SS = 2
	};

	/* Motherboard
	 *
	 * Representation of a computer with a single CPU and a single main memory.
	 *
	 */
	class mb
	{
		public:
			void dc_on();	// power on
			void reset();
			void step();	// step the processor a cycle
			cpu & get_cpu();		// return cpu reference
			size_t get_mmem_size();	// get main memory size in BYTES

			mb(cpu_t ct = STANDARD, int mt = 16);
			~mb();

		private:
			cpu * mb_cpu;	// single socket cpu
			mmem * mb_mmem;  // single main memory
			cpu_t cpu_type;	// types of MIPS processor
			int mmem_type;	// bit length of address space
			
	};
}
#endif