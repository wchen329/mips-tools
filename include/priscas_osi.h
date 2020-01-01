#ifndef __PRISCAS_OSI_H__
#define __PRISCAS_OSI_H__
#include "priscas_global.h"

#ifdef WIN32
	#include <Windows.h>
	typedef CRITICAL_SECTION OS_lock_t;
	typedef HANDLE OS_thread_t; // Handle is obviously a more general type, but it should work as an alias
	typedef DWORD OS_thread_id_t;
#else
	#include <pthread.h>
	#include <unistd.h>
	typedef pthread_mutex_t OS_lock_t;
	typedef pthread_t OS_thread_t
#endif

/* OS Interfacing
 */
namespace priscas_osi
{

	/* mlock
	 * A UNIX and Win32 compatible Mutex / critical section
	 * The Mutex is automatically initialized- no need to initialize it.
	 * It's always unlocked to begin with...
	 */
	class mlock
	{
		private:
			OS_lock_t mutex;
			
		public:
			void lock();
			void unlock();
			mlock();
	};

	#ifdef WIN32
		DWORD WINAPI ThreadEventLoop(void* arg);
	#endif

	/* UPThread
	 * A wrapper around a POSIX or Win32 thread
	 */
	class UPThread
	{
		public:

			/* Execute()
			 * Run the thread
			 */
			void Execute();

			/* Work
			 * Once the thread starts running, it just goes into work.
			 * PROGRAMMER: to make the thread useful, derive this class and reimplement "Work".
			 */
			virtual void Work () {}

			/* Destructor.
			 * Clean up thread resources if required
			 */
			virtual ~UPThread()
			{
				#ifdef WIN32
				if(thinst != nullptr)
					CloseHandle(thinst);
				#endif
			}

			/* Constructor.
			 * Make a new empty instance of a thread.
			 */
			UPThread() :
				thinst(nullptr)  
			{
				myself = this;
			}

		private:
			OS_thread_t thinst;
			OS_thread_id_t tid;
			UPThread * myself;
	};

	/* (Threading)
	 * Sleep a thread for the given amount of milliseconds
	 */
	void sleep(int ms);

}

#endif
