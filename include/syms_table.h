#ifndef __SYMS_TABLE_H__
#define __SYMS_TABLE_H__
#include <cstddef>
#include <cstring>
#include <map>
#include <vector>
#include "primitives.h"

namespace mips_tools
{
	/* Wrapper class for the symbols table
	 *
	 */
	class syms_table
	{
		public:
			void put(std::string symbol_name, BW_32 PC) { this->syms.insert(std::pair<BW_32, std::string>(PC, symbol_name)); }
		private:
			std::map<BW_32, std::string> syms;
	};
}

#endif
