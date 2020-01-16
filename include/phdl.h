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
 * Note that: HiZ and unknown (x) values are not emulated
 *
 * Ultimately, this is supposed to be a parallel graph traversal
 * The graph "we'll call this "RTL" graph is simple a Directed Acyclic Graph
 * with entry points being sequential blocks
 * intermediate nodes being the buses, connections, and combinational blocks in-between,
 * and sequential blocks (or just the an unconnected bus the stop nodes)
 */
#include <algorithm>
#include <deque>
#include <queue>
#include "primitives.h"
#include "priscas_global.h"
#include "priscas_osi.h"

namespace priscas
{
	// Forward declaration
	class Clock;

	class Node;
	typedef std::shared_ptr<Node> mNode;
	
	class Drivable;
	typedef Drivable* pDrivable;
	typedef std::shared_ptr<Drivable> mDrivable;
	typedef std::deque<Drivable> DrivableList;
	typedef std::deque<mDrivable> mDrivableList;
	typedef std::deque<pDrivable> pDrivableList;
	typedef uint16_t eventcode_t;

	class PHDL_Execution_Engine;
	typedef std::shared_ptr<PHDL_Execution_Engine> mPHDL_Execution_Engine; 

	/* Drivable
	 * Elements which derive this within may be driven by a bus connection (or Node).
	 * The Drivable also has a single default output bus. Driving the default bus and feeding another Drivable through the default
	 * bus will in turn use "implicit connection" and feed the output of the source branch directly as an input to the output branch.
	 *
	 * If, for verbosity, or more than one output bus is needed, use the Node (Bus) class to define more output buses.
	 */
	class Drivable
	{
		public:

			/* drive(...)
			 * Drive this drivable with a voltage signal from its inputs (in the form of bits)
			 * Return true if end of a chain (i.e. sequential logics)
			 * Return false if not end of a chain.
			 */
			virtual bool drive() = 0;

			/* connect_input(...)
			 * Connect new input to this drivable.
			 * This automatically links the other way as well, adding the argument as a dependent node
			 */
			void connect_input(mDrivable inp) { drivers.push_back(inp.get()); inp->drivees.push_back(this);}

			/* connect_input(...)
			 * Connect new input to this drivable.
			 * This automatically links the other way as well, adding the argument as a dependent node
			 */
			void connect_input(pDrivable inp) { drivers.push_back(inp); inp->drivees.push_back(this);}

			/* event_notify(...)
			 * Notify a block that an event has occurred.
			 * Predefined:
			 * 0 - RTLBranches this is going to be "input ready"
			 * 1 - For registers, this is going to be "ignore structual links (i.e. do not read input except for those specified by <=)"
			 */
			virtual void event_notify(eventcode_t event_code) {};

			 /* get_dependents
			  * Get the elements which are driven by this drivable
			  */
			const pDrivableList& get_dependents() { return this->drivees;}

			/* get_Drive_Output
			 * Get the input of this block which resulted from driving this block.
			 */
			const BW& get_Drive_Output() const { return this->drive_output; for(size_t oind = 0; oind < drivers.size(); ++oind) drivers[oind]->event_notify(0); }

			Drivable() : drive_output(0) {}

			enum EventCodes
			{
				EventCode_InputReady = 0,
				EventCode_IgnoreRegisterStructualLink = 1
			};

		protected:

			/* get_drivers
			 * Get the elements which are driving this drivable
			 * (This is protected, rather than public since this is technically representing a DAG. This is only intended to be used to receive inputs as necessary)
			 *
			 * TO BE REMOVED: this is no longer necessary
			 */
			virtual pDrivableList get_drivers() { return this->drivers; }

			/* get_num_inputs
			 * Return the amount of inputs this circuit element has
			 */
			size_t get_num_inputs() { return this->drivers.size(); }

			/* get_nth_input
			 * Returns the nth input if available (checked)
			 */
			const pDrivable get_nth_input(size_t n) { return this->drivers.at(n); }

			/* set_Drive_Output
			 * Set the output of driving this block which will be visible to its children.
			 */
			void set_Drive_Output(const BW& output)
			{
				this->drive_output = output.AsInt32();
			}

		private:
			pDrivableList drivers;
			pDrivableList drivees;
			BW_32 drive_output;
	};

	/* RTLBranch
	 * A RTL branch is supposed to represent dataflow (RTL-ish level) throughout the processor. It is also intended to handle behavioral constructions.
	 * Mainly, it's supposed to be combinational blocks which set the states of sequential logics.
	 * RTL branches are only to be tied to the base clock (which incidentally is also the fastest)
	 *
	 * RTL branches will not execute (cycle) until all parent buses are done being driven.
	 * The 
	 */
	class RTLBranch : public Drivable
	{
			public:

				/* cycle()
				 * Take the inputs and evaluate the node.
				 */
				virtual void cycle() = 0;

				/* drive()
				 * If the RTLBranch isn't ready, this should short circuit by returning true.
				 * If it is ready, then this should "cycle" which depends on implementation.
				 * Upon leaving, the visit_count should reset to zero, 
				 *     If there are no children, then this should return true
				 *     If there are children, return false
				 * drive somehow executes early or gets stuck, then this probably means there is a combination loop (error) or an extra bus is driving this.
				 */
				LINK_DE bool drive();

				/* virtual void event_notify(...)
				 * Register event. For RTLBranches, 0 = input ready
				 */
				LINK_DE void event_notify(eventcode_t event_code);

				RTLBranch() : visit_count(0) {}

			private:
				unsigned visit_count;
				priscas_osi::mlock synch;
	};

	typedef std::shared_ptr<RTLBranch> mRTLBranch;

	/* SequentialBlock
	 * Represents block of sequential logic
	 * that holds state in it
	 *
	 * A sequential block is aimed to have a single clock signal.
	 * If multiple signals are needed, break them into separate blocks.
	 */
	class SequentialBlock : public Drivable
	{

		public:

			friend class Clock;
			
			/* drive()
			 * Execute epilogue when clock is met. Then return true.
			 */
			virtual bool drive()
			{
				if(timeout == 0)
				{
					epilogue();
					timeout = thresh;
				}

				else
				{
					--timeout;
				}

				return true;
			}

			/* prologue()
			 * IMPLEMENTATION: perform operations during which would otherwise happen during a cycle.
			 */
			virtual void prologue() = 0;

			/* epilogue()
			 * IMPLEMENTATION: perform operations which occur between cycles (i.e. assign queue STATE CHANGES)
			 */
			virtual void epilogue() = 0;

			SequentialBlock() : thresh(0), timeout(0) {}

		protected:
			/* set_Thresh(...)
			 * Sets the amount of base clocks required to wait until drive executes the epilogue.
			 */
			void set_Thresh(uint64_t offset) { this->thresh = offset; this->timeout = offset; }

		private:
			uint64_t thresh;
			uint64_t timeout; // the amount of time left until this clock can fire.
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
			 * (alias to cycle)
			 */
			LINK_DE void base_cycle();

			/* cycle()
			 * Cycle all connected sequential blocks.
			 */
			LINK_DE virtual void cycle();

			/* connect(...)
			 * Connect a sequential logic block. This will be
			 * controlled by the clock accordingly and will be cycled when "cycle" is called
			 */
			void connect(mSequentialBlock logic)  { this->logics.push_back(logic); logic->set_Thresh(this->interval); }

			/* set_Interval(...)
			 * Sets how many "base clock cycles" to wait before actually "cycling"
			 * It can be also trivially seen that the base clock has an interval of 0.
			 * For example, Clock with interval of 1 will actually activate every 2 base clock cycles.
			 */
			void setInterval(uint64_t interval);
			
			/* set_Offset(...)
			 * Sets clock skew.
			 * NOT YET IMPLEMENTED. DO NOT USE.
			 * The offset is the "head start" of the clock (or how much it is delayed when first inserted).
			 * - A negative offset means that the clock is delayed by the absolute value of that base clock cycles
			 * - A postive offset means that the clock is ahead of the base clock by the offset in base clock cycles. However, the reset signal is assumed to be asserted
			 *   at the beginning, so any state changes which occur before the first cycle of the base clock will not take effect
			 */
			virtual void setOffset(int64_t cyclesleft) { this->cyclesleft = cyclesleft; }

			virtual void setExecutionEngine(mPHDL_Execution_Engine execin) { this->execeng = execin; }

			/* Constructor.
			 * Defaults to a clock matching the base clock.
			 */
			Clock() :
				interval(0),
				cyclesleft(0)
			{}

			/* Constructor.
			 * Takes an interval and offset directly. 
			 */
			Clock(int64_t interval, int64_t offset) :
				interval(interval),
				cyclesleft(offset)
			{}

		private:
			typedef std::vector<mSequentialBlock> SeqmBlkV;
			SeqmBlkV logics;
			int64_t interval;
			int64_t cyclesleft;
			mPHDL_Execution_Engine execeng;
	};

	typedef std::vector<Clock> Clock_Vec;

	/* Register_Generic
	 * A generic n bit register
	 * (this will supersede the Reg_32)
	 */
	template<class RegCC> class Register_Generic : public SequentialBlock
	{
		public:
		
			/* RegCC get_current_state()
			 * Return the current state
			 * (atomic)
			 */
			RegCC& get_current_state() { return current_state; }
			const RegCC& get_current_state() const { return current_state; }

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
			void set_next_state(RegCC data_in) { this->next_state = data_in; dirty = true; }

			/* operator<=
			 * Short hand for set_next_state(...).
			 */
			void operator<=(RegCC data_in) { set_next_state(data_in); }

			/* RegCC operator*
			 * Short hand for get_current_state()
			 */
			RegCC operator*() { return get_current_state(); }

			/* prologue()
			 * Start the execution chain.
			 */
			void prologue()
			{
				// Prologue and Content
				this->set_Drive_Output(this->get_current_state());
			}

			/* epilogue()
			 * Commit state changes queued during the cycle.
			 */
			void epilogue()
			{
				if(this->get_drivers().size() > 0)
				{
					pDrivable dout = this->get_drivers().front();
					const BW& input = dout->get_Drive_Output();
					this->next_state.AsInt32() = input.AsInt32();
					this->current_state.AsInt32() = input.AsInt32();
					return;
				}

				// Epilogue: if "dirty" then update.
				// If not, don't do anything
				// having this allows for implicit feedback setting the state

				if(this->dirty)
				{
					this->current_state = this->next_state;
				}

				dirty = false;
			}

			Register_Generic() : dirty(false) {}

		private:

			// Register's State
			RegCC current_state;

			// Register State on Next Update
			RegCC next_state;

			// Input Connection
			mDrivable input_connection;

			// Output Connection
			mDrivable output_connection;

			// Dirty. Does this register have a queued update?
			bool dirty;
	};

	typedef Register_Generic<BW_32> Register_32;
	typedef Register_Generic<BW_16> Register_16;
	typedef std::shared_ptr<Register_32> mRegister_32;
	typedef std::shared_ptr<Register_16> mRegister_16;

	/* Bus
	 * A general bus.
	 */
	class Bus : public Drivable
	{
		public:
			/* drive(...)
			 * Drive the bus with some data.
			 * (atomic)
			 */
			virtual bool drive() = 0;

			/* read(...)
			 * Read data off the bus
			 * (atomic)
			 */
			virtual const BW& read() = 0;
	};

	/* Generic_Mux
	 * (Multiplexor)
	 *
	 * This represents a m:1 mux with storage-class bitwide inputs and outputs.
	 * 
	 * The mux also acts as a synchronization barrier.
	 * The last thread to set an input inherits the execution of the output bus
	 *
	 * If a mux input is not properly fed, deadlock will occur.
	 *
	 * Other threads reaching this point end their job.
	 *
	 */
/*	template<int m, class storage_class> class Generic_Mux : public Execution_Synchronized
	{
		public:
			Generic_Mux() :
			  Execution_Synchronized(m + 1) // m + 1 since there are the n data inputs, but also the select input
			{}

		private:
			mBus input_buses[m];
			mBus output_bus;
			mBus select_bus;
	};
	
	typedef Generic_Mux<2, BW_32> Mux_2_1_32w;
	typedef Generic_Mux<4, BW_32> Mux_4_1_32w;*/

	typedef std::vector<mDrivable> Drivable_Vec;

	/* Node
	 * (Bus connection)
	 * This connects a bus to other buses, sequential logics, or muxes.
	 *
	 *
	 * It represents a node in a logical circuit
	 *
	 * Be aware, that the correctness of this still assumes
	 * that only one bus is driving a node/connection at once
	 *
	 * Nodes are particularly useful when more than one bus is needed (i.e. can't drive a branch's output using the default bus)
	 *
	 */
	class Node : public Drivable
	{
		public:


			/* Drive the connection with data.
			 * If this bus did not have an explicit charge, then it will implicitly assume the
			 * first block feeding this bus's default bus has the value to pass through.
			 *
			 * Otherwise, the explicit charge is used.
			 *
			 * This drives all connected devices' with the provided data accordingly.
			 */
			bool drive()
			{
				pDrivableList drl = this->get_drivers(); // todo use references

				if(this->charge_data.get() == nullptr)
				{
					this->set_Drive_Output(this->get_drivers().front()->get_Drive_Output());
				}

				else
				{
					this->set_Drive_Output(*this->charge_data);
				}

				if(this->get_dependents().size() == 0)
				{
					return true;
				}

				else
				{
				return false;
				}
			}

			
			/* explicit_charge(...)
			 *
			 */
			void explicit_charge(const BW& charge_data)
			{
				if(this->charge_data.get() == nullptr)
				{
					this->charge_data = mBW(new BW_default());
				}
			}

		private:

			mBW charge_data;
	};

	/* PHDL Execution Engine
	 * The execution engine is responsible for evaluating operations performed during a cycle.
	 * generally its execution pattern will look like this: 
	 *
	 * - Start the firing chain by executing the prologue of all the sequential logics (this is done by the clock)
	 * - Mark all the dependents for executing
	 * - Execute the dependents, and climb the chain of dependents until
	 *   - A path divergence is encountered. In that case, one path will be traversed, while the other, will be pushed on some data structure for executing later.
	 *   - Another block of sequential logic is encountered and executed. In that case, the path has ended and we should continue executing another path or execute a new one until
	 *     we are finished.
	 */
	class PHDL_Execution_Engine
	{
		public:
			 /* Add a work unit to the execution engine.
			  * 
			  */
			LINK_DE virtual void Register_Work_Request(pDrivable executable) = 0;

			/* ready()
			 * Returns true if all execution paths required have been finished executing.
			 * Otherwise returns false.
			 */
			LINK_DE virtual bool ready() = 0;

			/* start()
			 * Begin the execution chain.
			 */
			LINK_DE virtual void start() = 0;
	};
	 
	/* PHDL Sequential Execution Engine
	 * This execution engine traverses the graph in a single thread.
	 * (somewhat slow, but emphasizing correctness)
	 *
	 */
	class PHDL_Sequential_Execution_Engine : public PHDL_Execution_Engine
	{
		public:
			 /* Add a work unit to the execution engine.
			  * 
			  */
			LINK_DE void Register_Work_Request(pDrivable executable)
			{
				this->dr.push_back(executable);
			}

			/* ready()
			 * Returns true if all execution paths required have been finished executing.
			 * Otherwise returns false.
			 * For the sequential engine this just mean not ready when enter, ready when exit
			 */
			LINK_DE bool ready() { return this->bready; }

			/* start()
			 * Start traversing the chain.
			 */
			LINK_DE void start();

			LINK_DE PHDL_Sequential_Execution_Engine() : bready(true) {}

		private:
			bool bready;
			pDrivableList dr;
	};

	/* Thread Level Parallel Execution Engine.
	 * The execution engine attempts to parallelize the work occurring within
	 * one clock cycle.
	 *
	 * This is currently quite an (experimental) naive implementation and is not really that optimized (might be slower than Sequential)
	 *
	 */
	class PHDL_TLP_Execution_Engine : public PHDL_Execution_Engine
	{

		public:

			class PHDL_Work_Unit
			{
				public:
					/* Execute the work unit which is a sequential block chain in this case
					 */
					virtual void operator()() {this->work->drive(); }

					/* Construct.
					 */
					PHDL_Work_Unit(pDrivable w) : work(w) {}

				private:
					pDrivable work;
			};

			/* LoadFactor
			 * The amount of load this thread is experiencing.
			 * The higher it is, the more load.
			 * The lower it is, the less load.
			 */
			typedef size_t LoadFactor;

			typedef std::queue<PHDL_Work_Unit> PHDLTLPWorkQueue;

			/* pHDL_EventHandler
			 * A special thread which checks the 
			 *
			 */
			class pHDL_EventHandler : public priscas_osi::UPThread
			{
				public:
					void Work();

					/* Add a work unit to this thread's local work queue.
					 * Adjusts the load factor of the associated thread accordingly. 
					 * (thread-safe)
					 */
					void addWork_ts(PHDL_Work_Unit work) { wq_lock.lock(); wq.push(work); ++loadFac; wq_lock.unlock();}

					/* The "smallest" pHDL_EventHandler is going to be the
					 * one with the least load.
					 *
					 * Helps with load balancing.
					 *
					 */
					bool operator<(const pHDL_EventHandler& eh_2)
					{
						wq_lock.lock();
						return this->loadFac < eh_2.loadFac;
						wq_lock.unlock();
					}

					pHDL_EventHandler() :
						loadFac(0)
					{
						UPThread::ActivateThread(this);
					}

				private:
					PHDLTLPWorkQueue wq;
					priscas_osi::mlock wq_lock;
					LoadFactor loadFac;
			};
			typedef std::shared_ptr<pHDL_EventHandler> mpHDL_EventHandler;

			/* Adds some work to the least occupied thread
			 */
			void Register_Work_Request(pDrivable executable);
								
			// TODO
			bool ready() { return true; }
			
			typedef std::vector<mpHDL_EventHandler> UPThreadPool;

			PHDL_TLP_Execution_Engine(size_t threadcount) :
				tschedind(0)
			{
				threads.resize(threadcount);
				
				for(size_t tind = 0; tind < threads.size(); ++tind)
				{					
					threads[tind] = mpHDL_EventHandler(new pHDL_EventHandler());
					threads[tind]->Execute();
				}
			}

			void start()
			{
				// todo
			}

		private:
			size_t tschedind;
			UPThreadPool threads;
			
	};

	/* Other relavent typedefs
	 * (most corresponding to STL typedefing)
	 */

	/* Vector of Clock
	 */
	typedef std::vector<Clock> Clock_Vec;
}

#endif