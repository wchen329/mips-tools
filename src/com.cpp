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
#include "com.h"

namespace priscas
{
	void com_port::com_read_bytesequence::operator()(io_device& dev)
	{
		// Verify that this is working against a com port
		com_port& c = detectAs<com_port>(dev);
		c.read_buffer = c.comgetc();
	}

	void com_port::com_write_bytesequence::operator()(io_device& dev)
	{
		// Verify that this is working against a com port
		com_port& c = detectAs<com_port>(dev);
	}

	com_port_fileimpl::com_port_fileimpl(UPString fn_read, UPString fn_write)
	{
		if(fn_read == fn_write)
		{
			fp_read = fopen(fn_read.c_str(), "r+");
			fp_write = fp_read;
		}

		else
		{
			fp_read = fopen(fn_read.c_str(), "r");
			fp_write = fopen(fn_write.c_str(), "w");
		}

		bool invalid_handle_detected = true;

		if(fp_read == nullptr)
		{
			shared_cleanup();
			throw mt_io_file_open_failure(fn_read);
		}

		if(fp_write == nullptr)
		{
			shared_cleanup();
			throw mt_io_file_open_failure(fn_write);
		}
	}

/*	com_port_fileimpl::com_port_fileimpl(UPString filename, com_mode)
	{

	}*/

	com_port_fileimpl::~com_port_fileimpl()
	{
		shared_cleanup();
	}

	void com_port_fileimpl::shared_cleanup()
	{
		bool destroy_fread = fp_read == nullptr ? false : true;
		bool destroy_fwrite = fp_write == nullptr ? false : true;
		
		// Check for duplicate destruction
		if(fp_read == fp_write)
		{
			destroy_fwrite = false;
		}

		// Remove Read File handle
		if(destroy_fread)
		{
			fclose(fp_read);
		}

		// Remove Write File handle
		if(destroy_fwrite)
		{
			fclose(fp_write);
		}
	}

	byte_8b com_port_fileimpl::comgetc()
	{
		return fgetc(this->fp_read);
	}

	void com_port_fileimpl::computc(byte_8b in)
	{
		fwrite(&in, sizeof(byte_8b), 1, this->fp_write);
	}
}
