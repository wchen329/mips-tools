#ifndef __DIAG_CPU_H__
#define __DIAG_CPU_H__

// CPU with Diagnostic Functions
class diag_cpu : public cpu
{
			virtual BW_32 get_reg_data(int index) = 0;
			virtual void encode(int rs, int rt, int rd, int funct, int imm, opcode op) = 0;
			virtual void force_fetch(BW_32 codeword) = 0;	// force current instruction
};

#endif