#include <cstring>
#include "mips.h"

namespace mips_tools
{
	int friendly_to_numerical(char * fr_name)
	{
		int len = strlen(fr_name);
		if(len < 2) return INVALID;

		REGISTERS reg_val
			=
			// Can optimize based off of 
			fr_name[1] == 'a' ?
				!strcmp("$a0", fr_name) ? $a0 :
				!strcmp("$a1", fr_name) ? $a1 :
				!strcmp("$a2", fr_name) ? $a2 :
				!strcmp("$a3", fr_name) ? $a3 :
				!strcmp("$at", fr_name) ? $at : INVALID
			:

			fr_name[1] == 'f' ?
				!strcmp("$fp", fr_name) ? $fp : INVALID
			:

			fr_name[1] == 'g' ?
				!strcmp("$gp", fr_name) ? $gp : INVALID
			:

			fr_name[1] == 'k' ?
				!strcmp("$k0", fr_name) ? $k0 :
				!strcmp("$k1", fr_name) ? $k1 : INVALID
			:

			fr_name[1] == 'r' ?
				!strcmp("$ra", fr_name) ? $ra : INVALID
			:

			fr_name[1] == 's' ?
				!strcmp("$s0", fr_name) ? $s0 :
				!strcmp("$s1", fr_name) ? $s1 :
				!strcmp("$s2", fr_name) ? $s2 :
				!strcmp("$s3", fr_name) ? $s3 :
				!strcmp("$s4", fr_name) ? $s4 :
				!strcmp("$s5", fr_name) ? $s5 :
				!strcmp("$s6", fr_name) ? $s6 :
				!strcmp("$s7", fr_name) ? $s7 : INVALID
			:

			fr_name[1] == 't' ?
				!strcmp("$t0", fr_name) ? $t0 :
				!strcmp("$t1", fr_name) ? $t1 :
				!strcmp("$t2", fr_name) ? $t2 :
				!strcmp("$t3", fr_name) ? $t3 :
				!strcmp("$t4", fr_name) ? $t4 :
				!strcmp("$t5", fr_name) ? $t5 :
				!strcmp("$t6", fr_name) ? $t6 :
				!strcmp("$t7", fr_name) ? $t7 : INVALID
			:

			fr_name[1] == 'v' ?
				!strcmp("$v0", fr_name) ? $v0 :
				!strcmp("$v1", fr_name) ? $v1 : INVALID

			: INVALID;

		return reg_val;
	}

	bool r_inst(opcode operation)
	{
		return
		
			operation == R_FORMAT ? true :
			false ;
	}

	bool i_inst(opcode operation)
	{
		return
			operation == ADDI ? true :
			operation == ADDIU ? true:
			operation == ANDI ? true :
			operation == ORI ? true :
			false ;
	}

	template <class T> inline T MIPS_ADD(T r_s, T r_t) { return r_s + r_t; }	// to still do- enforce numerical specialization on classes
	template <class T> inline T MIPS_SUB(T r_s, T r_t) { return r_s - r_t; }
	template <class T> inline T MIPS_SUBU(T r_s, T r_t) { return MIPS_SUB(r_s, r_t) };	// to still do, reimplement based on exception detection
	template <class T> inline T MIPS_ADDI(T r_s, long imm) { return r_s + imm; }
	template <class T> inline T MIPS_ADDIU(T r_s, long imm) { return MIPS_ADDI(r_s, imm); }
	template <class T> inline T MIPS_AND(T r_s, T r_t) { return r_s & r_t; }
	template <class T> inline T MIPS_ANDI(T r_s, long imm) { return r_s & imm; }
	template <class T> inline T MIPS_OR(T r_s, T r_t) { return r_s | r_t ; }
	template <class T> inline T MIPS_ORI(T r_s, long imm) { return r_s | imm };
}