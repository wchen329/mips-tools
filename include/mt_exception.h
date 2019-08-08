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
#ifndef __MT_EXCEPTION_H__
#define __MT_EXCEPTION_H__
#include <string>

/* MIPS Tools
 * Exception Class
 */

namespace mips_tools
{
	namespace exception_nums
	{
		enum exception_code
		{
			GENERAL = 0,
			REGISTER_OOB = 1,
			MEMORY_OOB = 2,
			INVALID_RANGE = 3,
			INVALID_CPU_OPT = 4
		};
	}


	// General Exception
	class mt_exception
	{
		public:
			int number() { return this->except_num; } // get exception number
			std::string& get_err() { std::string & msg = message; msg = msg + '\n'; return this->message; }	// get full exception name
			mt_exception() : except_num(exception_nums::GENERAL), message("An error has occurred.") {}

		protected:
			exception_nums::exception_code except_num;
			std::string message;
	};

	// Register Out of Bounds Exception
	class reg_oob_exception : public mt_exception
	{
		public:
			reg_oob_exception()
			{
				mt_exception::except_num = exception_nums::REGISTER_OOB;
				mt_exception::message = "Invalid registry index given : out of bounds.";
			
			}
	};

	// Memory Out of Bounds Exception
	class mem_oob_exception : public mt_exception
	{
		public:
			mem_oob_exception()
			{
				mt_exception::except_num = exception_nums::MEMORY_OOB;
				mt_exception::message = "Invalid memory index given: out of bounds";
			}
	};

	// Invalid Range Specifier Exception
	class mt_invalid_range : public mt_exception
	{
		public:
			mt_invalid_range(const char * info)
			{
				mt_exception::except_num = exception_nums::INVALID_RANGE;
				mt_exception::message = "Invalid range specifer: " + std::string(info);
			}
	};

	// Invalid CPU Option Error
	class mt_invalid_cpu_opt : public mt_exception
	{
		public:
			mt_invalid_cpu_opt(const char * info)
			{
				mt_exception::except_num = exception_nums::INVALID_CPU_OPT;
				mt_exception::message = "Invalid CPU Option: " + std::string(info);
			}
	};
}

#endif
