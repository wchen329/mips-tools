#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "mips.h"
#include "reg_32.h"
#include "reg_pipeline.h"
#include "sc_cpu.h"
namespace mips_tools
{
	/* A 5-stage (FS)
	 * MIPS R4000 clone pipelined processor (P)
	 *
	 * wchen329
	 */
	class fsp_cpu : public sc_cpu
	{
		public:
			bool cycle();
			fsp_cpu(mmem & m);
		private:
			reg_32 fetch_plr;
			decode_ex_plr de_plr;
			ex_mem_plr em_plr;
			mem_wb_plr mw_plr;
	};
}

#endif