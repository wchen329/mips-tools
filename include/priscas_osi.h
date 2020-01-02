#ifndef __PRISCAS_OSI_H__
#define __PRISCAS_OSI_H__
#include <memory>
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
		public:
			LINK_DE void lock();
			LINK_DE void unlock();
			LINK_DE mlock();
			LINK_DE ~mlock();

		private:
			OS_lock_t * mutex;
			mlock(const mlock&);
			mlock operator=(const mlock&);
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
			
			friend DWORD WINAPI ThreadEventLoop(void* arg);

			/* Execute()
			 * Run the thread
			 */
			void Execute();

			/* Work
			 * Once the thread starts running, it just goes into work.
			 * PROGRAMMER: to make the thread useful, derive this class and reimplement "Work".
			 */
			virtual void Work () = 0;

			/* InFlight
			 * Returns true if the thread hasn't returned from its "Work" function yet.
			 * If it has (or never started) then it returns false.
			 */
			bool InFlight() { return active; }

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
				thinst(nullptr),
				active(false)
			{
				myself = this;
			}

		protected:
			void SetInFlight(bool state) { this->active; }
			void ActivateThread(UPThread * myself) { this->myself = myself; }

		private:
			OS_thread_t thinst;
			OS_thread_id_t tid;
			UPThread * myself;
			bool active;

			// Disable copy construction
			UPThread(const UPThread&);
			UPThread& operator=(const UPThread&);
	};

	// Managed Pointers for Threads
	typedef std::unique_ptr<UPThread> umUPThread;
	typedef std::shared_ptr<UPThread> mUPThread;

	/* (Threading)
	 * Sleep a thread for the given amount of milliseconds
	 */
	void sleep(int ms);

	/* wait_generic(...)
	 * Wait for a thread to complete.
	 * Templates:
	 *     - Pointer type to thread to accept
	 *     - wait_time (how long to wait before trying again)
	 *
	 * wait_target: the thread  to wait for
	 */
	/*template <class ThreadPtrT, int wait_time>void wait_generic(ThreadPtrT wait_target)
	{
		// This can be optimized a bit, but here is the simplest wait formula.
		
	}*/


	/* Example_UPThread_Spinner
	 * Example thread which just spins
	 */
	class Example_UPThread_Spinner : public UPThread
	{
		void Work()
		{
			// Spin
			while(true);
		}
	};
}

#endif
