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
#ifndef __PHDL_H__
#define __PHDL_H__

/* "Pseudo HDL"
 * This refers to the parallelizable backend portion of the
 * simulator which attempts to mirror the operation of real hardware
 * similar to a Hardware Description Language (HDL).
 * Note that: z and x values are not emulated (i.e. from Verilog)
 */
#include "primitives.h"
#include "priscas_global.h"

namespace priscas
{
	/* Bus_Generic
	 * A generic n bit bus.
	 */
	template<class ContainerT> class Bus_Generic
	{
		public:
			/* drive(...)
			 * Drive the bus with some data.
			 * (atomic)
			 */
			void drive(ContainerT input) { data = input; };

			/* read(...)
			 * Read data off the bus
			 * (atomic)
			 */
			ContainerT read() { return data; };

		private:
			ContainerT data;
	};

	typedef Bus_Generic<BW_32> Bus_32;
	typedef Bus_Generic<BW_16> Bus_16;

	/* SequentialBlock
	 * Represents block of sequential logic
	 * that holds state in it
	 *
	 * A sequential block is aimed to have a single clock signal.
	 * If multiple signals are needed, break them into separate clock signals.
	 */
	class SequentialBlock
	{

		public:
			/* cycle()
			 * IMPLEMENTATION: perform operations during which would otherwise happen during a cycle.
			 */
			virtual void cycle() = 0;

	};

	typedef std::shared_ptr<SequentialBlock> mSequentialBlock;

	/* Clock
	 * A clock signal which is used to power Sequential Blocks
	 */
	class Clock
	{
		public:

			/* base_cycle()
			 * Advance time one base clock cycle for this clock.
			 */
			void base_cycle();

			/* cycle()
			 * Cycle all connected sequential blocks.
			 * Unless there is a good reason to, use base_cycle instead (which works off of the base clock)
			 */
			void cycle();

			/* connect(...)
			 * Connect a sequential logic block. This will be
			 * controlled by the clock accordingly and will be cycled when "cycle" is called
			 */
			void connect(mSequentialBlock logic)  { this->logics.push_back(logic); }

			/* set_Interval(...)
			 * Sets how many "base clock cycles" to wait before actually "cycling"
			 * It can be also trivially seen that the base clock has an interval of 0.
			 * For example, Clock with interval of 1 will actually activate every 2 base clock cycles.
			 */
			void setInterval(int64_t interval) { this->interval = interval; }
			
			/* set_Offset(...)
			 * Sets clock skew.
			 * The offset is the "head start" of the clock (or how much it is delayed when first inserted).
			 * - A negative offset means that the clock is delayed by the absolute value of that base clock cycles
			 * - A postive offset means that the clock is ahead of the base clock by the offset in base clock cycles. However, the reset signal is assumed to be asserted
			 *   at the beginning, so any state changes which occur before the first cycle of the base clock will not take effect
			 */

		private:
			typedef std::vector<mSequentialBlock> SeqmBlkV;
			SeqmBlkV logics;
			uint64_t interval;
			int64_t cyclesleft;
	};

	/* Register_Generic
	 * A generic n bit register
	 * (this will supersede the Reg_32)
	 */
	template<class RegCC> class Register_Generic : protected SequentialBlock
	{
		public:
		
			/* RegCC get_current_state()
			 * Return the current state
			 * (atomic)
			 */
			RegCC get_current_state() { return current_state; }

			/* force_current_state(...)
			 * Atomically set the current state.
			 * Unless this is really intended, use set_next_state(...) instead
			 * So it is updated with the clock accordingly
			 * (atomic)
			 */
			void force_current_state(RegCC data_in) { this->current_state = data_in; }

			/* set_next_state(...)
			 * Set the next state.
			 * The next state becomes the current state on the next clock edge.
			 * (NOT atomic)
			 */
			void set_next_state(RegCC data_in) { this->next_state = data_in; }

			/* operator<=
			 * Short hand for set_next_state(...).
			 */
			void operator<=(RegCC data_in) { set_next_state(data_in); }

			/* RegCC operator*
			 * Short hand for get_current_state()
			 */
			RegCC operator*() { return get_current_state(); }

			/* cycle()
			 * Set the current state to next state.
			 */
			void cycle()
			{
				this->current_state = this->next_state;
			}

		private:

			// Register's State
			RegCC current_state;

			// Register State on Next Update
			RegCC next_state;
	};

	typedef Register_Generic<BW_32> Register_32;
	typedef Register_Generic<BW_16> Register_16;
}

#endif