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
#include "rtl_cpu_common.h"

namespace priscas
{
	bool RTLB_basic_sc::drive()
	{
		fetch();
		decode();
		execute();
		return false;
	}

	void PrimitiveAnd::cycle()
	{
		pDrivableList dl = this->get_drivers();

		pDrivableList::iterator dlter = dl.begin();

		// Get drive output
		const BW& A = (*dlter)->get_Drive_Output();

		++dlter;

		const BW& B = (*dlter)->get_Drive_Output();

		// Set the output as the bitwise and of them
		this->set_Drive_Output(BW_32(A.AsInt32() & B.AsInt32()));
	}
}