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
#ifndef __STREAMS_H__
#define __STREAMS_H__

#include <cstdint>
#include <string>
#include "priscas_global.h"

namespace priscas_io
{

	/* A "text_stream"
	 * Or, another writable ("file") for the output of the simulator
	 * Derived versions of "text_stream" can be set to receive output and error streams of 
	 * the Shell
	 */
	class LINK_DE text_stream
	{
		public:
			virtual void operator<<(std::string& input) = 0;
			virtual void operator>>(std::string& output) = 0;
			virtual bool isNull() { return false; }
	};

	/* A text stream that does nothing
	 * -
	 */
	class LINK_DE null_text_stream : public text_stream
	{
		public:
			void operator<<(std::string& input) { }
			void operator>>(std::string& output) { }
			bool isNull() { return true; }
	};

	extern LINK_DE null_text_stream null_tstream;

	LINK_DE const extern priscas::UPString newLine;

	/* Utilities for printing a number
	 *
	 *
	 */
	class LINK_DE StrTypes
	{
		public:
			static std::string BoolToStr(bool in) { return in ? "true" : "false"; }
			static std::string IntToStr(int in);
			static std::string Int32ToStr(long in);
			static std::string UIntToStr(unsigned in);
			static std::string FloatToStr(float in);
			static std::string SizeToStr(size_t in);
			static std::string UInt32ToStr(unsigned long in);
			static std::string UInt64ToStr(unsigned long long in);
	};
}

#endif
