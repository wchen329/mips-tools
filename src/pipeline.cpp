#include "pipeline.h"

namespace mips_tools
{
	bool fsp_cpu::cycle()
	{
		// Write Enable signals for pipeline registers and PC, used for stalling
		bool we_pc = true;
		bool we_plr_mw = true;
		bool we_plr_em = true;
		bool we_plr_de = true;
		bool we_plr_fetch = true;
		BW_32 pc_next = this->pc.get_data();


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

		if(wb_regWE && wb_save_num != 0)
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
			if(mem_data_rs.AsInt32() == wb_save_num) mem_data_rs = wb_save_data;
			if(mem_data_rt.AsInt32() == wb_save_num) mem_data_rt = wb_save_data;
		}

		// Memory operations
		BW_32 lr_bbb(0,0,0,0);
		
		switch(mem_op)
		{
			case LBU:
				lr_bbb = BW_32(0,0,0,this->mem_req_load(mem_dataALU.AsUInt32()));
				break;
			case LHU:
				lr_bbb = BW_32(	this->mem_req_load(mem_dataALU.AsUInt32()),
									this->mem_req_load(mem_dataALU.AsUInt32()),
									0,
									0								);
				break;
			case LW:
				lr_bbb = BW_32(	this->mem_req_load(mem_dataALU.AsUInt32()),
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
			if(ex_rs == wb_save_num) ex_data_rs = wb_save_data;
			if(ex_rt == wb_save_num) ex_data_rt = wb_save_data;
		}

		// EX->EX forwarding
		if(mem_regWE)
		{
			if(r_inst(mem_op))
			{
				// R instructions write to RD, and their results are saved directly
				if(ex_rs == mem_rd && mem_rd != 0) ex_data_rs = mem_dataALU;
				if(ex_rt == mem_rd && mem_rd != 0) ex_data_rt = mem_dataALU;

			}

			else
			{
				// Else, a little trickier
				// If it's a memory operation (LOAD) then the value isn't ready yet. Use a stall instead
				// Otherwise, forward it!
				if(!mem_inst(mem_op))
				{
					if(ex_rs == mem_rt && mem_rt != 0) ex_data_rs = mem_dataALU;
					if(ex_rt == mem_rt && mem_rt != 0) ex_data_rt = mem_dataALU;
				}

				else
				{
					this->flush_em_plr();
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
					case OR:
						ex_aluResult = alu.execute(ALU::OR, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case NOR:
						ex_aluResult = alu.execute(ALU::OR, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case AND: 
						ex_aluResult = alu.execute(ALU::AND, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case SLL:
						ex_aluResult = alu.execute(ALU::SLL, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case SRL:
						ex_aluResult = alu.execute(ALU::SRL, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
						break;
					case SLT:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false) < 0 ? 1 : 0;
						break;
					case SLTU:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs.AsUInt32(), ex_data_rt.AsUInt32(), true) < 0 ? 1 : 0;
						break;
					case SUB:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs.AsInt32(), ex_data_rt.AsInt32(), false);
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
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs.AsUInt32(), ex_imm.AsUInt32(), true) < 0 ? 1 : 0;
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
		if(mem_regWE && jorb_inst(decode_op))
		{
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
				if(!mem_inst(ex_op))
				{
					if(decode_rs == mem_rd && decode_rs != 0) decode_rs_data = mem_dataALU;
					if(decode_rt == mem_rd && decode_rt != 0) decode_rt_data = mem_dataALU;
				}

				else
				{
					we_pc = false;
					we_plr_fetch = false;
					this->flush_em_plr();
				}
			}
		}

		// Check for EX dependency, a REQUIRED stall for branches
		if(ex_regWE && jorb_inst(decode_op))
		{
			if((decode_rs == ex_rt && decode_rs != 0) || (decode_rt == ex_rt && decode_rt != 0))
			{
					we_pc = false;
					we_plr_fetch = false;
					this->flush_em_plr();
			}
		}



		// Execute branch instruction
		bool branch_taken = false;
		BW_32 branch_addr = (decode_imm << 2);
		switch(decode_op)
		{
				case BEQ:
						if(decode_rs_data == decode_rt_data)
						{
							BW_32 curr_pc = this->get_PC();
							pc_next = curr_pc.AsUInt32() + branch_addr.AsUInt32();
							branch_taken = true;
						}

						break;
				case BNE:
						if(decode_rs_data != decode_rt_data)
						{
							BW_32 curr_pc = this->get_PC();
							pc_next = curr_pc.AsUInt32() + branch_addr.AsUInt32();
							branch_taken = true;
						}
						break;
		}

		// Write data to carry on
		bool decode_regWE = reg_write_inst(decode_op, decode_funct);
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

		if(!branch_taken)
			pc_next = pc.get_data().AsUInt32() + 4;
		else
			this->flush_fetch_plr();

		// Basically part of "branch not taken" if not taken

		/* Commit Transactions
		 */
		if(we_plr_fetch)
			this->fetch_plr.set_data(next_inst);
		if(we_plr_de)
			this->de_plr.load(decode_rs_data, decode_rt_data, decode_funct, decode_shamt, decode_imm,
				decode_op, decode_regWE, decode_memWE, decode_memRE, decode_rs, decode_rt, decode_rd);
		if(we_plr_em)
			this->em_plr.load(ex_aluResult, ex_data_rs, ex_data_rt, ex_op, ex_regWE, ex_memWE, ex_memRE, ex_rs, ex_rt, ex_rd);
		if(we_plr_mw)
			this->mw_plr.load(mem_regWriteData, mem_regWE, mem_write_reg_num);
		if(we_pc)
			pc.set_data(pc_next);
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

	fsp_cpu::fsp_cpu(mmem & m) : sc_cpu(m)
	{
		sc_cpu::clk_T = 40000;
		sc_cpu::cpu_opts.push_back(NameDescPair("--IFID", "print out the current instruction at this register"));
		sc_cpu::cpu_opts.push_back(NameDescPair("--IDEX", "print out control signals set at this register"));
		sc_cpu::cpu_opts.push_back(NameDescPair("--EXMEM", "print out control signals set at this register"));
		sc_cpu::cpu_opts.push_back(NameDescPair("--MEMWB", "print out control signals set at this register"));
		sc_cpu::cpu_opts.push_back(NameDescPair("--ASCII", "print out a ASCII representation of the pipeline registers"));
	}

	void fsp_cpu::exec_CPU_option(std::vector<std::string>& args)
	{
		for(size_t s = 1; s < args.size(); s++)
		{
			if(args[s] == "--IFID")
			{
				fprintf(stdout, "Instruction: 0x%x\n", this->fetch_plr.get_data());
			}

			else
			{
				
			}
		}
	}
}
