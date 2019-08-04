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
//   You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PARSER_ERR_H__
#define __PARSER_ERR_H__ 
#include <string>
namespace mipsshell
{
	// exception class
	class parser_err
	{
		public:
			virtual void err_pr(); // print the error
	};

	class badformat_err : public parser_err
	{
		public:
			void err_pr(); // print the error
	};

	class badimm_err : public parser_err
	{
		public:
			void err_pr();
	};

	class expected_err : public parser_err
	{
		public:
			void err_pr();
	};

	class bad_escape_err : public parser_err
	{
		std::string err;
		public:
			void err_pr(); // print the error
	};

	class unexpected_err : public parser_err
	{
		public:
			void err_pr();
			unexpected_err(const char * expected, const char * unexpected) : expected(expected), unexpected(unexpected) {};
		private:
			const char * unexpected;	// unexpected sequence
			const char * expected;
	};
}

#endif
