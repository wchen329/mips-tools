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
#ifndef __COM_H__
#define __COM_H__
#include <cstdio>
#include "io_device.h"
#include "mt_exception.h"
#include "primitives.h"
#include "priscas_global.h"

namespace priscas
{
	/* Communications
	 * Serial Port
	 * (a very simple one)
	 */
	class com_port : public io_device
	{
		public:

			// Receive Request
			void receive_req(io_request& in) { in(*this); }

			/* peek_read();
 			 * Get the current value in the read buffer
			 */
			byte_8b peek_read() { return this->read_buffer; }

			// COM Port specific requests
			class com_read_bytesequence : public io_request
			{
				void operator()(io_device& iod_ref);
			};

			class com_write_bytesequence : public io_request
			{
				void operator()(io_device& iod_ref);
			};

			/* com_port()
			 * Set the COM port buffers
			 */
			com_port() :
				write_buffer(0),
				read_buffer(0)
			{}

		protected:

			/* byte_8b comget_c()
			 * Get the next byte in the sequence.
			 */
			virtual byte_8b comgetc() = 0;

			/* byte_8b comput_c()
			 * Put the next byte in the serial target.
			 */
			virtual void computc(byte_8b) = 0;

		private:
			byte_8b write_buffer;
			byte_8b read_buffer;

	};

	/* com_port_fileimpl
	 * A communication port to a specific file.
	 */
	class com_port_fileimpl : public com_port
	{
		public:

			enum com_mode
			{
				COM_READ_ONLY,
				COM_WRITE_ONLY
			};

			/* com_port_fileimpl(...)
			 * Opens a communications port into files
			 * Writing and reading can be done to different files.
			 * If the filenames are the same, then that file will be put into update mode for
			 * read AND write access.
			 *
			 * If only read or write access (but not both) is needed, use the other constructor
			 */
			com_port_fileimpl(UPString filename_read, UPString filename_write);
//			com_port_fileimpl(UPString filename, com_mode);

		protected:

			/* byte_8b comget_c()
			 * Get the next byte in the sequence.
			 */
			byte_8b comgetc();

			/* byte_8b comput_c()
			 * Put the next byte in the serial target.
			 */
			void computc(byte_8b);
		
		private:
			// Disable copy construction
			com_port_fileimpl(const com_port_fileimpl&);
			com_port_fileimpl operator=(const com_port_fileimpl);

			void shared_cleanup();
			~com_port_fileimpl();
			FILE * fp_read;
			FILE * fp_write;
	};
}

#endif
