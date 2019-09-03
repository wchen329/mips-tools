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
#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "debug_view_simple.h"
#include "mips.h"
#include "priscas_global.h"
#include "reg_32.h"
#include "reg_pipeline.h"
#include "sc_cpu.h"
#include "streams.h"
namespace priscas
{
	/* A 5-stage (FS)
	 * MIPS R4000 clone pipelined processor (P)
	 *
	 * wchen329
	 */
	class fsp_cpu : public sc_cpu
	{
		public:
			const UPString& getName() { return this->name; }
			bool cycle();
			void rst();
			fsp_cpu(mmem & m);
			std::vector<DebugView*>& get_DebugViews() { return this->debug_views; }
			CPU_ControlPanel& get_CPU_options() { return this->cp; }

			~fsp_cpu();

		private:
			std::vector<DebugView*> debug_views;
			reg_32 fetch_plr;
			decode_ex_plr de_plr;
			ex_mem_plr em_plr;
			mem_wb_plr mw_plr;
			void flush_fetch_plr() { this->fetch_plr.set_data(0); }
			void flush_de_plr() { this->de_plr.load(0,0,static_cast<funct>(0),0,0,static_cast<opcode>(0),0,0,0,0,0,0); }
			void flush_em_plr() { this->em_plr.load(0,0,0,static_cast<opcode>(0),false,false,false,0,0,0); }

			DebugTree_Simple* ifid_dbg;
			DebugTree_Simple* idex_dbg;
			DebugTree_Simple* exmem_dbg;
			DebugTree_Simple* memwb_dbg;
			DebugTableStringValue * pipeline_diagram;
			
			// CPU Control Panel
			CPU_ControlPanel cp;

			// Signatures of Instructions, for pipelining diagrams
			int next_sig;
			int if_sig;
			int id_sig;
			int ex_sig;
			int mem_sig;
			int wb_sig;
			unsigned long long current_cycle_num;

			// Special names
			const UPString name;
			const std::string DBG_INSTRUCTION_WORD;
			const std::string DBG_MEMWB_REGWE;
			const std::string DBG_MEMWB_WRITE_DATA;
			const std::string DBG_MEMWB_TARGET_REG;
			const std::string DBG_IDEX_DATA_RS;
			const std::string DBG_IDEX_DATA_RT;
			const std::string DBG_IDEX_RS_N;
			const std::string DBG_IDEX_RT_N;
			const std::string DBG_IDEX_RD_N;

			const std::string DBG_IDEX_FUNCT;
			const std::string DBG_IDEX_SHAMT;
			const std::string DBG_IDEX_IMM;
			const std::string DBG_IDEX_OP;
			const std::string DBG_IDEX_REGWE;
			const std::string DBG_IDEX_MEMWE;
			const std::string DBG_IDEX_MEMRE;
			const std::string DBG_EXMEM_DATA_ALU;
			const std::string DBG_EXMEM_DATA_RS;
			const std::string DBG_EXMEM_DATA_RT;
			const std::string DBG_EXMEM_OPCODE;
			const std::string DBG_EXMEM_REGWE;
			const std::string DBG_EXMEM_MEMWE;
			const std::string DBG_EXMEM_MEMRE;
			const std::string DBG_EXMEM_RS_N;
			const std::string DBG_EXMEM_RT_N;
			const std::string DBG_EXMEM_RD_N;

			class FSP_Options
			{
				public:
					// Option Names
					static inline UPString getName_MEM_TO_MEM() { return "PATH_MEM_MEM"; }
					static inline UPString getName_MUX_AluSrc1() { return "MUX_AluSrc1"; }
					static inline UPString getName_MUX_AluSrc2() { return "MUX_AluSrc2"; }

					// Option Values
					static inline UPString value_FORWARD() { return "FORWARD"; }
					static inline UPString value_STALL() { return "STALL"; }
					static inline UPString value_AUTO() { return "AUTO"; }
					static inline UPString value_STUCK_FW_MEM() { return "STUCK_FW_MEM"; }
					static inline UPString value_STUCK_FW_EX() { return "STUCK_FW_EX"; }
					static inline UPString value_STUCK_OFF() { return "STUCK_OFF"; }

					// Option Raw Values
					static inline PCPU_OpRawV valueRaw_FORWARD() { return 0; }
					static inline PCPU_OpRawV valueRaw_STALL() { return 1; }
					static inline PCPU_OpRawV valueRaw_AUTO() { return 0; }
					static inline PCPU_OpRawV valueRaw_STUCK_FW_MEM() { return 1; }
					static inline PCPU_OpRawV valueRaw_STUCK_FW_EX() { return 2; }
					static inline PCPU_OpRawV valueRaw_STUCK_OFF() { return 10; }
			};
	};
}

#endif
