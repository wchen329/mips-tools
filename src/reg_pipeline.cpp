#include "reg_pipeline.h"

namespace mips_tools
{

	void decode_ex_plr::load(	BW_32 data_rs,
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
								int rd )
	{
			this->data_rs = data_rs;
			this->data_rt = data_rt;
			this->func = func;
			this->shamt = shamt;
			this->imm = imm;
			this->op = op;
			this->regWE = regWE;
			this->memWE = memWE;
			this->memRE = memRE;
			this->rs = rs;
			this->rt = rt;
			this->rd = rd;
	}

	void decode_ex_plr::get(
						BW_32& data_rs,
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
						int& rd )
	{
		data_rs = this->data_rs;
		data_rt = this->data_rt;
		func = this->func;
		shamt = this->shamt;
		imm = this->imm;
		op = this->op;
		regWE = this->regWE;
		memWE = this->memWE;
		memRE = this->memRE;
		rs = this->rs;
		rt = this->rt;
		rd = this->rd;
	}

	void ex_mem_plr::load(	BW_32 data_ALU,
							BW_32 data_rs,
							BW_32 data_rt,
							opcode op,
							bool regWE,
							bool memWE,
							bool memRE,
							int rs,
							int rt,
							int rd )
	{
		this->data_ALU = data_ALU;
		this->data_rs = data_rs;
		this->data_rt = data_rt;
		this->op = op;
		this->regWE = regWE;
		this->memWE = memWE;
		this->memRE = memRE;
		this->rs = rs;
		this->rt = rt;
		this->rd = rd;
	}

	void ex_mem_plr::get(	BW_32& data_ALU,
							BW_32& data_rs,
							BW_32& data_rt,
							opcode& op,
							bool& regWE,
							bool& memWE,
							bool& memRE,
							int& rs,
							int& rt,
							int& rd )
	{
		data_ALU = this->data_ALU;
		data_rs = this->data_rs;
		data_rt = this->data_rt;
		op = this->op;
		regWE = this->regWE;
		memWE = this->memWE;
		memRE = this->memRE;
		rs = this->rs;
		rt = this->rt;
		rd = this->rd;
	}

	void mem_wb_plr::load(	BW_32 save_data,
							bool regWE,
							int r_save_num )
	{
		this->save_data = save_data;
		this->regWE = regWE;
		this->r_save_num = r_save_num;
	}

	void mem_wb_plr::get(	BW_32& save_data,
							bool& regWE,
							int& r_save_num )
	{
		save_data = this->save_data;
		regWE = this->regWE;
		r_save_num = this->r_save_num;
	}
}