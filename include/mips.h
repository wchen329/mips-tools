#ifndef __MIPS_H__
#define __MIPS_H__

/* A header for mips specifc details
 * such as register name mappings
 * and a jump list for functional routines
 *
 * Instruction Formats:
 * R - 6 opcode, 5 rs, 5 rt, 5 rd, 5 shamt, 6 funct
 * I - 6 opcode, 5 rs, 5 rt, 16 imm
 * J - 6 opcode, 26 addr
 *
 *
 * wchen329
 */
#include "primitives.h"
#include "reg_32.h"

namespace mips_tools
{
	// friendly register names
/*	enum REGISTERS
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
	};*/

	enum format
	{
		R, I, J	// instruction formats
	};

	// MIPS Operation Templates
	template <class T> inline T MIPS_ADD(T r_s, T r_t);	// to still do- enforce numerical specialization on classes
	template <class T> inline T MIPS_SUB(T r_s, T r_t);
	template <class T> inline T MIPS_SUBU(T r_s, T r_t);
	template <class T> inline T MIPS_ADDI(T r_s, long imm);
	template <class T> inline T MIPS_ADDIU(T r_s, long imm);
	template <class T> inline T MIPS_AND(T r_s, T r_t);
	template <class T> inline T MIPS_ANDI(T r_s, long imm);
	template <class T> inline T MIPS_OR(T r_s, T r_t);
	template <class T> inline T MIPS_ORI(T r_s, long imm);

}

#endif