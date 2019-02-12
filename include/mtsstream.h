#ifndef __MTSSTREAM_H__
#define __MTSSTREAM_H__
#include "primitives.h"

namespace mipsshell
{

	class asm_ostream
	{
		public:	
			void append(mips_tools::BW_32);
			asm_ostream(char * filename);
			~asm_ostream();
		private:
			FILE * f;
			asm_ostream(asm_ostream &);
			asm_ostream& operator=(asm_ostream &);
	};

	namespace mtsstream
	{
		extern asm_ostream * asmout;
	}
}

#endif