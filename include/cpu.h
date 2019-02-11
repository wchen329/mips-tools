#ifndef __CPU_H__
#define __CPU_H__

namespace mips_tools
{

	/* Contains interface information concerning making a
	 * CPU object, abstract class
	 *
	 * wchen329
	 */
	class cpu
	{
		public:
			virtual void rst() = 0;		// zeroes out the processor state
			virtual bool cycle() = 0;	// advance clock period by one cycle 
										// returns a boolean TRUE if HLT has been fetched, FALSE if HLT has not (or an EOF)
			virtual double get_clk_T() = 0; // get clock period in seconds
	};

}
#endif