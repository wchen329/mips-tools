#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__
#include <cctype>
#include <cstdint>
#include <string>
/* Various byte long
 * primitives such as a 32-bitlong word
 * or a 64-bitlong one.
 *
 * wchen329
 */

namespace mips_tools
{

	typedef int64_t BW_64;
	typedef int32_t BW_32;
	typedef int16_t BW_16;

	// general "bit word class"
	class BW
	{
		public:
			BW_64 as_BW_64() { return representation; }
			BW_32 as_BW_32() { return (static_cast<BW_32>(representation)); }
			BW_16 as_BW_16() { return (representation & ((1 << 16)  - 1)); }
			BW(BW_64 w) { this->representation = w ; }
			BW(BW_32 w) { this->representation = (0 + w); }
			BW(BW_16 w) { this->representation = (0 + w); }
		private:
			long long representation; // 64 bit representation
	};

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

		enum HighLevelType
	{
		T_NONE,
		T_BW16,
		T_BW32,
		T_BW64,
		T_INT,
		T_INT32,
		T_UINT,
		T_UINT32,
		T_FLOAT,
		T_DOUBLE,
		T_STRING,
		T_OTHER
	};

	template<class TC> HighLevelType getTypeGeneric()
	{
		if(typeid(TC) == typeid(int))
		{
			return T_INT;
		}

		else if(typeid(TC) == typeid(unsigned))
		{
			return T_UINT;
		}
			
		else if(typeid(TC) == typeid(long) || typeid(TC) == typeid(int32_t))
		{
			return T_INT32;
		}

		else if(typeid(TC) == typeid(unsigned long) || typeid(TC) == typeid(uint32_t))
		{
			return T_UINT32;
		}

		else if(typeid(TC) == typeid(float))
		{
			return T_FLOAT;
		}

		else if(typeid(TC) == typeid(double))
		{
			return T_DOUBLE;
		}

		else if(typeid(T_STRING) == typeid(std::string))
		{
			return T_STRING;
		}

		else if(typeid(TC) == typeid(BW_16))
		{
			return T_BW16;
		}

		else if(typeid(TC) == typeid(BW_32))
		{
			return T_BW32;
		}

		else if(typeid(TC) == typeid(BW_64))
		{
			return T_BW64;
		}
	}
}

#endif