#ifndef __REG_PIPELINE_H__
#define __REG_PIPELINE_H__
#include "mips.h"

namespace mips_tools
{
	/* DE-EX
	 * Pipeline Register
	 */
	class decode_ex_plr
	{
		public:
			void load(	BW_32 data_rs,
						BW_32 data_rt,
						funct func,
						BW_32 shamt,
						BW_32 imm,
						opcode op,
						bool regWE,
						bool memWE,
						bool memRE,
						int rs,
						int rt,
						int rd );
			void get(	BW_32& data_rs,
						BW_32& data_rt,
						funct& func,
						BW_32& shamt,
						BW_32& imm,
						opcode& op,
						bool& regWE,
						bool& memWE,
						bool& memRE,
						int& rs,
						int& rt,
						int& rd );
		private:
			BW_32 data_rs;
			BW_32 data_rt;
			funct func;
			BW_32 shamt;
			BW_32 imm;
			opcode op;
			bool regWE;
			bool memWE;
			bool memRE;
			int rs;
			int rt;
			int rd;
	};

	/* EX-MEM
	 * Pipeline Register
	 */
	class ex_mem_plr
	{
		public:
			void load(	BW_32 data_ALU,
						BW_32 data_rs,
						BW_32 data_rt,
						opcode op,
						bool regWE,
						bool memWE,
						bool memRE,
						int rs,
						int rt,
						int rd );
			void get(	BW_32& data_ALU,
						BW_32& data_rs,
						BW_32& data_rt,
						opcode& op,
						bool& regWE,
						bool& memWE,
						bool& memRE,
						int& rs,
						int& rt,
						int& rd );
		private:
			BW_32 data_ALU;
			BW_32 data_rs;
			BW_32 data_rt;
			opcode op;
			bool regWE;
			bool memWE;
			bool memRE;
			int rs;
			int rt;
			int rd;
	};

	/* MEM-WB
	 * Pipeline Register
	 */
	class mem_wb_plr
	{
		public:
			void load(	BW_32 save_data,
						bool regWE,
						int r_save_num );
			void get(	BW_32& save_data,
						bool& regWE,
						int& r_save_num );
		private:
			BW_32 save_data;
			bool regWE;
			int r_save_num;
	};
}

#endif