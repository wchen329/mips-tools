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