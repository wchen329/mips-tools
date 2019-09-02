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
#ifndef __DYNAMIC_H__
#define __DYNAMIC_H__

/* Declarations for dynamic linking related
 * such as certain macroes
 * wchen329
 */

/* dynamic linking options
 * (Windows only for now)
 */
#ifdef WIN32
	#define LINK_DI __declspec(dllimport) // Link Dynamic Import
	#define LINK_DE __declspec(dllexport) // Link Dynamic Export
#else
	#define LINK_DI
	#define LINK_DE
#endif

#endif
