#include "mem.h"

namespace mips_tools
{
	mem::mem(int addr_size)
	{
		this -> data = new char[addr_size];
	}

	mem::mem(const mem & m)
	{
		// to finish
	}
	
	mem mem::operator=(const mem & m)
	{
		// to finish
		return m;
	}

	mem::~mem()
	{
		delete this->data;
	}
}