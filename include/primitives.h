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
#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__
#include <cctype>
#include <cstdint>
#include <memory>
#include <string>
#include "priscas_global.h"
/* Various byte long
 * primitives such as a 32-bitlong word
 * or a 64-bitlong one.
 *
 * wchen329
 */

namespace priscas
{
	typedef unsigned char byte_8b;
	typedef uint8_t bytecount_t;
	typedef uint8_t bitlength_t;

	template<class ConvType, int bitlength> UPString genericHexBuilder(ConvType c)
	{
		UPString ret = "0x";
		int tbl = bitlength;
		
		UPString interm = "";

		while(tbl > 0)
		{
			/* This "mask" is to support
			 * bitword non-aligned with modulo 4
			 */
			char mask =	tbl == 4 ? 0x0F :
						tbl == 3 ? 0x07 :
						tbl == 2 ? 0x03 :
						tbl == 1 ? 0x01 :
						0;

			char val = c & mask;

			switch(val)
			{
				case 0x0:
					interm = "0" + interm;
					break;
				case 0x1:
					interm = "1" + interm;
					break;
				case 0x2:
					interm = "2" + interm;
					break;
				case 0x3:
					interm = "3" + interm;
					break;
				case 0x4:
					interm = "4" + interm;
					break;
				case 0x5:
					interm = "5" + interm;
					break;
				case 0x6:
					interm = "6" + interm;
					break;
				case 0x7:
					interm = "7" + interm;
					break;
				case 0x8:
					interm = "8" + interm;
					break;
				case 0x9:
					interm = "9" + interm;
					break;
				case 0xA:
					interm = "a" + interm;
					break;
				case 0xB:
					interm = "b" + interm;
					break;
				case 0xC:
					interm = "c" + interm;
					break;
				case 0xD:
					interm = "d" + interm;
					break;
				case 0xE:
					interm = "e" + interm;
					break;
				case 0xF:
					interm = "f" + interm;
					break;
			};


			/* If bitwidth is not modulo 4 aligned and less than 4
			 * then we are done already.
			 */
			if(tbl < 4)
			{
				tbl = 0;
			}

			c = (c >> 4);
			tbl -= 4;
		}

		return ret + interm;
	}

	/* BW
	 * generic nth bit bit-word
	 */
	class LINK_DE BW
	{
		public:
			/* toHexString()
			 * IMPLEMENTATION:
			 * Return the hexadecimal representation of this bit word in
			 * a string
			 */
			virtual UPString toHexString() const = 0;

			/* The following are "safe" non-ambiguous casts into
			 * C primitive types
			 */
			virtual const uint8_t& AsUInt8() const = 0;
			virtual uint8_t& AsUInt8() = 0;

			virtual const int8_t& AsInt8() const = 0;
			virtual int8_t& AsInt8() = 0;

			virtual const uint16_t& AsUInt16() const = 0;
			virtual uint16_t& AsUInt16() = 0;
			
			virtual const int16_t& AsInt16() const = 0;
			virtual int16_t& AsInt16() = 0;
			
			virtual const uint32_t& AsUInt32() const = 0;
			virtual uint32_t& AsUInt32() = 0;

			virtual const int32_t& AsInt32() const = 0;
			virtual int32_t& AsInt32() = 0;
			
			virtual const uint64_t& AsUInt64() const = 0;
			virtual uint64_t& AsUInt64() = 0;

			virtual const int64_t& AsInt64() const = 0;
			virtual int64_t& AsInt64() = 0;

			virtual const float& AsSPFloat() const = 0;
			virtual float& AsSPFloat() = 0;

			virtual const double& AsDPFloat() const = 0;
			virtual double& AsDPFloat() = 0;

			/* get_bitlength()
			 * IMPLEMENTATION: Returns the bitlength of a bitword
			 */
			virtual bitlength_t get_bitlength() const = 0;

			/* get_bytecount()
			 * IMPLEMENTATION: Returns the amount of bytes this BW consists of
			 * If the amount of bits is is not modulo 8 aligned, this will always round down.
			 */
			bytecount_t get_bytecount() { return get_bitlength() / 8; }

			/* get_ByteN(...)
			 * Get the nth byte of this (little endian)
			 */
			virtual byte_8b get_ByteN(ptrdiff_t n) const = 0;

			/* set_ByteN(...)
			 * Get the nth byte of this (little endian)
			 */
			void set_ByteN(byte_8b writable, ptrdiff_t n);

			/* == (equality)
			 * IMPLEMENTATION:
			 * Two BWs are considered equal iff
			 * - the two BWs have the same valid BITLENGTH
			 * - the two BWs have matching internal bit patterns, for its valid bits
			 */
			virtual bool operator==(const BW& bw2) const = 0;
			
			/* != (not-equal)
			 * Not equals.
			 */
			bool operator!=(const BW& bw2) const { return !(*this == bw2); }
	};

	/* BW_generic (Little Endian)
	 * n-bit BW
	 * This BW simply uses truncation to values < 64 bits
	 * Converting from a larger-bit number to a smaller one is considered lossy-
	 * there is no guarentee properties such as equality hold after such an operation happens.
	 */
	template<int bitlength> class LINK_DE BW_generic: public BW
	{
		public:

			BW_generic() { w.i64 = 0; }
			BW_generic(int32_t data){ w.i64 = data; }
			BW_generic(uint32_t data) { w.ui64 = data; }
			BW_generic(float data) { w.fp32 = data; }
			BW_generic(double data) { w.fp64 = data; }

			UPString toHexString() const { return genericHexBuilder<int64_t, bitlength>(this->w.i32); }

			/* The following are "safe" non-ambiguous casts into
			 * C primitive types
			 */
			const uint8_t& AsUInt8() const { return w.ui8; }
			uint8_t& AsUInt8() { return w.ui8; }

			const int8_t& AsInt8() const { return w.i8; }
			int8_t& AsInt8() { return w.i8; }

			const uint16_t& AsUInt16() const { return w.ui16; }
			uint16_t& AsUInt16() { return w.ui16; }
			
			const int16_t& AsInt16() const { return w.i16; }
			int16_t& AsInt16() { return w.i16; }
			
			const uint32_t& AsUInt32() const { return w.ui32; }
			uint32_t& AsUInt32() { return w.ui32; }

			const int32_t& AsInt32() const { return w.i32; }
			int32_t& AsInt32() { return w.i32; }
			
			const uint64_t& AsUInt64() const { return w.ui64; }
			uint64_t& AsUInt64() { return w.ui64; }

			const int64_t& AsInt64() const { return w.i64; }
			int64_t& AsInt64() { return w.i64; }

			const float& AsSPFloat() const { return w.fp32; }
			float& AsSPFloat() { return w.fp32; }

			const double& AsDPFloat() const { return w.fp64; }
			double& AsDPFloat() { return w.fp64; }

			/* TODO:
			 * These will be modified when > 64 bitlength is supported
			 */
			BW_generic<bitlength> operator=(const BW& in) {return BW_generic<bitlength>(in.asInt64());}
			BW_generic<bitlength>(const BW& in) { BW_generic<bitlength>(in.AsInt64()); }

			/* get_bitlength()
			 * Return bitlength;
			 */
			bitlength_t get_bitlength() const { return bitlength; }

			/* == (equality)
			 * Two BWs are considered equal iff
			 * - the two BWs have the same valid BITLENGTH
			 * - the two BWs have matching internal bit patterns, for its valid bits
			 */
			virtual bool operator==(const BW& bw2) const
			{
				if(this->get_bitlength() != bw2.get_bitlength())
				{
					return false;
				}

				// Truncate
				uint64_t m1 = (static_cast<uint64_t>(1) << bitlength) - 1;
				uint64_t imm_1 = (m1 & imm_1);

				// Truncate
				uint64_t m2 = (static_cast<uint64_t>(1) << bw2.get_bitlength()) - 1;
				uint64_t imm_2 = (m2 & imm_2);

				if(imm_1 != imm_2)
				{
					return false;
				}

				else
				{
					return true;
				}
			}

			/* get_ByteN(...)
			 * Get the nth byte of this (little endian)
			 */
			byte_8b get_ByteN(ptrdiff_t n) const
			{
				byte_8b * bpt = (byte_8b*)(&w);
				return *(bpt + n);
			}

			/* set_ByteN(...)
			 * Get the nth byte of this (little endian)
			 */
			void set_ByteN(byte_8b writable, ptrdiff_t n)
			{
				byte_8b * bpt = (byte_8b*)(&w);
				*(bpt + n) = writable;
			}

		private:
			
			union BW_prim_internal
			{
				int8_t i8;
				uint8_t ui8;
				int16_t i16;
				uint16_t ui16;
				int32_t i32;
				uint32_t ui32;
				float fp32;
				int64_t i64;
				uint64_t ui64;
				double fp64;
			};

			BW_prim_internal w;
	};

	typedef BW_generic<8> BW_8;
	typedef BW_generic<16> BW_16;
	typedef BW_generic<32> BW_32;
	typedef BW_generic<64> BW_64;

	/* Just a collection of two strings
	 * Name - the name of this object
	 * Description - description of this object
	 */
	class NameDescPair
	{
		public:
			std::string& getName() { return this->name; }
			std::string& getDescription() { return this->desc; }
			const std::string& getName() const { return this->name; }
			const std::string& getDescription() const { return this->desc; }
			NameDescPair(std::string name_of, std::string desc_of) : name(name_of), desc(desc_of) {}
		private:
			std::string name;
			std::string desc;
	};

	/* Just a collection of two strings
	 * Name - the name of this object
	 * Value - the string value of this object
	 */
	class NameValueStringPair
	{
		public:
			std::string& getName() { return this->name; }
			std::string& getValue() { return this->value; }
			NameValueStringPair(std::string name_of, std::string value_of) : name(name_of), value(value_of) {}
		private:
			std::string name;
			std::string value;
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

	
	/* Bit-Word (Managed)
	 * (mBW)
	 */
	typedef std::shared_ptr<BW> mBW;
}

#endif
