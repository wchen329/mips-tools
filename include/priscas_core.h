#ifndef __PRISCAS_CORE_H__
#define __PRISCAS_CORE_H__

/* PRISCAS Core Library Include
 * wchen329
 */
#include "osi.h"

// Change all exports to imports
#undef DLE
#define DLE DLI

#include "cpu.h"
#include "diag_cpu.h"
#include "primitives.h"
#include "range.h"
#include "shell.h"

#endif