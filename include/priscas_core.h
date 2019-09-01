#ifndef __DLL_PRISCAS_CORE_H__
#define __DLL_PRISCAS_CORE_H__

#include "osi.h"
#ifdef DLE
	#undef DLE
	#define DLE DLI
#endif

/* Library import header
 * wchen329
 */
#include "cpu.h"
#include "diag_cpu.h"
#include "shell.h"

#endif