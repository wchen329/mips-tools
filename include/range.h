#ifndef __RANGE_H__
#define __RANGE_H__
#include <list>
#include <cstdio>
#include <string>
#include <vector>
#include "mt_exception.h"

namespace mips_tools
{
	typedef std::list<size_t>::iterator range_iterator;

	/* A generic, iterable "range"
	 * Really just an ordered collection of numbers which can be iterated over
	 */
	class range
	{
		std::list<size_t> numbers;

		// Constructs a range using a string specified
		// Ranges are constructed using MATLAB syntax, that is:
		// begin (inclusive):end (inclusive):step
		// i.e. 0:4:2 would look like 0, 2 (4 is out of range)
		// the step can be omitted which then would look like 0:4 (0, 1, 2, 3)
		public:
			range(std::string&);
			range_iterator begin() { return this->numbers.begin(); }
			range_iterator end() { return this->numbers.end(); }
	};
}

#endif