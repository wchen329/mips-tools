#include "cpuoption.h"

namespace mips_tools
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