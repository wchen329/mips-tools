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
#ifndef __BRANDING_H__
#define __BRANDING_H__

// These predefines are specifically used for Windows Resource Compiler
#define PRISCAS_BRANDING_AUTHOR "Winor Chen"
#define PRISCAS_BRANDING_FULL_VERSION_STRING "0.2.2019.8"
#define PRISCAS_BRANDING_COPYRIGHT "Copyright (c) 2019 Winor Chen"
#define PRISCAS_BRANDING_FULL_VERSION 0,2,2019,8

/* Header that includes information about project name, description, and versioning, etc., etc.
 * wchen329
 */
namespace branding
{
	const std::string APPLICATION_NAME = std::string("PRISCAS");
	const std::string APPLICATION_DESC = std::string("[computer architecture simulator]");
	const std::string APPLICATION_VERSION = std::string("0.2");

	#ifdef P_RELEASE
		const std::string APPLICATION_DBG = "(release)";
	#else
		const std::string APPLICATION_DBG = "(developmental build)";
	#endif
}


#endif
