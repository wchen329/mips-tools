#ifndef __SEQ_CPU_COMMON_H__
#define __SEQ_CPU_COMMON_H__
#include "phdl.h"

/* Provides some very basic SequentialBlock facilities
 *
 */
namespace priscas
{
	/* RegisterFile_Generic
	 * A Generic register file with uniform bitwidth per each register. Single cycle read.
	 *
	 * The amount of registers, read and write ports can be specified through additional templates.
	 */
	/*template<class BitST, int read_port_count, intwrite_port_count, int register_count> RegisterFile_Generic : public SequentialBlock
	{
		typedef Register_Generic<BitST> RegisterT;
		typedef Bus_Generic<BitST> BusT;

		public:
			*/
			/* cycle()
			 * Advance the register file a cycle.
			 */
			//void cycle();

			/* These are atomic operations
			 * They can be used in debugging features or for
			 * CPUs which assumes zero cycle data read from the RF
			 */
/*			void direct_set_read_address(ptrdiff_t addr) { read_addr = addr; }
			void direct_set_write_address(ptrdiff_t addr) { write_addr = addr; }

		private:
			RegisterT registers[register_Count];
			BusT read_ports[read_port_count];
			BusT write_ports[write_port_count];
			ptrdiff_t write_addr;
			ptrdiff_t read_addr;
			
	}*/
}

#endif