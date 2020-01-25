//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RTL_CPU_COMMON_H__
#define __RTL_CPU_COMMON_H__
#include <list>
#include "phdl.h"

/* Provides some very basic RTLBranch facilities
 *
 */
namespace priscas
{
	// Common gates

	/* PrimitiveAnd
	 * Output: first input & second input
	 *
	 */
	class PrimitiveAnd : public RTLBranch
	{
		public:
			LINK_DE void cycle();

	};

	typedef std::shared_ptr<PrimitiveAnd> mPrimitiveAnd;

	// Adders
	template<int bitcount> class nnaryIntAdder : public RTLBranch
	{
		void cycle()
		{
			mDrivableList dl = this->get_drivers();
			
			BW_generic<bitcount> sum;

			unsigned depth = 0;

			for(size_t pind = 0; pind < this->get_num_inputs(); ++pind)
			{
				sum += this->get_nth_input(pind)->get_Drive_Output().AsInt64();
				++depth;
			}
		}
	};

	typedef nnaryIntAdder<8> nnaryIntAdder8;
	typedef nnaryIntAdder<16> nnaryIntAdder16;
	typedef nnaryIntAdder<32> nnaryIntAdder32;
	typedef nnaryIntAdder<64> nnaryIntAdder64;

	template<int bitcount> class nnaryUIntAdder : public RTLBranch
	{
		void cycle()
		{
			mDrivableList dl = this->get_drivers();
			
			BW_generic<bitcount> sum;

			unsigned depth = 0;

			for(size_t pind = 0; pind != this->get_num_inputs(); ++pind)
			{
				sum += this->get_nth_input(pind)->get_Drive_Output().AsUInt64();
				++depth;
			}
		}

	};

	typedef nnaryUIntAdder<8> BinaryUIntAdder8;
	typedef nnaryUIntAdder<16> BinaryUIntAdder16;
	typedef nnaryUIntAdder<32> BinaryUIntAdder32;
	typedef nnaryUIntAdder<64> BinaryUIntAdder64;

	template<uint64_t incamount> class PrimitiveIntegerIncrementer : public RTLBranch
	{
		public:
			void cycle()
			{
				mDrivableList dl = this->get_drivers();
				
				// Increment the first by 4
				dl.front();

				this->set_Drive_Output(dl.front()->get_Drive_Output().AsUInt64() + incamount);
			}

	};
	
	typedef PrimitiveIntegerIncrementer<1> rtlbIntegerIncPlus1;
	typedef PrimitiveIntegerIncrementer<2> rtlbIntegerIncPlus2;
	typedef PrimitiveIntegerIncrementer<4> rtlbIntegerIncPlus4;
	typedef PrimitiveIntegerIncrementer<8> rtlbIntegerIncPlus8;

	/* Mux_Generic
	 * Requires one select input, and n other inputs
	 *
	 * drivers[0] - is always the select input
	 * drivers[1, ...] - the rest are the data inputs
	 *
	 */
	template<unsigned data_input_count> class Mux_Generic : public RTLBranch
	{
		public:
			void cycle()
			{
				BW_32 addr = this->get_drivers()[0]->get_Drive_Output();
				unsigned selection = addr.AsUInt32() % data_input_count;

				this->set_Drive_Output(this->get_drivers()[selection + 1]->get_Drive_Output());
			}

	};

	/* Basic Single Cycle CPU
	 * RTL "template"
	 *
	 * Since a single cycle CPU is more or less, a single stage pipeline
	 * many can mostly be described in a single RTL branch.
	 *
	 * This specific branch encapsulates a whole Fetch->Decode->Execute sort of sequence
	 * in a single go
	 *
	 * The individual specifics, vary between implementations. fetch(), decode(), and execute() must be reimplemented by derivatives
	 */
	class RTLB_basic_sc : public RTLBranch
	{
		public:
			virtual bool drive();
		protected:
			virtual void fetch() = 0;
			virtual void decode() = 0;
			virtual void execute() = 0;
	};

	/* UniformRegisterFile
	 * A register file with regcount bwclass-bit registers, read_port_count read ports and write_port_count write ports.
	 */
	template<class bwclass, unsigned regcount, unsigned read_port_count, unsigned write_port_count> class UniformRegisterFile : public SequentialBlock
	{
		public:
			
			/* rst
			 * Asynchronous reset.
			 * Reset the state of all contained registers
			 */
			void rst()
			{
				for(unsigned rind = 0; rind < regcount; ++rind)
				{
					regs[rind]->force_current_state(0);
				}
			}

			/* prologue
			 * Based on input buses, drive the read ports.
			 * (Too change the register driving in zero cycles, use zc_address_select)
			 */
			void prologue()
			{
				// Do nothing. 
			}

			/* epilogue
			 * Commit "dirty" registers.
			 */
			void epilogue()
			{
				// For each of the write addresses, write the corresponding data to the registers.
				for(unsigned in = 0; in < write_port_count; ++in)
				{
					const BW& addr = write_address_select[in]->get_Drive_Output();
					const BW& writable = write_ports[in]->get_Drive_Output();
					this->regs[addr.AsUInt32()]->force_current_state(writable);
				}
			}

			/* get_nth_read_addr_port
			 * Return the nth read addr port
			 */
			mNode get_nth_read_addr_port(ptrdiff_t n)
			{
				return read_address_select[n];
			}

			/* get_nth_write_addr_port
			 * Return the nth write addr port
			 */
			mNode get_nth_write_addr_port(ptrdiff_t n)
			{
				return write_address_select[n];
			}

			/* get_nth_read_port
			 * Return the nth read port
			 */
			std::shared_ptr<Mux_Generic<regcount>> get_nth_read_port(ptrdiff_t n)
			{
				return read_ports[n];
			}

			/* get_nth_write_port
			 * Return the nth write_port
			 */
			mNode get_nth_write_port(ptrdiff_t n)
			{
				mNode test = write_ports[n];
				return write_ports[n];
			}

			/* read_reg
			 * Read the nth register's data
			 */
			const BW_default read_reg(unsigned n)
			{
				return **regs[n];
			}

			UniformRegisterFile(Clock& clk)
			{
				// Initialize everything
				for(unsigned c1 = 0; c1 < read_port_count; ++c1)
				{
					read_address_select[c1] = mNode(new Node);
					read_ports[c1] = std::shared_ptr<Mux_Generic<regcount>>(new Mux_Generic<regcount>);
				}
				for(unsigned c2 = 0; c2 < write_port_count; ++c2)
				{
					write_address_select[c2] = mNode(new Node);
					write_ports[c2] = mNode(new Node);
				}

				for(unsigned c3 = 0; c3 < regcount; ++c3)
				{
					regs[c3] = std::shared_ptr<Register_Generic<bwclass>>(new Register_Generic<bwclass>);
					clk.connect(regs[c3]);
				}

				// Connect each mux to the addresser, and each register to the muxes
				for(unsigned t1 = 0; t1 < read_port_count; ++t1)
				{	
					read_ports[t1]->connect_input(read_address_select[t1]);
				
					for(unsigned t11 = 0; t11 < regcount; ++t11)
					{
						read_ports[t1]->connect_input(regs[t11]);
					}
				}
				
				// Connect each write port as children and write address
				for(unsigned t2 = 0; t2 < write_port_count; ++t2)
				{
					this->connect_input(write_ports[t2]);
					this->connect_input(write_address_select[t2]);
				}

			}

		private:
			std::shared_ptr<Register_Generic<bwclass>> regs[regcount];
			unsigned read_mux_indicies[read_port_count];
			unsigned write_mux_indicies[write_port_count];

			// Read and write ports
			mNode read_address_select[read_port_count];
			std::shared_ptr<Mux_Generic<regcount>> read_ports[read_port_count];
			mNode write_address_select[write_port_count];
			mNode write_ports[write_port_count];
	};
}

#endif