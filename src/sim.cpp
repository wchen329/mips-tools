#include "mb.h"

/* Main routine
 *
 * A simple MIPS processor emulator
 * 
 * Processor is to be configured in a cpu.ini file in the directory where the binary resides.
 * Options to define in cpu.ini are defined in the documentation
 *
 * wchen329
 */
void init()
{
	// Make a new computer
	mips_tools::mb computer;
	computer.dc_on();	// turn on the computer, run until hlt

}