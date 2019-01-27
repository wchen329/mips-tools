#include "format_chk.h"


namespace mipsshell
{

	bool r_inst(op operation)
	{
		return
		
			operation == ADD ? true :
			operation == SUB ? true :
			operation == SUBU ? true :
			operation == AND ? true :
			operation == OR ? true :
			false ;
	}

	bool i_inst(op operation)
	{
		return
			operation == ADDI ? true :
			operation == ADDIU ? true:
			operation == ANDI ? true :
			operation == ORI ? true :
			false ;
	}
}