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
}

#endif