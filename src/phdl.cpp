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
	void Clock::base_cycle()
	{
		int64_t tleft = this->cyclesleft;

		if(this->cyclesleft == 0)
		{
			// Cycle the actual clock signal
			this->cycle();

			// Reset the timer
			this->cyclesleft = this->interval;
		}

		else
		{
			--this->cyclesleft;
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

		this->execeng->start();
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
			// If not ready, then execute something else first until we are.
			if(!dr.front()->ready())
			{
				dr.push_back(dr.front()); // Put it at the lowest priority.
				dr.pop_front();
			}

			// Otherwise, execute, and push the children to the front (if not at the end of a path).
			else
			{
				// If I'm not at an end, keep climbing
				if(!dr.front()->drive())
				{
					pDrivableList tl = dr.front()->get_dependents();
					for(pDrivableList::iterator tli = tl.begin(); tli != tl.end(); ++tli)
					{
						dr.push_front(*tli);
					}

				}

				// In either case, this work has been done. Pop it.
				dr.pop_front();
			}
		}

		this->bready = true;
	}
}