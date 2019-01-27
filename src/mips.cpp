#include "mips.h"

namespace mips_tools
{
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