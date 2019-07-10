#include <exception>
#include "parser_err.h"
#include "mt_exception.h"
#include "syms_table.h"

namespace mips_tools
{
	void syms_table::insert(std::string key, unsigned long PC)
	{

		// That symbol better not be assigned already...
		bool already_def = this->syms_to_PC.count(key) > 0;
			
		if(already_def)
			throw mipsshell::parser_err();

		// Allows lookup from PC to symbol

		// case: not yet allocated, create a new entry
		if(!this->has(PC))
		{
			std::pair<unsigned long, std::vector<std::string>> in;
			in.first = PC;
			in.second = std::vector<std::string>();
			in.second.push_back(key);
			this->PC_to_syms.insert(in);
		}


		// other case: PC entry already there, just create a new entry
		else
		{
			// Allows lookup from PC to symbol
			this->PC_to_syms.at(PC).push_back(key);
		}

		// Allows lookup from symbol to PC
		std::pair<std::string, unsigned long> in_2;
		in_2.first = key;
		in_2.second = PC;
		this->syms_to_PC.insert(in_2);

	}
}