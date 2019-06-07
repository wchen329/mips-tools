#include "pipeline.h"

namespace mips_tools
{
	bool fsp_cpu::cycle()
	{
		return true;
	}

	fsp_cpu::fsp_cpu(mmem & m) : sc_cpu(m)
	{
		// Fetch
		/* TODO: check for control hazards */
		BW_32 next_inst_addr = pc.get_data();
		BW_32_T bbbni = next_inst_addr;
		BW_32_T next_inst
		(
			this->mem_req_load(bbbni.b_0()),
			this->mem_req_load(bbbni.b_1()),
			this->mem_req_load(bbbni.b_2()),
			this->mem_req_load(bbbni.b_3())
		);

		pc.set_data(pc.get_data() + 4);

		this->fetch_plr.set_data(next_inst.as_BW_32());

		// Decode


		// Execute

		mips_alu<BW_32> alu;
		
		

		// Memory


		// Write-Back


	}
}