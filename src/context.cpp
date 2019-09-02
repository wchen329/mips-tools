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
#include "context.h"

namespace priscas
{
	void sr_handler::image_save(srpackable& p, const UPString& filename)
	{
		FILE * f = fopen(filename.c_str(), "w");

		if(f == nullptr)
		{
			throw mt_io_file_open_failure(filename);
		}

		p.save(f);

		fclose(f);
	}

	void sr_handler::image_restore(srpackable& p, const UPString& filename)
	{
		FILE * f = fopen(filename.c_str(), "r");
		
		if(f == nullptr)
		{
			throw mt_io_file_open_failure(filename);
		}

		p.restore(f);

		fclose(f);
	}
}