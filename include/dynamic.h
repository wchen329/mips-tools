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
