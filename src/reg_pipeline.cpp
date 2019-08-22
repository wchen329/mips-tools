//////////////////////////////////////////////////////////////////////////////
//
//    PRISCAS - Computer architecture simulator
//    Copyright (C) 2019 Winor Chen
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////
#include "reg_pipeline.h"

namespace priscas
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
