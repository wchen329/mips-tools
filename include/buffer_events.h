#ifndef __BUFFER_EVENTS_H__
#define __BUFFER_EVENTS_H__
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