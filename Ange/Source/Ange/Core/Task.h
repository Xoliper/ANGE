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
	class Task
	{
	public:
		Task()
		{
			m_iFetchPoint = 0;
			m_bDone = true;
		}

		virtual ~Task()
		{
			Join(true); //Remove this
			if (m_bDone == false) {
				ANGE_FATAL("You destroys Task without ending the working thread!");
				exit(-1000);
			}
		}

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

		void Join(bool terminate)
		{
			if (terminate) Done();
			if(m_Thread.joinable()) m_Thread.join();
		}

		int Available()
		{
			if (m_iFetchPoint >= m_Data.size()) return 0;
			return m_Data.size() - m_iFetchPoint;
		}

		virtual M Make()
		{
			return M();
		};

	protected:

		virtual void EntryPoint()
		{
		}

		F Fetch()
		{
			++m_iFetchPoint;
			typename std::list<F>::iterator it = m_Data.begin();
			std::advance(it, (int)m_iFetchPoint - 1);
			return (*it);
		}

		void Feed(F data)
		{
			m_Data.push_back(data);
		}

		bool IfDone()
		{
			return m_bDone;
		}

		void Done(bool mode = true)
		{
			m_bDone = mode;
		}


		std::list<F> m_Data;

	private:
		std::thread m_Thread;
		size_t m_iFetchPoint;
		bool m_bDone;
	};

	//-----------------------------------------------------------------------
	//Macros
	//-----------------------------------------------------------------------

	#define TASK_START_THREAD(c)	if(IfDone()){ if(m_Thread.joinable()) m_Thread.join(); m_Thread = std::thread(&Task::EntryPoint, this); Done(false); m_iFetchPoint = 0;}
	#define TASK_CONSTRUCTOR(c)		c(){TASK_START_THREAD(c)}
	#define TASK_DESTRUCTOR(c)		~c(){Join(true);}  
	#define TASK_DEFAULT(c)			TASK_CONSTRUCTOR(c) TASK_DESTRUCTOR(c)

}
