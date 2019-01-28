#ifndef __MMEM_H__
#define __MMEM_H__
#include "mem.h"

namespace mips_tools
{
	// Main memory
	class mmem : public mem
	{
		public:
			mmem(int size = 1024) : mem(size){}
	};

}

#endif