#ifndef __MEM_H__
#define __MEM_H__

namespace mips_tools
{

	/* A standard memory
	 * That represents anything that any level of the memory hierarchy
	 *
	 * wchen329
	 */
	class mem
	{
		public:
			char operator[](int ind);		// indexing operater into memory
			mem(int addr_size = 10);		// constructor takes, address space bit length as argument
			~mem();

		protected:
				
		private:
			char * data;					// the actual data of memory
			int size;						// size of memory space in bytes
			mem operator=(const mem &);		// copy assignment, disabled
			mem(const mem &);				// copy constructor, disabled
	};

}
#endif