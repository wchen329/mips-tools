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
			byte_8b& operator[](int ind);
		private:
			mmem operator=(const mem &);		// copy assignment, disabled
			mmem(const mem &);					// copy constructor, disabled
	};

}

#endif
