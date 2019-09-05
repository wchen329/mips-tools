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
#include "southbridge.h"

namespace priscas
{
	void southbridge::send_HDD_Write(unsigned hdd_num, uint64_t address, byte_8b data)
	{
		if(hdd_num >= this->hard_disks.size())
		{
			return; // throw exception
		}

		hdd::hdd_write_byte_sequence req(address, data);
		hard_disks[hdd_num]->receive_req(req);
	}

	void southbridge::send_HDD_Read(unsigned hdd_num, uint64_t address)
	{
		if(hdd_num >= this->hard_disks.size())
		{
			return; // throw exc
		}

		hdd::hdd_read_byte_sequence req(address);
		hard_disks[hdd_num]->receive_req(req);
	}

	byte_8b southbridge::exec_HDD_Read(unsigned hdd_num)
	{
		return 0;
	}
}