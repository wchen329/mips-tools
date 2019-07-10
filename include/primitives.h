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
	template<class ConvType, int bitlength> std::string genericHexBuilder(ConvType c)
	{
		std::string ret = "0x";
		int tbl = bitlength;
		
		std::string interm = "";

		while(tbl > 0)
		{
			char val = c & 0x0F;

			switch(val)
			{
				case 0:
					interm = "0" + interm;
					break;
				case 1:
					interm = "1" + interm;
					break;
				case 2:
					interm = "2" + interm;
					break;
				case 3:
					interm = "3" + interm;
					break;
				case 4:
					interm = "4" + interm;
					break;
				case 5:
					interm = "5" + interm;
					break;
				case 6:
					interm = "6" + interm;
					break;
				case 7:
					interm = "7" + interm;
					break;
				case 8:
					interm = "8" + interm;
					break;
				case 9:
					interm = "9" + interm;
					break;
				case 10:
					interm = "a" + interm;
					break;
				case 11:
					interm = "b" + interm;
					break;
				case 12:
					interm = "c" + interm;
					break;
				case 13:
					interm = "d" + interm;
					break;
				case 14:
					interm = "e" + interm;
					break;
				case 15:
					interm = "f" + interm;
					break;
			};


			c = (c >> 4);
			tbl -= 4;
		}

		return ret + interm;
	}

	// general "bit word class"
	class BW
	{
		public:
			virtual std::string toHexString() = 0;
			virtual int32_t& AsInt32() = 0;
			virtual uint32_t& AsUInt32() = 0;
			virtual float& AsSPFloat() = 0;
			virtual bool operator==(BW& bw2) = 0;
			virtual bool operator!=(BW& bw2) = 0;

	};

	class BW_32 : public BW
	{
		public:
			char b_0() { return *(w_addr());}
			char b_1() { return *(w_addr() + 1);}
			char b_2() { return *(w_addr() + 2);}
			char b_3() { return *(w_addr() + 3);}
			BW_32() { w.i32 = 0; }
			BW_32(int32_t data){ w.i32 = data; }
			BW_32(uint32_t data) { w.ui32 = data; }
			BW_32(float data) { w.fp32 = data; }
			BW_32(char b_0, char b_1, char b_2, char b_3);

			std::string toHexString() { return genericHexBuilder<int32_t, 32>(this->w.i32); }
			int32_t& AsInt32() { return w.i32; }
			uint32_t& AsUInt32() { return w.ui32; }
			float& AsSPFloat() { return w.fp32; }

			bool operator==(BW& bw2) { return (this->AsInt32() == bw2.AsInt32()); }
			bool operator!=(BW& bw2) { return (this->AsInt32() != bw2.AsInt32()); }

		private:
			char * w_addr() { return (char*)&w.i32; }
			
			union BW_32_internal
			{
				int32_t i32;
				uint32_t ui32;
				float fp32;
			};

			BW_32_internal w;
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
		T_BW32,
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
		/*
		else if(typeid(TC) == typeid(BW_16))
		{
			return T_BW16;
		}*/

		else if(typeid(TC) == typeid(BW_32))
		{
			return T_BW32;
		}
		/*
		else if(typeid(TC) == typeid(BW_64))
		{
			return T_BW64;
		}*/
		else
		{
			return T_OTHER;
		}
	}
}

#endif