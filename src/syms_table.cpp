#include <exception>
#include "mt_exception.h"
#include "syms_table.h"

namespace mips_tools
{
	void syms_table::insert(std::string key, BW_32 PC)
	{

		// Allows lookup from PC to symbol
		std::pair<BW_32, std::string> in;
		in.first = PC;
		in.second = key;
		this->PC_to_syms.insert(in);

		// Allows lookup from symbol to PC
		std::pair<std::string, BW_32> in_2;
		in_2.first = key;
		in_2.second = PC;
		this->syms_to_PC.insert(in_2);
	}
}