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
//   You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CPU_TIME_H__
#define __CPU_TIME_H__
#define PICOSECONDS_IN_A_DAY 86400000000000000
#define TEN_TO_TWELVE 1000000000000
#include <cmath>

namespace mips_tools
{
	// CPUTime
	// stores time in two separate fields
	// days and picoseconds!
	class CPUTime
	{
		unsigned long long day;
		unsigned long long picoseconds;

		CPUTime add_time(long picoseconds);

		public:
			unsigned long long getDays() { return this->day; }
			unsigned long long getPS() { return this->picoseconds; }
			CPUTime() : day(0), picoseconds(0) {}
			CPUTime operator+(long picoseconds) { return this->add_time(picoseconds) ; }
			void operator+=(long picoseconds) { CPUTime tmp = this->add_time(picoseconds) ; this->day = tmp.getDays(); this->picoseconds = tmp.getPS(); }
	};
}

#endif
