#ifndef __SEQ_CPU_COMMON_H__
#define __SEQ_CPU_COMMON_H__
#include "phdl.h"

/* Provides some very basic SequentialBlock facilities
 *
 */
namespace priscas
{
	/* RegisterFile_ZC_Generic
	 *
	 * A Generic register file with uniform bitwidth per each register.
	 *
	 * This register file takes zero cycles to set the read/write addresses.
	 *
	 * The amount of registers, read and write ports can be specified through additional templates.
	 */
	template<int bitcount, int read_port_count, intwrite_port_count, int register_count> RegisterFile_ZC_Generic
	{
		typedef BW_generic<bitcount> BWusable;

		public:

			/* void rst(...)
			 * Sets every register's value to 0x0
			 */
			void rst()
			{
				for(ptrdiff_t sind = 0; sind < register_count; ++sind)
				{				
					*this->saved_states[sind] = 0;
				}
			}

			RegisterFile_ZC_Generic() { for(ptrdiff_t sind = 0; sind < register_count; ++sind) saved_states[sind] = mBW(new BW_generic<bitcount>); }

		private:
	
			// Note: we aren't using Register_Generic for EVERYTHING here
			// Since the only registers that really need state updates are the ones written to, we only create neough sequential blocks corresponding to writable
			// registers (which corresponds to the amount of write ports)
			Register_Generic<BWusable> writable_reg_layer;

			// Each register's saved state
			mBW saved_states[register_count];
	};

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