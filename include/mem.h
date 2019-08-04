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
//   You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MEM_H__
#define __MEM_H__
#include <cstddef>

namespace mips_tools
{

	/* A standard memory
	 * That represents anything that any level of the memory hierarchy
	 *
	 * wchen329
	 */
	class mem
	{
		public:
			virtual char& operator[](int ind);		// indexing operater into memory
			mem(size_t size = 1024);			// constructor takes, byte size of memory as argument
			size_t get_size(){return size;}		// returns size;
			~mem();
			char * begin() {return data;}		// get beginning address of data range

		protected:
				
		private:
			char * data;					// the actual data of memory
			size_t size;					// size of memory space in bytes
			mem operator=(const mem &);		// copy assignment, disabled
			mem(const mem &);				// copy constructor, disabled
	};

}
#endif
