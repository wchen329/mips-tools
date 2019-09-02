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
#include "mmem.h"

namespace priscas
{
	byte_8b& mmem::operator[](ptrdiff_t ind)
	{
		return data[ind];
	}

	const byte_8b& mmem::operator[](ptrdiff_t ind) const
	{
		return data[ind];
	}

	mmem::mmem(size_t size)
	{

		this -> data = new byte_8b[size];
		this -> size = size;
	}

	mmem::mmem(const mmem & m)
	{
		// disabled, see interface declaration, may enable later
	}
	
	mmem mmem::operator=(const mmem & m)
	{
		// disabled, see interface declaration, may enable later
		return m;
	}

	mmem::~mmem()
	{
		delete this->data;
	}

	void mmem::save(FILE* f)
	{
		// Save size information
		fwrite(&(this->size), sizeof(size_t), 1, f);

		// Dump the rest of the memory array
		fwrite(this->data, sizeof(byte_8b), this->size, f);
	}

	void mmem::restore(FILE* f)
	{
		// Load size information
		fread(&(this->size), sizeof(size_t), 1, f);

		// Start filling the new memory zone
		delete[] this->data;
		this->data = new byte_8b[this->size];

		// Load the rest of the memory array
		fread(this->data, sizeof(byte_8b), this->size, f);
	}
}
