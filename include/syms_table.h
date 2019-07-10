#ifndef __SYMS_TABLE_H__
#define __SYMS_TABLE_H__
#include <cstddef>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include "primitives.h"

namespace mips_tools
{
	/* Wrapper class for the symbols table
	 *
	 */
	class syms_table
	{
		/* There can be many symbols assigned to one PC
		 * However, only one PC can be assigned to a symbol
		 *
		 */

		public:
			void insert(std::string key, unsigned long PC);
			bool has(std::string key) { return this->syms_to_PC.count(key) > 0; };
			bool has(unsigned long pc) { return this->PC_to_syms.count(pc) > 0; }
			std::vector<std::string>& lookup_from_PC(unsigned long PC) { return this->PC_to_syms.at(PC); }
			unsigned long lookup_from_sym(const std::string & sym) { return this->syms_to_PC.at(sym); }
		private:
			std::map<uint32_t, std::vector<std::string>> PC_to_syms;
			std::map<std::string, uint32_t> syms_to_PC;
			
	};
}

#endif
