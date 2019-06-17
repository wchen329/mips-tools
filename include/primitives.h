#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__
#include <cctype>
#include <string>
/* Various byte long
 * primitives such as a 32-bitlong word
 * or a 64-bitlong one.
 *
 * wchen329
 */

namespace mips_tools
{
	typedef long BW_32;	// bitword 32, will be changed to uint_32 or something of the like

	class BW_32_T
	{
		public:
			char b_0() { return *(w_addr());}
			char b_1() { return *(w_addr() + 1);}
			char b_2() { return *(w_addr() + 2);}
			char b_3() { return *(w_addr() + 3);}
			BW_32_T(BW_32 data) : w(data) {}
			BW_32_T(char b_0, char b_1, char b_2, char b_3);

			BW_32 as_BW_32() { return this-> w; }
		private:
			char * w_addr() { return (char*)&w; }
			BW_32 w;
	};

	/* Just a collection of two strings
	 * Name - the name of this object
	 * Description - description of this object
	 */
	class NameDescPair
	{
		public:
			std::string& getName() { return this->name; }
			std::string& getDescription() { return this->desc; }
			NameDescPair(std::string name_of, std::string desc_of) : name(name_of), desc(desc_of) {}
		private:
			std::string name;
			std::string desc;
	};
}

#endif