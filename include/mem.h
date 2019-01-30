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
			char& operator[](int ind);		// indexing operater into memory
			mem(size_t size = 1024);		// constructor takes, byte size of memory as argument
			size_t get_size(){return size;}	// returns size;
			~mem();
			char * begin() {return data;}	// get beginning address of data range

		protected:
				
		private:
			char * data;					// the actual data of memory
			size_t size;					// size of memory space in bytes
			mem operator=(const mem &);		// copy assignment, disabled
			mem(const mem &);				// copy constructor, disabled
	};

}
#endif