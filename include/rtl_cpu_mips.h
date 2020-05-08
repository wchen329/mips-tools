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
#ifndef __RTL_CPU_MIPS_H__
#define __RTL_CPU_MIPS_H__
#include "mips.h"
#include "mmem.h"
#include "phdl.h"
#include "rtl_cpu_common.h"

/* Provides some very basic RTLBranch facilities
 * for MIPS processors
 */
namespace priscas
{
	/* Single Fetch
	 * Fetches only one instruction per cycle.
	 */
	class mips_single_fetch_unit_32 : public RTLBranch
	{
		public:

			LINK_DE void cycle()
			{
				const BW& fetch_addr = this->get_drivers()[0]->get_Drive_Output();
				BW_32 inst;
				int32_t base_addr = fetch_addr.AsInt32();
				inst.set_ByteN(ma[base_addr], 0);
				inst.set_ByteN(ma[base_addr + 1], 1);
				inst.set_ByteN(ma[base_addr + 2], 2);
				inst.set_ByteN(ma[base_addr + 3], 3);
				this->set_Drive_Output(inst);
			}

			/* Fetch Unit.
			 * Constructs a single 32-bit word by reading the provided main memory.
			 * It needs to connect to the PC so a Register ptr must be given too
			 *
			 * ALTERNATIVELY: a branch predictor can be given instead; PC is just the source address. TODO: flush_unit
			 */
			LINK_DE mips_single_fetch_unit_32(mmem& m, mRegister_32 pc) : ma(m) { this->connect_input(pc.get()); }

		private:
			mmem& ma;
	};
	typedef std::shared_ptr<mips_single_fetch_unit_32> mmips_single_fetch_unit_32;

	/* Decoding unit for MIPS-32
	 *
	 */
	class mips_decoding_unit_32 : public RTLBranch
	{
		public:

			LINK_DE void cycle()
			{
				decode(this->get_drivers()[0]->get_Drive_Output());
			}

			LINK_DE void decode(BW_32 inst_word);

			LINK_DE mips_decoding_unit_32(mmips_single_fetch_unit_32 fu) :
				rs_out(new Node),
				rt_out(new Node),
				rd_out(new Node),
				funct_out(new Node),
				shamt_out(new Node),
				imm_out(new Node),
				RegWrite_out(new Node),
				RegDst_out(new Node),
				ALUSrc_out(new Node),
				ALUOp_out(new Node),
				Branch_out(new Node),
				MemWrite_out(new Node),
				MemRead_out(new Node),
				MemToReg_out(new Node)
			{
				this->connect_input(fu);
				rs_out->connect_input(this);
				rt_out->connect_input(this);
				rd_out->connect_input(this);
				funct_out->connect_input(this);
				shamt_out->connect_input(this);
				imm_out->connect_input(this);
				RegWrite_out->connect_input(this);
				RegDst_out->connect_input(this);
				ALUSrc_out->connect_input(this);
				ALUOp_out->connect_input(this);
				Branch_out->connect_input(this);
				MemWrite_out->connect_input(this);
				MemRead_out->connect_input(this);
				MemToReg_out->connect_input(this);
			}

			LINK_DE mNode get_bus_rs_out() { return this->rs_out; }
			LINK_DE mNode get_bus_rt_out() { return this->rt_out; }
			LINK_DE mNode get_bus_rd_out() { return this->rd_out; }
			LINK_DE mNode get_bus_funct_out() { return this->funct_out; }
			LINK_DE mNode get_bus_shamt_out() { return this->shamt_out; }
			LINK_DE mNode get_bus_imm_out() { return this->imm_out; }
			LINK_DE mNode get_RegWrite_out() { return this->RegWrite_out; }
			LINK_DE mNode get_RegDst_out() { return this->RegDst_out; }
			LINK_DE mNode get_ALUSrc_out() { return this->ALUSrc_out; }
			LINK_DE mNode get_ALUOp_out() { return this->ALUOp_out; }
			LINK_DE mNode get_Branch_out() { return this->Branch_out; }
			LINK_DE mNode get_MemWrite_out() { return this->MemWrite_out; }
			LINK_DE mNode get_MemRead_out() { return this->MemRead_out; }
			LINK_DE mNode get_MemToReg_out() { return this->MemToReg_out; }

		private:
			mNode rs_out;
			mNode rt_out;
			mNode rd_out;
			mNode funct_out;
			mNode shamt_out;
			mNode imm_out;
			mNode RegWrite_out;
			mNode RegDst_out;
			mNode ALUSrc_out;
			mNode ALUOp_out;
			mNode Branch_out;
			mNode MemWrite_out;
			mNode MemRead_out;
			mNode MemToReg_out;
	};

	typedef std::shared_ptr<mips_decoding_unit_32> mmips_decoding_unit_32;

	/* Branch Resolver.
	 * Given the:
	 *    - PCSrc input (0)
	 *    - PC + 4 (input 1)
	 *    - Sign Extended Immediate (input 2)
	 *
	 *    Selects the correct branch signal.
	 */
	class mips_branch_resolver_32 : public RTLBranch
	{
		public:
			void cycle()
			{
				pDrivableList pdr = this->get_drivers();

				BW_32 PCSrc = pdr[0]->get_Drive_Output();
				BW_32 PC_Plus_4 = pdr[1]->get_Drive_Output();
				//BW_32 SignExtendedImm = pdr[2]->get_Drive_Output();

				int32_t PCSrc_i32 = PCSrc.AsInt32();

				BW_32 next_PC =		PC_Plus_4;//PCSrc_i32 == 0 ? PC_Plus_4 :
									//PCSrc_i32 == 1 ? SignExtendedImm.AsInt32() << 2 : PC_Plus_4; 

				this->set_Drive_Output(next_PC);
			}

	};
	
	typedef std::shared_ptr<mips_branch_resolver_32> mmips_branch_resolver_32;


	/* mips_32_alu
	 * It's an ALU.
	 * It takes in
	 * (0) ALUOp - which helps distinguish what type of operation it is performing
	 * (1) Funct - which helps distinguish what type of operation it is performing
	 * (2) Data_1 - the first operand (in non-commutative operations) from register file port 1
	 * (3) Data_2 - the second operand (in non-commutative operations)
	 *              the output of a mux. If 0, then source is from register file port 2
	 *              If 1, then source if the sign extended immediate.
	 *
	 * Outputs
	 * (default bus) - 
	 * (branch_out) - 
	 */
	class mips_alu_32 : public RTLBranch
	{
		public:
			void cycle();

		private:

			// If branch evaluates true, this is 1. If not, this is 0.
			mNode branch_true;

			// Constants
			static const size_t input_ALUOp = 0;
			static const size_t input_Funct = 1;
			static const size_t input_Data_1 = 2;
			static const size_t input_Data_2 = 3;
	};

	typedef std::shared_ptr<mips_alu_32> mmips_alu_32;

	/* mips_32_sc_mem
	 * It's a memory.
	 * It takes in a memory
	 */


	/* mips_32_write_back
	 * Register Write Back
	 * for SC CPU and FSP
	 * takes in two inputs
	 * (1) RegWrite. Write if 1, Don't write if 0
	 * (1) WriteData. The data that is written if RegWrite
	 *
	 * In addition this 
	 */
	class mips_32_write_back : public RTLBranch
	{
		public:
			void cycle();

		private:
	};
}

#endif