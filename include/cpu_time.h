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