#include "osi.h"

#ifdef WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
#endif

namespace osi
{
	void sleep(int ms)
	{
		#ifdef WIN32
			Sleep(ms);
		#else
			usleep(1000*ms);
		#endif
	}
}
