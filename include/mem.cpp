#include "mem.h"

namespace mips_tools
{
	mem::mem(size_t size)
	{
		this -> data = new char[size];
		this -> size = size;
	}

	mem::mem(const mem & m)
	{
		// disabled, see interface declaration, may enable later
	}
	
	mem mem::operator=(const mem & m)
	{
		// disabled, see interface declaration, may enable later
		return m;
	}

	char mem::operator[](int ind)
	{
		return data[ind];
	}

	mem::~mem()
	{
		delete this->data;
	}
}