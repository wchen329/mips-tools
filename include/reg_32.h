/* 32 bit register
 */
#include "primitives.h"

namespace mips_tools
{

	class reg_32
	{
		public:
			void set_data(BW_32 data) { this -> data = data; }
			BW_32 get_data() { return this -> data; }

		private:
			BW_32 data;
	};

}