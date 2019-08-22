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
#include "cpu_option.h"

namespace priscas
{
	CPU_Option::CPU_Option(const std::string& name, const std::string& desc) :
		nameDesc(name, desc),
		value(0)
	{
	}

	void CPU_Option::add_Value(std::string& strValue, int intValue)
	{
		// First add the value to the string list
		this->strNames.push_back(strValue);
		this->IntToStr.insert(std::pair<int, std::string>(intValue, strValue));

		// Then assign the integer value

		// NOTE: this isn't checked for duplicates!
		strToInt.insert(std::pair<std::string, int>(strValue, intValue));
	}

	void CPU_Option::set_Value(std::string& value)
	{
		if(this->strToInt.count(value) > 0)
		{
			this->value = this->strToInt[value];
		}

		else
		{
			throw mt_invalid_cpu_opt("Value is not valid for this option.");
		}
	}
}