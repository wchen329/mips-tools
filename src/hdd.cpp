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
#include "hdd.h"

namespace priscas
{
	void hdd::hdd_read_byte_sequence::operator()(io_device & dev)
	{
		hdd& hdev = detectAs<hdd>(dev);

		hdev.read_buf = hdev.hddgetc(this->address);
	}

	void hdd::hdd_write_byte_sequence::operator()(io_device & dev)
	{
		hdd& hdev = detectAs<hdd>(dev);

		hdev.hddputc(this->address, this->writable);
	}

	hdd_img_gen1_impl::hdd_img_gen1_impl(const UPString& filename)
	{
		// Open img
		hddimg = fopen(filename.c_str(), "r+");

		// If img is not valid, throw an exception
		if(hddimg == nullptr)
		{
			throw mt_io_file_open_failure(filename);
		}

		// Else start reading
		this->offset = 10 * sizeof(char) + 2 * sizeof(uint32_t);

		const size_t BUF_SIZE = 15;
		char buf[BUF_SIZE];
		memset(buf, 0, BUF_SIZE);

		// Read generation to verify
		if(fread(buf, sizeof(char), 10, this->hddimg) < 1)
		{
			throw mt_io_hdd_rw_io_failure();
		}

		// Read disk size
		if(fread(&(this->disk_size), sizeof(uint32_t), 1, this->hddimg) < 1)
		{
			throw mt_io_hdd_rw_io_failure();
		}

		// Read disk delay
		if(fread(&(this->disk_delay), sizeof(uint32_t), 1, this->hddimg) < 1)
		{
			throw mt_io_hdd_rw_io_failure();
		}
	}

	byte_8b hdd_img_gen1_impl::hddgetc(unsigned long long address)
	{
		// Normalize address
		uint32_t real_address = address % this->disk_size;

		// Start from beginning of file, offset is the sum of the read address and file header offset
		if(fseek(this->hddimg, this->offset + real_address, SEEK_SET))
		{
			throw mt_io_hdd_seek_failure();
		}

		byte_8b mini_buf;
		
		if(fread(&mini_buf, sizeof(byte_8b), 1, this->hddimg) < 1)
		{
			throw mt_io_hdd_rw_io_failure();
		}

		return mini_buf;
	}

	void hdd_img_gen1_impl::hddputc(unsigned long long address, byte_8b data)
	{
		// Normalize address
		uint32_t real_address = address % this->disk_size;

		// Start from beginning of file, offset is the sum of the read address and file header offset
		if(fseek(this->hddimg, this->offset + real_address, SEEK_SET))
		{
			throw mt_io_hdd_seek_failure();
		}

		// Do a write to that location
		if(fwrite(&data, sizeof(byte_8b), 1, this->hddimg) < 1)
		{
			throw mt_io_hdd_rw_io_failure();
		}
	}
}

