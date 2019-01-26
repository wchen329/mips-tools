/* A header for mips specifc details
 * such as register name mappings
 * and a jump list for functional routines
 *
 * Instruction Formats:
 * R -
 * I -
 * J -
 *
 *
 * wchen329
 */
#include "primitives.h"
#include "reg_32.h"

namespace mips_tools
{
	// MIPS Operations
	long MIPS_ADD(long r_s, long r_t);
	long MIPS_SUB(long r_s, long r_t);
	long MIPS_ADDI(long r_s, long imm);
	long MIPS_AND(long r_s, long imm);

	// friendly register names
	enum REGISTERS
	{
		$zero = 0,
		$at = 1,
		$v0 = 2,
		$v0 = 3,
		$a0 = 4,
		$a1 = 5,
		$a2 = 6,
		$a3 = 7,
		$t0 = 8,
		$t1 = 9,
		$t2 = 10,
		$t3 = 11,
		$t4 = 12,
		$t5 = 13,
		$t6 = 14,
		$t7 = 15,
		$s0 = 16,
		$s1 = 17,
		$s2 = 18,
		$s3 = 19,
		$s4 = 20,
		$s5 = 21,
		$s6 = 22,
		$s7 = 23,
		$t8 = 24,
		$t9 = 25,
		$k0 = 26,
		$k1 = 27,
		$gp = 28,
		$sp = 29,
		$fp = 30,
		$ra = 31
	};
}