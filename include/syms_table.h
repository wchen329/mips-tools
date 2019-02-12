#ifndef __SYMS_TABLE_H__
#define __SYMS_TABLE_H__
#include <vector>
#include "primitives.h"

namespace mips_tools
{
	class syms_table
	{
		public:
			void put(char*, BW_32);
			BW_32 lookup(char *);
		private:
			std::vector<char *> key_list;
			std::vector<BW_32> val_list;
			// Will replace with hash map later
	};
}

#endif