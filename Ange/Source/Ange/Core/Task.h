#pragma once

#include <thread>
#include <list>
#include "Ange/Core/Logger.h"

namespace Ange {

	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------

	using Nothing = void*;

	template <class F, class M>
	/*!
	A class that helps creating tasks - that is, processes performed in another thread that collect data and then allow
	"pseudo asynchronous" widget's production in the main graphical thread.
	*/
	class Task
	{
	public:

		/*!
		Default constructor.
		*/
		Task()
		{
			m_iFetchPoint = 0;
			m_bDone = true;
		}

		/*!
		Default destructor. It does not destroy the memory allocated by the user. The user have to write his own
		destructor.
		*/
		virtual ~Task()
		{
			Join(true); //Remove this
			if (m_bDone == false) {
				ANGE_FATAL("You destroys Task without ending the working thread!");
				exit(-1000);
			}
		}

		/*!
		Function that starts the task thread.
		*/
		void Run()
		{
			if (IfDone()){
				if (m_Thread.joinable()) m_Thread.join();
				Done(false);
				m_iFetchPoint = 0;
				m_Data.clear();
				m_Thread = std::thread(&Task::EntryPoint, this); 
			}
		}

		/*!
		A function that allows synchronization between threads. It waits for the task to finish, but it can also
		terminate it (if the thread is well implemented).
		*/
		void Join(bool terminate)
		{
			if (terminate) Done();
			if(m_Thread.joinable()) m_Thread.join();
		}

		/*!
		Returns information how many objects can be created from the internal data/buffer.
		*/
		int Available()
		{
			if (m_iFetchPoint >= m_Data.size()) return 0;
			return m_Data.size() - m_iFetchPoint;
		}

		/*!
		Creates a new object (executed at the place of the call - usually the main thread).
		*/
		virtual M Make()
		{
			return M();
		}

	protected:

		/*!
		Thread 'main' function. 
		*/
		virtual void EntryPoint()
		{
		}

		/*!
		A function helpful in implementing the Make() function. Returns the next data packet needed to create
		a specific object.
		*/
		F Fetch()
		{
			++m_iFetchPoint;
			typename std::list<F>::iterator it = m_Data.begin();
			std::advance(it, (int)m_iFetchPoint - 1);
			return (*it);
		}

		/*!
		Adds a data packet to the buffer.
		*/
		void Feed(F data)
		{
			m_Data.push_back(data);
		}

		/*!
		Returns the status of the thread (whether it works or has already end).
		*/
		bool IfDone()
		{
			return m_bDone;
		}

		/*!
		Signals the end of the thread.
		*/
		void Done(bool mode = true)
		{
			m_bDone = mode;
		}

		/* Stores data packets. */
		std::list<F> m_Data;

	private:

		/* Thread handler. */
		std::thread m_Thread;

		/* Stores the current position for Fetch() function. */
		size_t m_iFetchPoint;
		
		/* Flag indicationg the state of the thread. */
		bool m_bDone;
	};

}
