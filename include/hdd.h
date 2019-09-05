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
#include "primitives.h"
#include "priscas_global.h"

namespace priscas
{
	class hdd : public io_device
	{
		public:
			void receive_req(io_request& in) { in(*this);  }
			
			/* HDD
			 * zero out buffers
			 */
			hdd() :
				read_buf(0),
				write_buf(0)
			{}

			class hdd_read_byte_sequence : public io_request
			{
				public:
					/* application operator
					 * Read a byte from a given address into the HDD's single byte
					 * read buffer
					 */
					void operator()(io_device& dev);	

					/* constructor
					 * Arg: address
					 */
					hdd_read_byte_sequence(unsigned long long addr) : address(addr) {}

				private:

					// Address to read from, if bigger then disk
					// addressable, it is mod(address_size)
					unsigned long long address;
			};

			class hdd_write_byte_sequence : public io_request
			{
				public:

					/* application operator
					 * Write a byte from the write buffer into the given address.
					 */
					void operator()(io_device& dev);

					/* constructor
					 * Arg: address
					 */
					hdd_write_byte_sequence(unsigned long long addr, byte_8b data) : address(addr), writable(data) {}

				private:
	
					// Address to write to, if bigger than disk
					// addressable, it is mod(address_size)
					unsigned long long address;

					// Data to write
					byte_8b writable;
			};

		protected:
			/* byte_8b hddgetc()
			 * Read a byte from the HDD
			 */
			virtual byte_8b hddgetc(unsigned long long address) = 0;

			/* void hddputc(byte_8b)
			 * Write a byte to an address in the HDD
			 */
			virtual void hddputc(unsigned long long address, byte_8b) = 0;


		private:

			byte_8b read_buf;
			byte_8b write_buf;
	};

	/* HDD
	 * Implemented through a image on the local filesystem
	 * (Generation One)
	 */
	class hdd_img_gen1_impl : public hdd
	{
		public:
			hdd_img_gen1_impl(const UPString& filename);
		protected:
			byte_8b hddgetc(unsigned long long address);
			void hddputc(unsigned long long address, byte_8b);
		private:
			size_t offset;
			uint32_t disk_delay;
			uint32_t disk_size;
			FILE * hddimg;
	};
}

#endif
