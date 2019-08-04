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
#include "cpu_time.h"

namespace mips_tools
{
	CPUTime CPUTime::add_time(long picoseconds_in)
	{
		CPUTime cputime = CPUTime(*this);
		unsigned long ps = abs(picoseconds_in);

		if(ps > cputime.picoseconds && picoseconds_in < 0)
		{
			return cputime; // throw exception
		}

		// Check if a new day has been reached
		if(cputime.picoseconds + ps > PICOSECONDS_IN_A_DAY)
		{
			cputime.picoseconds = (cputime.picoseconds + picoseconds_in) - PICOSECONDS_IN_A_DAY;
			cputime.day++;
		}

		// Otherwise just add the picoseconds
		else
		{
			cputime.picoseconds += picoseconds_in;
		}

		return cputime;
	}
}
