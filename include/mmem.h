#ifndef __MMEM_H__
#define __MMEM_H__
#include "mem.h"

namespace mips_tools
{
	// Main memory
	class mmem : public mem
	{
		public:
			mmem(size_t size = 1024) : mem(size){}
			char& operator[](int ind);
		private:
			mmem operator=(const mem &);		// copy assignment, disabled
			mmem(const mem &);					// copy constructor, disabled
	};

}

#endif