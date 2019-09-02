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
	CPU_Option::CPU_Option(const std::string& name, const std::string& desc, int default_value) :
		nameDesc(name, desc),
		value(default_value)
	{
	}

	void CPU_Option::add_Value(const UPString& strValue, int intValue)
	{
		// First add the value to the string list
		this->strNames.push_back(strValue);
		this->IntToStr.insert(std::pair<int, std::string>(intValue, strValue));

		// Then assign the integer value

		// NOTE: this isn't checked for duplicates!
		strToInt.insert(std::pair<std::string, int>(strValue, intValue));
	}

	void CPU_Option::set_Value(const UPString& value)
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

	void CPU_ControlPanel::add_Control(const CPU_Option& op)
	{
		this->ops.insert(std::pair<const UPString, CPU_Option>(op.getName(), CPU_Option(op)));
	}

	void CPU_ControlPanel::set_ControlValue(const UPString& option_Name, const UPString& option_Value)
	{
		if(this->ops.count(option_Name) > 0)
		{
			ops[option_Name].set_Value(option_Value);
		}

		else
		{
			throw mt_no_such_cpu_option(option_Name);
		}		
	}

	const UPString& CPU_ControlPanel::get_ControlValue(const UPString& option_Name) const
	{
		if(this->ops.count(option_Name) > 0)
		{
			return ops.at(option_Name).getValue();
		}

		else
		{
			throw mt_no_such_cpu_option(option_Name);
		}
	}

	PCPU_OpRawV CPU_ControlPanel::get_ControlValue_Raw(const UPString& option_Name) const
	{
		if(this->ops.count(option_Name) > 0)
		{
			return ops.at(option_Name).getValue_Raw();
		}

		else
		{
			throw mt_no_such_cpu_option(option_Name);
		}
	}

	const CPU_Option_List CPU_ControlPanel::list_Controls() const
	{
		CPU_Option_List status_list;
		status_list.empty();
		Transmute_OptionPair_to_OptionListEntry_Push feachp(status_list);

		std::for_each(ops.begin(), ops.end(), feachp);

		return status_list;
	}
}