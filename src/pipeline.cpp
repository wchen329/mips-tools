#include "pipeline.h"

namespace mips_tools
{
	bool fsp_cpu::cycle()
	{
		/* Execute but do not yet commit transactions
		 *
		 */
		/* FETCH STAGE
		 *
		 *
		 *
		 */
		/* TODO: check for control hazards */
		BW_32 next_inst_addr = pc.get_data();
		BW_32_T next_inst
		(
			this->mem_req_load(next_inst_addr),
			this->mem_req_load(next_inst_addr + 1),
			this->mem_req_load(next_inst_addr + 2),
			this->mem_req_load(next_inst_addr + 3)
		);

		BW_32 pc_next = pc.get_data() + 4;

		/* DECODE STAGE
		 *
		 *
		 *
		 */
		mips_decoding_unit_32 decoding_unit;
		format decode_fm;
		opcode decode_op;
		int decode_rs, decode_rt, decode_rd;
		funct decode_funct;
		BW_32 decode_shamt;
		BW_32 decode_imm;

		decoding_unit.decode(fetch_plr.get_data(), decode_fm, decode_op, decode_rs, decode_rt,
			decode_rd, decode_funct, decode_shamt, decode_imm);

		BW_32 decode_rs_data = this->registers[decode_rs].get_data();
		BW_32 decode_rt_data = this->registers[decode_rt].get_data();
		bool decode_regWE = reg_write_inst(decode_op, decode_funct);
		bool decode_memWE = mem_write_inst(decode_op);
		bool decode_memRE = mem_read_inst(decode_op);

		/* EXECUTE STAGE
		 *
		 *
		 *
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

		mips_alu<BW_32> alu; 

		BW_32 ex_aluResult = 0;

		switch(ex_fm)
		{
			case R:
				
				// Eventually replace these with template functions
				switch(ex_funct)
				{
					case ADD:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs, ex_data_rt, false);
						break;
					case OR:
						ex_aluResult = alu.execute(ALU::OR, ex_data_rs, ex_data_rt, false);
						break;
					case NOR:
						ex_aluResult = ~alu.execute(ALU::OR, ex_data_rs, ex_data_rt, false);
						break;
					case AND: 
						ex_aluResult = alu.execute(ALU::AND, ex_data_rs, ex_data_rt, false);
						break;
					case SLL:
						ex_aluResult = alu.execute(ALU::SLL, ex_data_rs, ex_data_rt, false);
						break;
					case SRL:
						ex_aluResult = alu.execute(ALU::SRL, ex_data_rs, ex_data_rt, false);
						break;
					case SLT:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs, ex_data_rt, false) < 0 ? 1 : 0;
						break;
					case SLTU:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs, ex_data_rt, true) < 0 ? 1 : 0;
						break;
					case SUB:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs, ex_data_rt, false);
						break;
				}

				break;
				
			case I:
				switch(ex_op)
				{
					case ADDI:
						ex_aluResult = alu.execute(ALU::ADD, ex_data_rs, ex_imm, false);
						break;
					case ORI:
						ex_aluResult = alu.execute(ALU::OR, ex_data_rs, ex_imm, false);
						break;;
					case ANDI:
						ex_aluResult = alu.execute(ALU::AND, ex_data_rs, ex_imm, false);
						break;
					case XORI:
						ex_aluResult = alu.execute(ALU::XOR, ex_data_rs, ex_imm, false);
						break;
					case SLTI:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs, ex_imm, false) < 0 ? 1 : 0;
						break;
					case SLTIU:
						ex_aluResult = alu.execute(ALU::SUB, ex_data_rs, ex_imm, true) < 0 ? 1 : 0;
						break;
				}
				break;
		}


		/* MEMORY STAGE
		 *
		 *
		 *
		 */
		BW_32 mem_imm, mem_dataALU;
		opcode mem_op;
		bool mem_regWE, mem_memWE, mem_memRE;
		int mem_rs, mem_rt, mem_rd;
		this->em_plr.get(mem_dataALU, mem_imm, mem_op, mem_regWE, mem_memWE, mem_memRE, mem_rs, mem_rt, mem_rd);
		int mem_write_reg_num = r_inst(mem_op) ? mem_rd : mem_rt;
		BW_32 mem_regWriteData = mem_dataALU;

		/* WRITE-BACK STAGE
		 *
		 *
		 *
		 */
		BW_32 wb_save_data;
		int wb_save_num;
		bool wb_regWE;
		this->mw_plr.get(wb_save_data, wb_regWE, wb_save_num);

		if(wb_regWE)
		{
			this->registers[wb_save_num].set_data(wb_save_data);
		}

		/* Commit Transactions
		 * // TODO: watch for stalls and hazards!
		 */
		pc.set_data(pc_next);
		this->fetch_plr.set_data(next_inst.as_BW_32());
		this->de_plr.load(decode_rs_data, decode_rt_data, decode_funct, decode_shamt, decode_imm,
			decode_op, decode_regWE, decode_memWE, decode_memRE, decode_rs, decode_rt, decode_rd);
		this->em_plr.load(ex_aluResult, ex_imm, ex_op, ex_regWE, ex_memWE, ex_memRE, ex_rs, ex_rt, ex_rd);
		this->mw_plr.load(mem_regWriteData, mem_regWE, mem_write_reg_num);
		return true;
	}

	void fsp_cpu::rst()
	{
		sc_cpu::rst();
		this->fetch_plr.set_data(0);
		this->de_plr.load(0,0,static_cast<funct>(0),0,0,static_cast<opcode>(0),0,0,0,0,0,0);
		this->em_plr.load(0,0,static_cast<opcode>(0),0,0,0,0,0,0);
		this->mw_plr.load(0,0,0);
	}

	fsp_cpu::fsp_cpu(mmem & m) : sc_cpu(m)
	{

	}

}