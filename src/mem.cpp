//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#include "mem.h"

namespace priscas
{
	mem::mem(size_t size)
	{
		this -> data = new byte_8b[size];
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

	byte_8b& mem::operator[](int ind)
	{

		return data[ind];
	}

	mem::~mem()
	{
		delete this->data;
	}
}
