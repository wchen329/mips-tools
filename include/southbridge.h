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
#ifndef __SOUTHBRIDGE_H__
#define __SOUTHBRIDGE_H__
#include <map>
#include "com.h"
#include "hdd.h"
#include "primitives.h"
#include "priscas_global.h"
#include "io_device.h"

namespace priscas
{
	/* Southbridge
	 * The "southbridge" on the motherboard
	 * is simply the I/O controller, and is accessed depending on ISA
	 *
	 * wchen329
	 */
	class southbridge
	{
		public:
			/* bool get_COM_Write_Ready(...)
			 * Checks to see if the com_numth com port is ready
			 * to be written to.
			 */
			bool get_COM_Write_Ready(unsigned com_num);

			/* bool get_COM_Read_Ready(...)
			 * Checks to see if the com_numth com port has a value 
			 * ready to be read
			 */
			bool get_COM_Read_Ready(unsigned com_num);

			/* void send_COM_Write(...)
			 * Send the byte data to the COM port
			 * This automatically performs the handshaking required by the CPU side.
			 */
			void send_COM_Write(byte_8b data, unsigned com_num);

			/* void send_COM_Read(...)
			 * This automatically performs the handshaking required by the CPU side.
			 * Does not read automatically necessarily. For now all COMs are instant read but this will change later.
			 * Use exec_COM_Read(...) to actually perform the read
			 */
			void send_COM_Read(unsigned com_num);

			/* void send_HDD_Write(...)
			 * Send byte data to an hdd to the address specified
			 */
			void send_HDD_Write(unsigned hdd_num, uint64_t address, byte_8b data);

			/* void send_HDD_Read(...)
			 * Read byte data from an hdd from the address specified
			 * Does not read automatically necessarily, use exec_HDD_read(...) to perform the read after
			 * the HDD is done setting up a read op (specified by the HDD speed)
			 */
			void send_HDD_Read(unsigned hdd_num, uint64_t address);

			/* byte_8b exec_COM_Read(...)
			 * Read from a COM port, make sure the read op is actually done in the device though!
			 */
			byte_8b exec_COM_Read(unsigned com_num);

			/* byte_8b exec_HDD_Read(...)
			 * Read from an HDD, make sure the read op is actuall done though!
			 */
			byte_8b exec_HDD_Read(unsigned hdd_num);

		private:
			std::vector<com_port*> communications_ports;
			std::vector<hdd*> hard_disks;
	};
}

#endif