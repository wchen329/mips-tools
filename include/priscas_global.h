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
#ifndef __PRISCAS_GLOBAL_H__
#define __PRISCAS_GLOBAL_H__
#include <string>

/* Intended Global defines for PRISCAS Core, in all header files
 * wchen329
 */

namespace priscas
{
	/* Universal PRISCAS String
	 * Currently just a typedef, this will most likely change in the future
	 */
	typedef std::string UPString;
}

/* Namespace aliases for old names of namespaces
 * mips_tools -> priscas
 * mipsshell -> priscas
 */
namespace mips_tools = priscas;
namespace mipsshell = priscas;

#endif
