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
#include "phdl.h"

namespace priscas
{
	bool RTLBranch::drive()
	{
		
		++this->visit_count; // increase visit count

		// Test if ready. If not ready, simply exit true without doing anything.
		// TODO: make this thread safe
		if(this->ready_requirement != this->visit_count)
		{
			return true;
		}

		// Otherwise, we're ready to execute.
		else
		{
			// Execute.
			this->cycle();

			// After execution, clean up by setting the visit count to 0. Since this is the last instance that should be driving for the cycle
			// this should be OK.
			this->visit_count = 0;

			if(this->get_dependents().size() == 0)
			{
				return true;
			}

			else
			{
				return false;
			}
		}
	}

	void Clock::base_cycle()
	{
		this->cycle();
	}

	void Clock::setInterval(uint64_t interval)
	{
		this->interval = interval;
		
		if(!this->logics.empty())
		{
			for(size_t li = 0; li < this->logics.size(); ++li)
			{
				logics[li]->set_Thresh(interval);
			}
		}
	}

	void Clock::cycle()
	{
		// Prepare each prologue and add each child to the execution engine
		for(size_t no = 0; no < this->logics.size(); ++no)
		{
			this->logics[no]->prologue();

			for(pDrivableList::const_iterator pdl = this->logics[no]->get_dependents().begin(); pdl != this->logics[no]->get_dependents().end(); ++pdl)
			{
				pDrivable p_add = *pdl;
				this->execeng->Register_Work_Request(p_add);
			}

		}
	}
	
	void pHDL_Execution_Engine::Register_Work_Request(mSequentialBlock executable)
	{
		// Just use round robin for now, instead of "smart" scheduling.
		tschedind = (tschedind + 1) % threads.size();

		threads[tschedind]->addWork_ts(mpHDL_Work_Unit(new pHDL_Work_Seq_Unit(executable)));
	}

	void pHDL_Execution_Engine::pHDL_EventHandler::Work()
	{
		while(true)
		{

			// Get work descriptor from queue if not empty
			while(this->wq.empty())
			{
				priscas_osi::sleep(1);	// Otherwise, wait for one
			}
			/*
			wq_lock.lock();
			mpHDL_Work_Unit work = this->wq.front(); wq.pop();
			--loadFac;
			wq_lock.unlock();
		
			// Perform work
			(*work)();*/
		}
	}

	void PHDL_Sequential_Execution_Engine::start()
	{
		this->bready = false;
			
		// For each of the work units, do BFS-style traversal.
		while(!this->dr.empty())
		{

			// Otherwise, execute, and push the children to the end for BFS-style traversal (if not at the end of a path).
			// If I'm not at an end, keep climbing
			if(!dr.front()->drive())
			{
				pDrivableList tl = dr.front()->get_dependents();
				for(pDrivableList::iterator tli = tl.begin(); tli != tl.end(); ++tli)
				{
					dr.push_back(*tli);
				}

			}

			// In either case, this work has been done. Pop it.
			dr.pop_front();
		}

		this->bready = true;
	}
}