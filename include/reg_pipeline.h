#ifndef __REG_PIPELINE_H__
#define __REG_PIPELINE_H__
#include "mips.h"

namespace mips_tools
{
	class decode_ex_plr
	{
		public:
			void load(	BW_32 data_1,
						BW_32 data_2,
						opcode op,
						bool regWE,
						bool memWE,
						bool memRE,
						int rs,
						int rt,
						int rd );
			void get(const BW_32& data_1, const BW_32& data_2, const opcode& op);
		private:
		BW_32 rs;
	};

	class ex_mem_plr
	{
		public:
			void load();
			void get();
		private:
	};

	class mem_wb_plr
	{
		public:
			void load();
			void get();
		private:
	};
}

#endif