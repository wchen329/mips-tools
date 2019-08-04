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
#ifndef __STATES_H__
#define __STATES_H__

namespace mipsshell
{
	extern bool ASM_MODE;		// acting in assembler mode? true if is, false if not
	extern bool DEBUG_MODE;		// acting in debug mode? true if is, false if now
	extern bool INTERACTIVE;	// in interactive mode, true if is, false if not
	extern bool HAS_INPUT;		// no input file
	extern bool INPUT_SPECIFIED;
	extern bool SUSPEND;
	extern bool PRE_ASM;
	extern bool WIN_32_GUI;
	extern bool EXIT_COND;
}

#endif
