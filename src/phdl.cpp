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
		// Change this to make it parallelizable, potentially
		for(size_t no = 0; no < this->logics.size(); ++no)
		{
			this->logics[no]->cycle();
		}
	}

	void BaseClock::cycle()
	{
		/* First evaluate RTLish
		 * statements
		 */

		// Change this to make it parallelizable, potentially
		for(size_t no = 0; no < this->logics.size(); ++no)
		{
			this->logics[no]->cycle();
		}

		/* Second evaluate sequential logic
		 * statements
		 */
		Clock::cycle();
	}
}