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
#ifndef __MIPS_CPU_H__
#define __MIPS_CPU_H__
#include "priscas_global.h"
#include "mips.h"
#include "primitives.h"
#include "cpu.h"

namespace priscas
{
	/* MIPS-32 CPU
	 * This is a partial implementation of cpu
	 * which should be added on in further derivations
	 */
	class mips32_cpu : public cpu
	{
		public:
			/* getName()
			 * Return the name of the processor
			 * Return: string
			 *         Processor name as a string
			 */
			const UPString& getName() { return name; }

			/* get_clk_T()
			 * Retrieve clock period of processor (in picoseconds)
			 * Return:	long
			 *			the clock period of the processor in picoseconds
			 */
			long get_clk_T() { return clk_period; };

			/* get_ISA()
			 * Retrieve a reference to the ISA which the processor
			 * implements
			 * The ISA is read only.
			 * Return:	ISA reference
			 *			reference to processors ISA
			 */
			const ISA& get_ISA() { return this->isa; };

			virtual ~mips32_cpu(){};

			mips32_cpu(const UPString& name, long clock_period) :
				clk_period(clock_period),
				name(name)
			{
			}

		private:
			long clk_period;
			UPString name;
			MIPS_32 isa;
	};
}

#endif