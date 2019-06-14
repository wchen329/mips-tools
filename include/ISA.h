#ifndef __ISA_H__
#define __ISA_H__

/* An abstract class describing
 * an interface for an Instruction Set Architecture
 *
 * wchen329
 */
#include <string>

class ISA
{
	public:

		enum addrmode
		{
			BYTE = 1,
			WORD = 2,
		};

		enum endian
		{
			LITTLE_ENDIAN = 0,
			BIG_ENDIAN = 1
		};

		virtual unsigned get_reg_count() = 0;			// The amount of registers in this architecture
		virtual unsigned get_address_bit_width() = 0;	// bit width of a PHYSICAL address / PC
		virtual std::string get_reg_name(int id) = 0;	// given a register ID give the "friendly name" of the register
		virtual int get_reg_id(std::string&) = 0;		// similarly, get the register ID given the "friendly name" of the register
		virtual int get_register_bit_width(int id) = 0;	// get the bit width of register of ID
		virtual addrmode get_addressing_mode() = 0;		// get addressing mode
		virtual endian get_endian() = 0;				// get endian
};

#endif