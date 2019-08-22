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
#ifndef __BUFFER_EVENTS_H__
#define __BUFFER_EVENTS_H__
#include "priscas_global.h"

/* buffer_events.h
 * This header is inplace for integration with event based facilities
 * (i.e. Qt) the contents within are not usable without an exterior definition
 */

namespace UI_integration
{
	/* Send an event notifying that the Standard Output
	 * buffer of the shell is ready
	 */
	void NotifyShellOutputReady();

	/* Send an event notifing the the Standard Error
	 * buffer of the shell is ready
	 */
	void NotifyShellErrorReady();

}

#endif
