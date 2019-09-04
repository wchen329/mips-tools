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
#ifndef __HDD_H__
#define __HDD_H__
#include "io_device.h"
#include "priscas_global.h"

namespace priscas
{
	class hdd : public io_device
	{
		public:
			void receive_req(io_request& in) { in(*this);  }

		private:
			class hdd_read_byte_sequence
			{
				/* application operator
				 * Read a byte from a given address into the HDD's single byte
				 * read buffer
				 */
				void operator(io_device& dev);	
			};

			class hdd_write_byte_sequence
			{
				/* application operator
				 * Write a byte from the write buffer into the given address.
				 */
				void operator(io_device& dev);
			};
	};
}

#endif
