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
#include "pipeline.h"

namespace priscas
{
	bool fsp_cpu::cycle()
	{
		// Write Enable signals for pipeline registers and PC, used for stalling
		bool we_pc = true;
		bool we_plr_mw = true;
		bool we_plr_em = true;
		bool we_plr_de = true;
		bool we_plr_fetch = true;
		BW_32 pc_original_fetch =this->pc.get_data();
		BW_32 pc_next = this->pc.get_data();		

		// Current Cycle Flush Signals
		bool if_flush_cycle = false;
		bool em_flush_cycle = false;
		bool de_flush_cycle = false;

		// Get a new id right away
		this->if_sig = this->next_sig;

		// Register the sig corresponding to the PC number
		if(!this->pipeline_diagram->hasSig(this->next_sig))
		{
			this->pipeline_diagram->registerPC(this->if_sig, pc_original_fetch.AsUInt32());
		}

		unsigned long pipeline_cycle_32 = static_cast<unsigned long>(current_cycle_num);

		// Write the pipeline state BEFORE anything has happened
		if(this->if_sig >= 0)
		{
			this->pipeline_diagram->setData(pipeline_cycle_32, if_sig, "F");
		}

		if(this->id_sig >= 0)
		{
			this->pipeline_diagram->setData(pipeline_cycle_32, id_sig, "D");
		}

		if(this->ex_sig >= 0)
		{
			this->pipeline_diagram->setData(pipeline_cycle_32, ex_sig, "X");
		}

		if(this->mem_sig >= 0)
		{
			this->pipeline_diagram->setData(pipeline_cycle_32, mem_sig, "M");
		}

		if(this->wb_sig >= 0)
		{
			this->pipeline_diagram->setData(pipeline_cycle_32, wb_sig, "W");
		}

		/* Step 1: Execute but do not yet commit transactions
		 * Save temporaries to be commited into pipeline registers
		 *
		 * In order to allow forwarding to happen evaluate backwards
		 * (this doesn't effect the correctness of regular operation as
		 *  everything is in one clock cycle)
		 */



		/* WRITE-BACK STAGE
		 * Write back a register if the original instruction was write enabled
		 * or do nothing if not...
		 *
		 */
		BW_32 wb_save_data;
		int wb_save_num;
		bool wb_regWE;
		this->mw_plr.get(wb_save_data, wb_regWE, wb_save_num);

		if(wb_regWE)
		{
			this->registers[wb_save_num].set_data(wb_save_data); // Register-File bypassing
		}

		/* MEMORY STAGE
		 * If memRE or memWE then access memory at the stage.
		 * If not well... do (mostly) nothing, and just pass the corresponding flags over
		 * to the write back stage.
		 */
		BW_32 mem_data_rs, mem_data_rt, mem_dataALU;
		opcode mem_op;
		bool mem_regWE, mem_memWE, mem_memRE;
		int mem_rs, mem_rt, mem_rd;
		this->em_plr.get(mem_dataALU, mem_data_rs, mem_data_rt, mem_op, mem_regWE, mem_memWE, mem_memRE, mem_rs, mem_rt, mem_rd);

		// MEM->MEM forwarding
		if(mem_write_inst(mem_op) && wb_regWE && wb_save_num != 0)
		{
			if(this->cp.get_ControlValue_Raw(FSP_Options::getName_MEM_TO_MEM()) == FSP_Options::valueRaw_FORWARD())
			{
				if(mem_rt == wb_save_num) mem_data_rt = wb_save_data;
			}
		}

		// Memory operations
		BW_32 lr_bbb(0,0,0,0);
		
		switch(mem_op)
		{
			case LBU:
				lr_bbb = BW_32(this->mem_req_load(mem_dataALU.AsUInt32()),0,0,0);
				break;
			case LHU:
				lr_bbb = BW_32(		this->mem_req_load(mem_dataALU.AsUInt32()),
									this->mem_req_load(mem_dataALU.AsUInt32() + 1),
									0,
									0								);
				break;
			case LW:
				lr_bbb = BW_32(		this->mem_req_load(mem_dataALU.AsUInt32()),
									this->mem_req_load(mem_dataALU.AsUInt32() + 1),
									this->mem_req_load(mem_dataALU.AsUInt32() + 2),
									this->mem_req_load(mem_dataALU.AsUInt32() + 3));
				break;
			case SB:
				this->mem_req_write(BW_32(mem_data_rt).b_0(), mem_dataALU.AsUInt32());
				break;
			case SH:
				this->mem_req_write(BW_32(mem_data_rt).b_0(), mem_dataALU.AsUInt32());
				this->mem_req_write(BW_32(mem_data_rt).b_1(), mem_dataALU.AsUInt32() + 1);
				break;
			case SW:
				this->mem_req_write(BW_32(mem_data_rt).b_0(), mem_dataALU.AsUInt32());
				this->mem_req_write(BW_32(mem_data_rt).b_1(), mem_dataALU.AsUInt32() + 1);
				this->mem_req_write(BW_32(mem_data_rt).b_2(), mem_dataALU.AsUInt32() + 2);
				this->mem_req_write(BW_32(mem_data_rt).b_3(), mem_dataALU.AsUInt32() + 3);
				break;
		}

		BW_32 load_result = lr_bbb;

		// Registry writing settings
		int mem_write_reg_num = r_inst(mem_op) ? mem_rd : mem_rt;
		BW_32 mem_regWriteData = mem_read_inst(mem_op) ? load_result : mem_dataALU;

		/* EXECUTE STAGE
		 * Perform arithmetic through the provided ALU (mips_alu<int_32t> alu)
		 * Most operations will end up using the ALU, even if it's not used as a store
		 * except branches and jumps
		 */
		BW_32 ex_data_rs, ex_data_rt, ex_shamt, ex_imm;
		opcode ex_op;
		funct ex_funct;
		format ex_fm;
		int ex_rs, ex_rt, ex_rd;
		bool ex_memWE, ex_memRE, ex_regWE;
		this->de_plr.get(ex_data_rs, ex_data_rt, ex_funct, ex_shamt, ex_imm, ex_op, ex_regWE, ex_memWE,
			ex_memRE, ex_rs, ex_rt, ex_rd);
		
		if(ex_op == R_FORMAT) ex_fm = R;
		else if(j_inst(static_cast<opcode>(ex_op))) ex_fm = J;
		else ex_fm = I;

		mips_alu<int32_t> alu; 

		BW_32 ex_aluResult = 0;

		// MEM->EX forwarding
		if(wb_regWE && wb_save_num != 0)
		{
			//if(sc_cpu::cpu_opts[MEM_EX_INDEX].get_IntValue() == PATH_FORWARD_MODE)
			//{	
				if(ex_rs == wb_save_num) ex_data_rs = wb_save_data;
				if(ex_rt == wb_save_num) ex_data_rt = wb_save_data;
			//}
		}

		// EX->EX forwarding
		if(mem_regWE)
		{
			//if(sc_cpu::cpu_opts[EX_EX_INDEX].get_IntValue() == PATH_FORWARD_MODE)
			//{

				if(r_inst(mem_op))
				{
					// R instructions write to RD, and their results are saved directly
					if(ex_rs == mem_rd && mem_rd != 0) ex_data_rs = mem_dataALU;
					if(ex_rt == mem_rd && mem_rd != 0) ex_data_rt = mem_dataALU;

				}

				else
				{
					// Others use Rt (if at all)
					if(ex_rs == mem_rt && mem_rt != 0) ex_data_rs = mem_dataALU;
					if(ex_rt == mem_rt && mem_rt != 0) ex_data_rt = mem_dataALU;
				}
			//}
		}

		// EX->EX Dependency, load to use, STALL required
		if(mem_read_inst(mem_op))
		{
				if(!(mem_write_inst(ex_op) && (ex_rt == mem_rt && mem_rt != 0)
					&& ex_rs != mem_rt && this->cp.get_ControlValue_Raw(FSP_Options::getName_MEM_TO_MEM()) != FSP_Options::valueRaw_STALL()) // special case checking for MEM-MEM forwarding
					&& !(r_inst(ex_op) && ex_rd == 0))
				{
					if((ex_rs == mem_rt && mem_rt != 0) || (!(reg_write_inst(ex_op, ex_funct)) && (ex_rt == mem_rt && mem_rt != 0)))
					{
						em_flush_cycle = true;
						we_plr_fetch = false;
						we_plr_de = false;
						we_pc = false;
					}
				}
		}

		switch(ex_fm)
		{
			case R:
				
				// Eventually replace these with template functions
				switch(ex_funct)
				{
					case ADD:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case ADDU:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsUInt32(), ex_data_rt.AsUInt32(), false);
						break;
					case OR:
						ex_aluResult = alu.execute(ALU::OR, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case NOR:
						ex_aluResult = ~alu.execute(ALU::OR, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case AND: 
						ex_aluResult = alu.execute(ALU::AND, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case SLL:
						ex_aluResult = alu.execute(ALU::SLL, ex_data_rs.AsInt32(), ex_shamt.AsInt32(), false);
						break;
					case SRL:
						ex_aluResult = alu.execute(ALU::SRL, ex_data_rs.AsInt32(), ex_shamt.AsInt32(), false);
						break;
					case SLT:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false) < 0 ? 1 : 0;
						break;
					case SLTU:
						ex_aluResult = ex_data_rs.AsUInt32() < ex_data_rt.AsUInt32() ? 1 : 0;
						break;
					case SUB:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case SUBU:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs.AsUInt32(), ex_data_rt.AsUInt32(), false);
						break;
				}

				break;
				
			case I:
				switch(ex_op)
				{
					// Arithmetic Operations that Store
					case ADDI:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsInt32(), ex_imm.AsInt32(), false);
						break;
					case ADDIU:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsUInt32(), ex_imm.AsUInt32(), false);
						break;
					case ORI:
						ex_aluResult = alu.execute(ALU::OR, ex_data_rs.AsInt32(), ex_imm.AsInt32(), false);
						break;;
					case ANDI:
						ex_aluResult = alu.execute(ALU::AND, ex_data_rs.AsInt32(), ex_imm.AsInt32(), false);
						break;
					case XORI:
						ex_aluResult = alu.execute(ALU::XOR, ex_data_rs.AsInt32(), ex_imm.AsInt32(), false);
						break;
					case SLTI:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs.AsInt32(), ex_imm.AsInt32(), false) < 0 ? 1 : 0;
						break;
					case SLTIU:
						ex_aluResult = ex_data_rs.AsUInt32() < ex_imm.AsUInt32() ? 1 : 0;
						break;
					
					// Memory Operations- for now, calculate the offset only
					case LBU:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsInt32(), ex_imm.AsInt32(), true);
						break;
					case LHU:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsInt32(), ex_imm.AsInt32(), true);
						break;
					case LW:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsInt32(), ex_imm.AsInt32(), false);
						break;
					case SB:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsInt32(), ex_imm.AsInt32(), false);
						break;
					case SH:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsInt32(), ex_imm.AsInt32(), false);
						break;
					case SW:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs.AsInt32(), ex_imm.AsInt32(), false);
						break;


				}
				break;

			case J:
				switch(ex_op)
				{
					// Special case: JAL- use immediate addition
					case JAL:
						ex_aluResult = alu.execute(ALU::ADD, 0, ex_shamt.AsInt32(), false);
						break;
				}
		}

		/* DECODE STAGE
		 * Figure out key parameters from the instruction word
		 * But also at this stage, resolve Jumps and Branches
		 */
		mips_decoding_unit_32 decoding_unit;
		format decode_fm;
		opcode decode_op;
		int decode_rs, decode_rt, decode_rd;
		funct decode_funct;
		int32_t decode_shamt;
		int32_t decode_imm;

		decoding_unit.decode(fetch_plr.get_data(), decode_fm, decode_op, decode_rs, decode_rt,
			decode_rd, decode_funct, decode_shamt, decode_imm);

		BW_32 decode_rs_data = this->registers[decode_rs].get_data();
		BW_32 decode_rt_data = this->registers[decode_rt].get_data();

		bool EX_EX_ENABLED = false;

		// EX-ID Forwarding Path, specifically for Control Hazards
		if(mem_regWE && jorb_inst(decode_op, decode_funct))
		{
			//if(sc_cpu::cpu_opts[EX_ID_INDEX].get_IntValue() == PATH_FORWARD_MODE)
			//{	
				if(r_inst(mem_op))
				{
					// R instructions write to RD, and their results are saved directly
					if(decode_rs == mem_rd && decode_rs != 0) decode_rs_data = mem_dataALU;
					if(decode_rt == mem_rd && decode_rt != 0) decode_rt_data = mem_dataALU;
				}

				else
				{
					// Else, a little trickier
					// If it's a memory operation (LOAD) then the value isn't ready yet. Use a stall instead
					// Otherwise, forward it!
					if(decode_rs == mem_rt && decode_rs != 0) decode_rs_data = mem_dataALU;
					if(decode_rt == mem_rt && decode_rt != 0) decode_rt_data = mem_dataALU;
				}
			//}
		}

		// EX-ID Load to Use handling, a REQUIRED stall for branches
		if(mem_regWE && jorb_inst(decode_op, decode_funct) && mem_read_inst(ex_op))
		{
			if((decode_rs == mem_rt && mem_rt != 0) || (!(reg_write_inst(decode_op, decode_funct)) && (decode_rt == mem_rt && mem_rt != 0)))
			{
				de_flush_cycle = true;
				we_pc = false;
				we_plr_fetch = false;
			}
		}

		// Check for ID->ID dependency, a REQUIRED stall for branches
		if(ex_regWE && jorb_inst(decode_op, decode_funct))
		{
			if(r_inst(ex_op))
			{
				if((decode_rs == ex_rd && decode_rs != 0) || (decode_rt == ex_rd && decode_rt != 0))
				{
						de_flush_cycle = true;
						we_pc = false;
						we_plr_fetch = false;
				}
			}

			else
			{
				if((decode_rs == ex_rt && decode_rs != 0) || (decode_rt == ex_rt && decode_rt != 0))
				{
						de_flush_cycle = true;
						we_pc = false;
						we_plr_fetch = false;
				}
			}
		}

		// Check for MEM->ID dependency, which is only needed if a load is present
		if(mem_regWE && jorb_inst(decode_op, decode_funct) && mem_read_inst(mem_op))
		{
			if((decode_rs == mem_rt && decode_rs != 0) || (decode_rt == mem_rt && decode_rt != 0))
			{
					de_flush_cycle = true;
					we_pc = false;
					we_plr_fetch = false;
			}
		}

		// Execute branch instruction
		bool branch_taken = false;
		bool branch_inst = true;
		BW_32 branch_addr = (decode_imm << 2);
		BW_32 jump_mask = ~((1 << 28) - 1);
		BW_32 jump_addr = (fetch_plr.get_data().AsInt32() & jump_mask.AsInt32()) | (decode_imm << 2);
		switch(decode_op)
		{
				case BEQ:
						if(decode_rs_data == decode_rt_data)
						{
							BW_32 curr_pc = this->get_PC();
							pc_next = curr_pc.AsInt32() + branch_addr.AsInt32();
							branch_taken = true;
						}

						break;
				case BNE:
						if(decode_rs_data != decode_rt_data)
						{
							BW_32 curr_pc = this->get_PC();
							pc_next = curr_pc.AsInt32() + branch_addr.AsInt32();
							branch_taken = true;
						}
						break;
				case JUMP:
						{
							BW_32 curr_pc = this->get_PC();
							pc_next = jump_addr.AsInt32();
							branch_taken = true;
						}
						break;
				case JAL:
						{
							BW_32 curr_pc = this->get_PC();
							pc_next = jump_addr.AsInt32();
							decode_shamt = pc.get_data().AsInt32() + 4;
							decode_rt = $ra; // treat the register save as a regular 
							branch_taken = true;
						}
						break;
				case R_FORMAT:
						if(decode_funct == JR)
						{
							pc_next = decode_rs_data;
							branch_taken = true;
						}
						break;
				default:
						branch_inst = false;
		}

		// Write data to carry on
		bool decode_regWE = reg_write_inst(decode_op, decode_funct) && ((r_inst(decode_op) && decode_rd != 0 ) || ((!r_inst(decode_op) && decode_rt != 0)));
		bool decode_memWE = mem_write_inst(decode_op);
		bool decode_memRE = mem_read_inst(decode_op);

		/* FETCH STAGE
		 * Fetch an instruction
		 *
		 *
		 */
		BW_32 next_inst_addr = pc.get_data();
		BW_32 next_inst
		(
			this->mem_req_load(next_inst_addr.AsUInt32()),
			this->mem_req_load(next_inst_addr.AsUInt32() + 1),
			this->mem_req_load(next_inst_addr.AsUInt32() + 2),
			this->mem_req_load(next_inst_addr.AsUInt32() + 3)
		);

		if((!branch_taken) || !branch_inst)
		{
			pc_next = pc.get_data().AsUInt32() + 4;
		}
		else
		{
			if_flush_cycle = true;
		}

		/* Commit Transactions
		 */
		if(we_plr_mw)
		{
			this->wb_sig = this->mem_sig;
			this->mw_plr.load(mem_regWriteData, mem_regWE, mem_write_reg_num);
		}

		if(we_plr_em)
		{
			this->mem_sig = this->ex_sig;
			this->em_plr.load(ex_aluResult, ex_data_rs, ex_data_rt, ex_op, ex_regWE, ex_memWE, ex_memRE, ex_rs, ex_rt, ex_rd);

		}

		if(we_plr_de)
		{
			this->de_plr.load(decode_rs_data, decode_rt_data, decode_funct, decode_shamt, decode_imm,
				decode_op, decode_regWE, decode_memWE, decode_memRE, decode_rs, decode_rt, decode_rd);
			this->ex_sig = this->id_sig;
		}

		if(we_plr_fetch)
		{
			this->fetch_plr.set_data(next_inst);
			this->id_sig = this->if_sig;
		}
		
		if(we_pc)
		{
			pc.set_data(pc_next);
			this->next_sig++;
		}

		// Commit flushes as needed
		if(em_flush_cycle)
		{
			this->flush_em_plr();
			this->mem_sig = -1;
		}

		if(de_flush_cycle && !em_flush_cycle)
		{
			this->flush_de_plr();
			this->ex_sig = -1;
		}

		if(if_flush_cycle && !de_flush_cycle && !em_flush_cycle)
		{
			this->flush_fetch_plr();
			this->id_sig = -1;
		}

		// Second take of register write back, important for debugging views
		BW_32 rwb_reg_val;
		bool rwb_regWE;
		int rwb_save_num;
		this->mw_plr.get(rwb_reg_val, rwb_regWE, rwb_save_num);
		if(rwb_regWE && rwb_save_num != 0)
		{
			this->registers[rwb_save_num].set_data(rwb_reg_val); // Register-File bypassing
		}

		// Set probes
		this->ifid_dbg->findChild(DBG_INSTRUCTION_WORD)->setValue(fetch_plr.get_data().toHexString());

		BW_32 de_rs_data_dbg;
		BW_32 de_rt_data_dbg;
		priscas::funct de_funct_dbg_raw;
		BW_32 de_shamt_dbg;
		BW_32 de_imm_dbg;
		priscas::opcode de_op_dbg_raw;
		bool de_regwe_dbg;
		bool de_memwe_dbg;
		bool de_memre_dbg;
		int de_rs_dbg;
		int de_rt_dbg;
		int de_rd_dbg;
		this->de_plr.get(de_rs_data_dbg, de_rt_data_dbg, de_funct_dbg_raw, de_shamt_dbg, de_imm_dbg, de_op_dbg_raw, de_regwe_dbg, de_memwe_dbg, de_memre_dbg, de_rs_dbg, de_rt_dbg, de_rd_dbg);
		BW_32 de_op_dbg = de_op_dbg_raw;
		BW_32 de_funct_dbg = de_funct_dbg_raw;

		BW_32 em_dalu_dbg;
		BW_32 em_rs_data_dbg;
		BW_32 em_rt_data_dbg;
		priscas::opcode em_op_dbg_raw;
		bool em_regwe_dbg;
		bool em_memwe_dbg;
		bool em_memre_dbg;
		int em_rs_dbg;
		int em_rt_dbg;
		int em_rd_dbg;
		this->em_plr.get(em_dalu_dbg, em_rs_data_dbg, em_rt_data_dbg, em_op_dbg_raw, em_regwe_dbg, em_memwe_dbg, em_memre_dbg, em_rs_dbg, em_rt_dbg, em_rd_dbg);
		BW_32 em_op_dbg = em_op_dbg_raw;

		BW_32 db_mwdata;
		bool db_mwRegWE;
		int db_mwRNum;
		this->mw_plr.get(db_mwdata, db_mwRegWE, db_mwRNum);
		this->idex_dbg->findChild(DBG_IDEX_DATA_RS)->setValue(de_rs_data_dbg.toHexString());
		this->idex_dbg->findChild(DBG_IDEX_DATA_RT)->setValue(de_rt_data_dbg.toHexString());
		this->idex_dbg->findChild(DBG_IDEX_RS_N)->setValue(priscas_io::StrTypes::Int32ToStr(de_rs_dbg));
		this->idex_dbg->findChild(DBG_IDEX_RT_N)->setValue(priscas_io::StrTypes::Int32ToStr(de_rt_dbg));
		this->idex_dbg->findChild(DBG_IDEX_RD_N)->setValue(priscas_io::StrTypes::Int32ToStr(de_rd_dbg));
		this->idex_dbg->findChild(DBG_IDEX_FUNCT)->setValue(de_funct_dbg.toHexString());
		this->idex_dbg->findChild(DBG_IDEX_SHAMT)->setValue(de_shamt_dbg.toHexString());
		this->idex_dbg->findChild(DBG_IDEX_IMM)->setValue(de_imm_dbg.toHexString());
		this->idex_dbg->findChild(DBG_IDEX_OP)->setValue(de_op_dbg.toHexString());
		this->idex_dbg->findChild(DBG_IDEX_REGWE)->setValue(priscas_io::StrTypes::BoolToStr(de_regwe_dbg));
		this->idex_dbg->findChild(DBG_IDEX_MEMWE)->setValue(priscas_io::StrTypes::BoolToStr(de_memwe_dbg));
		this->idex_dbg->findChild(DBG_IDEX_MEMRE)->setValue(priscas_io::StrTypes::BoolToStr(de_memre_dbg));

		this->exmem_dbg->findChild(this->DBG_EXMEM_DATA_ALU)->setValue(em_dalu_dbg.toHexString());
		this->exmem_dbg->findChild(this->DBG_EXMEM_DATA_RS)->setValue(em_rs_data_dbg.toHexString());
		this->exmem_dbg->findChild(this->DBG_EXMEM_DATA_RT)->setValue(em_rt_data_dbg.toHexString());
		this->exmem_dbg->findChild(this->DBG_EXMEM_OPCODE)->setValue(em_op_dbg.toHexString());
		this->exmem_dbg->findChild(this->DBG_EXMEM_REGWE)->setValue(priscas_io::StrTypes::BoolToStr(em_regwe_dbg));
		this->exmem_dbg->findChild(this->DBG_EXMEM_MEMWE)->setValue(priscas_io::StrTypes::BoolToStr(em_memwe_dbg));
		this->exmem_dbg->findChild(this->DBG_EXMEM_MEMRE)->setValue(priscas_io::StrTypes::BoolToStr(em_memre_dbg));
		this->exmem_dbg->findChild(DBG_EXMEM_RS_N)->setValue(priscas_io::StrTypes::Int32ToStr(em_rs_dbg));
		this->exmem_dbg->findChild(DBG_EXMEM_RT_N)->setValue(priscas_io::StrTypes::Int32ToStr(em_rt_dbg));
		this->exmem_dbg->findChild(DBG_EXMEM_RD_N)->setValue(priscas_io::StrTypes::Int32ToStr(em_rd_dbg));

		this->memwb_dbg->findChild(DBG_MEMWB_TARGET_REG)->setValue(priscas_io::StrTypes::Int32ToStr(db_mwRNum));
		this->memwb_dbg->findChild(DBG_MEMWB_REGWE)->setValue(priscas_io::StrTypes::BoolToStr(wb_regWE));
		this->memwb_dbg->findChild(DBG_MEMWB_WRITE_DATA)->setValue(db_mwdata.toHexString());

		this->current_cycle_num++;

		return true;
	}

	void fsp_cpu::rst()
	{
		sc_cpu::rst();
		this->fetch_plr.set_data(0);
		this->de_plr.load(0,0,static_cast<funct>(0),0,0,static_cast<opcode>(0),0,0,0,0,0,0);
		this->em_plr.load(0,0,0,static_cast<opcode>(0),0,0,0,0,0,0);
		this->mw_plr.load(0,0,0);
	}

	fsp_cpu::fsp_cpu(mmem & m) :
		sc_cpu(m),
		next_sig(0),
		if_sig(-1),
		id_sig(-1),
		ex_sig(-1),
		mem_sig(-1),
		wb_sig(-1),
		current_cycle_num(0),
		name("MIPS-32 FSP"),
		DBG_INSTRUCTION_WORD("Instruction Word"),
		DBG_MEMWB_REGWE("Register Write Enable (RegWE)"),
		DBG_MEMWB_WRITE_DATA("Write Data"),
		DBG_MEMWB_TARGET_REG("Target Register Number"),
		DBG_IDEX_DATA_RS("Value in Register Rs"),
		DBG_IDEX_DATA_RT("Value in Register Rt"),
		DBG_IDEX_RS_N("Rs"),
		DBG_IDEX_RT_N("Rt"),
		DBG_IDEX_RD_N("Rd"),
		DBG_IDEX_FUNCT("Funct"),
		DBG_IDEX_SHAMT("Shamt or JAL Store"),
		DBG_IDEX_IMM("Imm"),
		DBG_IDEX_OP("Opcode"),
		DBG_IDEX_REGWE("RegWE"),
		DBG_IDEX_MEMWE("MemWE"),
		DBG_IDEX_MEMRE("MemRE"),
		DBG_EXMEM_DATA_ALU("ALU Data"),
		DBG_EXMEM_DATA_RS("Data Rs"),
		DBG_EXMEM_DATA_RT("Data Rt"),
		DBG_EXMEM_OPCODE("Opcode"),
		DBG_EXMEM_REGWE("RegWE"),
		DBG_EXMEM_MEMWE("MemWE"),
		DBG_EXMEM_MEMRE("MemRE"),
		DBG_EXMEM_RS_N("Rs"),
		DBG_EXMEM_RT_N("Rt"),
		DBG_EXMEM_RD_N("Rd")
	{

		// Set clock period
		sc_cpu::clk_T = 40000;

		// Register CPU Options

		// Mem-to-Mem forwarding selection
		CPU_Option fsp_MEMMEM(FSP_Options::getName_MEM_TO_MEM(), "Specify mem-mem hazard detection behavior.");
		fsp_MEMMEM.add_Value(FSP_Options::value_FORWARD(), FSP_Options::valueRaw_FORWARD());
		fsp_MEMMEM.add_Value(FSP_Options::value_STALL(), FSP_Options::valueRaw_STALL());
		this->cp.add_Control(fsp_MEMMEM);
		
		UPString fsp_oAluSrc1_desc;
		fsp_oAluSrc1_desc = UPString("Specify Rs ALU input forwarding mux signal.");

		/*// AluSrc1 Signal
		CPU_Option fsp_MUX_AluSrc1(FSP_Options::getName_MUX_AluSrc1(), fsp_oAluSrc1_desc);
		fsp_MUX_AluSrc1.add_Value(FSP_Options::value_AUTO(), FSP_Options::valueRaw_AUTO());
		fsp_MUX_AluSrc1.add_Value(FSP_Options::value_STUCK_FW_MEM(), FSP_Options::valueRaw_STUCK_FW_MEM());
		fsp_MUX_AluSrc1.add_Value(FSP_Options::value_STUCK_FW_EX(), FSP_Options::valueRaw_STUCK_FW_EX());
		fsp_MUX_AluSrc1.add_Value(FSP_Options::value_STUCK_OFF(), FSP_Options::valueRaw_STUCK_OFF());
		this->cp.add_Control(fsp_MUX_AluSrc1);

		// AluSrc2 signal
		CPU_Option fsp_MUX_AluSrc2(FSP_Options::getName_MUX_AluSrc2(), "Specify Rt / Imm input forwarding mux signal.");
		fsp_MUX_AluSrc2.add_Value(FSP_Options::value_AUTO(), 0);
		fsp_MUX_AluSrc2.add_Value(FSP_Options::value_STUCK_FW_MEM(), FSP_Options::valueRaw_STUCK_FW_MEM());
		fsp_MUX_AluSrc2.add_Value(FSP_Options::value_STUCK_FW_EX(), FSP_Options::valueRaw_STUCK_FW_EX());
		fsp_MUX_AluSrc2.add_Value(FSP_Options::value_STUCK_OFF(), FSP_Options::valueRaw_STUCK_OFF());
		this->cp.add_Control(fsp_MUX_AluSrc2);*/

		this->cp.finalize();
		
		DebugTree_Simple_List* pipeline_register_list_dbg = new DebugTree_Simple_List;
		this->pipeline_diagram = new DebugTableStringValue;
		pipeline_register_list_dbg->setName("Pipeline Register Inspector");
		this->ifid_dbg = pipeline_register_list_dbg->newTree("IF/ID Pipeline Register", "");
		this->idex_dbg = pipeline_register_list_dbg->newTree("ID/EX Pipeline Register", "");
		this->exmem_dbg = pipeline_register_list_dbg->newTree("EX/MEM Pipeline Register", "");
		this->memwb_dbg = pipeline_register_list_dbg->newTree("MEM/WB Pipeline Register", "");

		// Add probes for IF/ID
		ifid_dbg->addChild(this->DBG_INSTRUCTION_WORD, "");

		// Add probes for ID/EX
		idex_dbg->addChild(this->DBG_IDEX_DATA_RS, "");
		idex_dbg->addChild(this->DBG_IDEX_DATA_RT, "");
		idex_dbg->addChild(this->DBG_IDEX_RS_N, "");
		idex_dbg->addChild(this->DBG_IDEX_RT_N, "");
		idex_dbg->addChild(this->DBG_IDEX_RD_N, "");
		idex_dbg->addChild(this->DBG_IDEX_FUNCT, "");
		idex_dbg->addChild(this->DBG_IDEX_SHAMT, "");
		idex_dbg->addChild(this->DBG_IDEX_IMM, "");
		idex_dbg->addChild(this->DBG_IDEX_OP, "");
		idex_dbg->addChild(this->DBG_IDEX_REGWE, "");
		idex_dbg->addChild(this->DBG_IDEX_MEMWE, "");
		idex_dbg->addChild(this->DBG_IDEX_MEMRE, "");

		// Add probes for EX/MEM
		exmem_dbg->addChild(this->DBG_EXMEM_DATA_ALU, "");
		exmem_dbg->addChild(this->DBG_EXMEM_DATA_RS, "");
		exmem_dbg->addChild(this->DBG_EXMEM_DATA_RT, "");
		exmem_dbg->addChild(this->DBG_EXMEM_RS_N, "");
		exmem_dbg->addChild(this->DBG_EXMEM_RT_N, "");
		exmem_dbg->addChild(this->DBG_EXMEM_RD_N, "");
		exmem_dbg->addChild(this->DBG_EXMEM_OPCODE, "");
		exmem_dbg->addChild(this->DBG_EXMEM_REGWE, "");
		exmem_dbg->addChild(this->DBG_EXMEM_MEMWE, "");
		exmem_dbg->addChild(this->DBG_EXMEM_MEMRE, "");

		// Add probes for MEM/WB
		memwb_dbg->addChild(this->DBG_MEMWB_REGWE, "");
		memwb_dbg->addChild(this->DBG_MEMWB_TARGET_REG, "");
		memwb_dbg->addChild(this->DBG_MEMWB_WRITE_DATA, "");
		
		this->debug_views.push_back(pipeline_register_list_dbg);
		this->debug_views.push_back(pipeline_diagram);
	}

	fsp_cpu::~fsp_cpu()
	{
		for(size_t w = 0; w < debug_views.size(); w++)
		{
			delete debug_views[w];
		}
	}
}
