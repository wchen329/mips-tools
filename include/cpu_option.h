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
#ifndef __CPUOPTION_H__
#define __CPUOPTION_H__
#include <map>
#include <string>
#include <vector>
#include "mt_exception.h"
#include "primitives.h"
#include "priscas_global.h"

namespace priscas
{
	/* Represents a CPU option
	 * (i.e. forcing a certain control signal or the like)
	 * wchen329
	 */
	class CPU_Option
	{
		public:
			CPU_Option(const std::string& name, const std::string& description, int default_value = 0);
			std::string& getName() { return nameDesc.getName(); }
			std::string& getDescription() { return nameDesc.getDescription(); }
			std::vector<std::string>& get_SValues() { return this->strNames; }	// get possible string values of the option
			void add_Value(std::string& strValue, int intValue);	// add a possible value to cpu options
			void set_Value(std::string& strValue);	// pass a string value which will set the internal int value accordingly
			int get_IntValue() { return this->value;} // get the current int value
			std::string& getValueName() { return this->IntToStr[value]; }

		private:
			std::vector<std::string> strNames;
			std::map<std::string, int> strToInt;
			std::map<int, std::string> IntToStr;
			NameDescPair nameDesc;	// name and description of the CPU options
			int value;				// Current value of CPU option (internal value)
	};
}

#endif