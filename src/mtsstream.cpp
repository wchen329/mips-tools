#include <cstdio>
#include "parser_err.h"
#include "mtsstream.h"

namespace mipsshell
{

	void asm_ostream::append(mips_tools::BW_32 data)
	{
		fwrite(&data, sizeof(mips_tools::BW_32), 1, this->f);
	}

	asm_ostream::asm_ostream(char * filename)
	{
		this->f = fopen(filename, "w");
		if(f == NULL)
		{
			// Throw an exception here
		}
	}

	asm_ostream::~asm_ostream()
	{
		fclose(this->f);
	}

	asm_ostream * mtsstream::asmout = nullptr;
}