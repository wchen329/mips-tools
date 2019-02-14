#ifndef __IMAGEABLE_H__
#define __IMAGEABLE_H__

namespace mips_tools
{
	/* Special abstract imageable class
	 * a class which is essentially a wrapper for a FILE pointer
	 * which is fed an array supplied through construction
	 */
	class imageable
	{
		public:
			virtual void dump(FILE *); // dump the imageable item to some file
	};
}

#endif