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
#include "rtl_cpu_mips.h"

namespace priscas
{
	void mips_decoding_unit_32::decode(BW_32 inst)
	{
		int32_t inst_word = inst.AsInt32();

		// -Masks-
		int32_t opcode_mask = (~(1 << 26)) + 1;
		int32_t rs_mask = ~( ((~(1 << 26)) + 1) | ((1 << 21) - 1));
		int32_t rt_mask = ~( ((~(1 << 21)) + 1) | ((1 << 16) - 1));
		int32_t rd_mask = ~( ((~(1 << 16)) + 1) | ((1 << 11) - 1));
		int32_t funct_mask = 63;
		int32_t shamt_mask = (1 << 11) - 1 - funct_mask;
		int32_t imm_mask_i = (1 << 16) - 1;
		int32_t addr_mask = (1 << 26) - 1;

		// - Actual values
		MIPS_32::opcode op = static_cast<MIPS_32::opcode>(((opcode_mask & inst_word) >> 26) & ((1 << 6) - 1));
		MIPS_32::format fm;
		int rs;
		int rt;
		int rd;
		MIPS_32::funct func;
		int shamt;
		int imm;

		// Set a mode based on OP
		if(op == MIPS_32::R_FORMAT) fm = MIPS_32::R;
		else if(MIPS_32::j_inst(static_cast<MIPS_32::opcode>(op))) fm = MIPS_32::J;
		else fm = MIPS_32::I;

		// Then decode!
		rs = (rs_mask & inst_word) >> 21;
		rt = (rt_mask & inst_word) >> 16;
		rd = (rd_mask & inst_word) >> 11;
		func = static_cast<MIPS_32::funct>((funct_mask & inst_word));
		shamt = (shamt_mask & inst_word) >> 6;
		imm = fm == MIPS_32::I	? (imm_mask_i & inst_word) | (~(inst_word & (1 << 15)) + 1)
						: (addr_mask & inst_word) | (~(inst_word & (1 << 25)) + 1); // make it signed

		// Assign primitive signals out
		rs_out->explicit_charge(BW_32(rs));
		rt_out->explicit_charge(BW_32(rt));
		rd_out->explicit_charge(BW_32(rd));		
		funct_out->explicit_charge(BW_32(func));
		shamt_out->explicit_charge(BW_32(shamt));
		imm_out->explicit_charge(BW_32(imm));

		// Assign control signals
		BW_32 RegWrite = MIPS_32::reg_write_inst(op, func) && MIPS_32::getRegDst(rd, rt, op) != 0 ? 1 : 0;
		BW_32 RegDst = MIPS_32::getRegDst(1, 0, op);
		BW_32 ALUSrc = MIPS_32::r_inst(op) ? 0 : 1;
		BW_32 ALUOp = MIPS_32::getALUOpControlSig(op, func);
		BW_32 Branch = MIPS_32::jorb_inst(op, func) ? 1 : 0;
		BW_32 MemWrite = MIPS_32::mem_write_inst(op) ? 1 : 0;
		BW_32 MemRead = MIPS_32::mem_read_inst(op) ? 1 : 0;
		BW_32 MemToReg = MIPS_32::r_inst(op) ? 0 : MIPS_32::mem_read_inst(op) ? 1 : 0;

		// Send each signal out
		RegWrite_out->explicit_charge(RegWrite);
		RegDst_out->explicit_charge(RegDst);
		ALUSrc_out->explicit_charge(ALUSrc);
		ALUOp_out->explicit_charge(ALUOp);
		Branch_out->explicit_charge(Branch);
		MemWrite_out->explicit_charge(MemWrite);
		MemRead_out->explicit_charge(MemRead);
		MemToReg_out->explicit_charge(MemToReg);
	}

	void mips_alu_32::cycle()
	{
		// Receive inputs.
		BW_32 aluop = this->get_nth_input(input_ALUOp)->get_Drive_Output();
		BW_32 funct = this->get_nth_input(input_Funct)->get_Drive_Output();
		BW_32 srcdata1 = this->get_nth_input(input_Data_1)->get_Drive_Output();
		BW_32 srcdata2 = this->get_nth_input(input_Data_2)->get_Drive_Output();
		
		// Execute.
		BW_32 result = 0;
		MIPS_32::ALUOp rop = static_cast<MIPS_32::ALUOp>(aluop.AsInt32());
		MIPS_32::funct fcode = static_cast<MIPS_32::funct>(funct.AsUInt32());

		switch(rop)
		{
			case MIPS_32::ALUOp_ADD:
				result = srcdata1.AsUInt32() + srcdata2.AsUInt32();
				break;

			case MIPS_32::ALUOp_SUB:
				result = srcdata1.AsUInt32() - srcdata2.AsUInt32();
				break;

			case MIPS_32::ALUOp_FUNCT_DEFINED:
				result =
					fcode == MIPS_32::SLL ? srcdata1.AsUInt32() << srcdata2.AsUInt32() :
					fcode == MIPS_32::SRL ? srcdata1.AsUInt32() >> srcdata2.AsUInt32() : // currently arithmetic, change to logical
					fcode == MIPS_32::AND ? srcdata1.AsInt32() & srcdata2.AsInt32() :
					fcode == MIPS_32::OR ? srcdata1.AsUInt32() | srcdata2.AsUInt32() :
					fcode == MIPS_32::NOR ? ~(srcdata1.AsUInt32() | srcdata2.AsUInt32()) :
					fcode == MIPS_32::SLT ?
						srcdata1.AsInt32() < srcdata2.AsInt32() ? 1 : 0
						:
					fcode == MIPS_32::SLTU ?
						srcdata1.AsUInt32() < srcdata2.AsUInt32() ? 1 : 0
						:
					0;

				break;
		}

		this->set_Drive_Output(result);
	}
}