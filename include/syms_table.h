#ifndef __SYMS_TABLE_H__
#define __SYMS_TABLE_H__
#include <cstddef>
#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <hash_set>
#include "primitives.h"

namespace mips_tools
{
	/* Wrapper class for the symbols table
	 *
	 */
	class syms_table
	{

		public:
			void insert(std::string key, BW_32 PC);
			std::string& lookup_from_PC(BW_32 PC) { return this->PC_to_syms.at(PC); }
			BW_32 lookup_from_sym(const std::string & sym) { return this->syms_to_PC.at(sym); }
		private:
			std::map<BW_32, std::string> PC_to_syms;
			std::unordered_map<std::string, BW_32> syms_to_PC;
			
	};
}

#endif
