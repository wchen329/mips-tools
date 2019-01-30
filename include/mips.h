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
	int friendly_to_numerical(const char *);


	// Friendly Register Names -> Numerical Assignments
	enum REGISTERS
	{
		$zero = 0,
		$at = 1,
		$v0 = 2,
		$v1 = 3,
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
		$ra = 31,
		INVALID = -1
	};

	// instruction formats
	enum format
	{
		R, I, J	
	};

	// MIPS Processor Opcodes
	enum opcode
	{
		R_FORMAT = 0,
		DUMMY = 1,
		JUMP = 2,
		JAL = 3,
		BEQ = 4,
		BNE = 5,
		BLEZ = 6,
		BGTZ = 7,
		ADDI =  8,
		ADDIU = 9,
		SLTI = 10,
		SLTIU = 11,
		ANDI = 12,
		ORI = 13,
		XORI = 14,
		LUI = 15,
		LB = 32,
		LH = 33,
		LWL = 34,
		LW = 35,
		LBU = 36,
		LHU = 37,
		LWR = 38,
		SB = 40,
		SH = 41,
		SWL = 42,
		SW = 43,
		SYS_RES = -1	// system reserved for shell interpreter
	};

	// Function codes for R-Format Instructions
	enum funct
	{
		SLL = 0,
		SRL = 2,
		JR = 8,
		ADD = 32,
		ADDU = 33,
		SUB = 34,
		SUBU = 35,
		AND = 36,
		OR = 37,
		NOR = 39,
		SLT = 42,
		SLTU = 43,
		NONE = -1	// default, if not R format
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

	// Format check functions
	/* Checks if an instruction is I formatted.
	 */
	bool i_inst(opcode operation);

	/* Checks if an instruction is R formatted.
	 */
	bool r_inst(opcode operation);

	/* Checks if an instruction performs
	 * memory access
	 */
	bool mem_inst(opcode operation);

}

#endif