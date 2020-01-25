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
	typedef PrimitiveIntegerIncrementer<4> rtlbIntegerIncPlus2;
	typedef PrimitiveIntegerIncrementer<4> rtlbIntegerIncPlus4;
	typedef PrimitiveIntegerIncrementer<4> rtlbIntegerIncPlus8;

	/* Mux_Generic
	 * Requires one select input, and n other inputs
	 */
	template<unsigned register_count> class Mux_Generic : public RTLBranch
	{
		public:
			void cycle()
			{
				BW_32 addr = this->get_drivers()[0]->get_Drive_Output();
				unsigned selection = addr.AsUInt32() % register_count;

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
			/* prologue
			 * Based on input buses, drive the read ports.
			 * (Too change the register driving in zero cycles, use zc_address_select)
			 */
			void prologue()
			{
				
			}

			/* epilogue
			 * Commit "dirty" registers.
			 */
			void epilogue()
			{
				while(!this->commit_list.empty())
				{
					// Commit the dirty registers
					commit_list.front()->epilogue();

					// Remove from commit log
					commit_list.pop_front();
				}
			}

			/* zc_read_address_select(BW_class addr, unsigned read_port_no)
			 * Select the register corresponding to the
			 * address passed in for the read_port_no'th read port, in zero cycles
			 */
			void zc_read_address_select(const bwclass& in, unsigned read_port_no)
			{
				// Set the read port index
				this->read_mux_indicies[read_port_no] = in.AsUInt32();

				// Perform a read
			}

			/* zc_write_address_select(BW_class addr, unsigned write_port_no)
			 * Select the register corresponding to the write_port_no'th port, in zero cycles
			 */
			void zc_write_address_select(const bwclass& in, unsigned write_port_no)
			{

			}

			/* drive_write_data
			 * Drive the nth write port with given data
			 */
			void drive_write_data(const bwclass& in, unsigned n)
			{
				this->regs[write_mux_indicies[n]] <= in;
				commit_list.push_back(&in);
			}

			/* read_reg
			 * Read the nth register's data
			 */
			const BW& read_reg(unsigned n)
			{
				return *regs[n];
			}

			UniformRegisterFile() :
				read_ports(new Node[read_port_count]),
				write_ports(new Node[write_port_count])
			{
				for(unsigned rpi = 0; rpi < read_port_count; ++rpi)
				{
					read_ports[rpi]->connect_input(this);
				}

			}

		private:
			Register_Generic<bwclass> regs[regcount];
			unsigned address;
			unsigned read_mux_indicies[read_port_count];
			unsigned write_mux_indicies[write_port_count];
			std::list<Register_Generic<bwclass>*> commit_list;

			// Read and write ports
			mNode read_ports[read_port_count];
			mNode write_ports[write_port_count];
	};
}

#endif