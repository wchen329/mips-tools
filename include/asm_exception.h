#ifndef __ASM_EXCEPTION__
#define __ASM_EXCEPTION__
#include <string>

namespace mips_tools
{
	class asm_exception
	{
		public:
			const char * get_err_msg() { return this->err_msg.c_str(); }
			asm_exception() : err_msg("A general assembler error has occurred"), err_code(0) {}
		protected:
			std::string err_msg;
			int err_code;
	};
}

#endif