#include "mmem.h"

namespace mips_tools
{

	char& mmem::operator[](int ind)
	{
		int real_ind = ind % this->get_size();
		return mem::operator[](real_ind);
	}
}