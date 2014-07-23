#include "HawkThread.h"
#include "HawkOSOperator.h"
#include "pthread.h"

namespace Hawk
{
	PVoid hawk_ThreadRoutine(void* pArgs)
	{
		HawkThread* pThread = static_cast<HawkThread*>(pArgs); 
		if (pThread)
			return pThread->RunLoop();

		return 0;
	}

	HawkThread::HawkThread(Bool bWaitTask) : m_pRoutine(0)
	{
		InnerInit();
	}

	HawkThread::HawkThread(HawkTask* pTask) : m_pRoutine(0)
	{
		InnerInit();

		AddTask(pTask);		
	}

	HawkThread::HawkThread(PThreadRoutine pRoutine) : m_pRoutine(pRoutine)
	{
		InnerInit();
	}	

	HawkThread::~HawkThread()
	{
		Close();
		
		if (m_pThread)
		{
			HawkFree(m_pThread);
			m_pThread = 0;
		}
		
		HAWK_RELEASE(m_pMutex);
	}

	Bool HawkThread::InnerInit()
	{
		m_pOwner		= 0;
		m_iId			= 0;
		m_pArgs			= 0;
		m_iPushTaskCnt	= 0;
		m_iPopTaskCnt	= 0;
		m_pThread		= 0;
		m_bRunning		= false;
		m_iState		= STATE_NONE;
		m_pMutex  		= new HawkMutex;

		return true;
	}

	UInt64 HawkThread::GetPushTaskCnt() const
	{
		return m_iPushTaskCnt;
	}

	UInt64 HawkThread::GetPopTaskCnt() const
	{
		return m_iPopTaskCnt;
	}

	UInt32 HawkThread::GetThreadId() const
	{
		return m_iId;
	}

	Bool HawkThread::IsRunning() const
	{
		return m_bRunning;
	}

	Bool HawkThread::IsClosing() const
	{
		return m_iState == STATE_CLOSING;
	}

	void HawkThread::SetOwner(PVoid pPool)
	{
		m_pOwner = pPool;
	}

	PVoid HawkThread::GetOwner()
	{
		return m_pOwner;
	}

	Bool  HawkThread::AddTask(HawkTask* pTask, Bool bFirst)
	{
		if(pTask)
		{		
			pTask->AddRef();
			HawkAutoMutex(lock, m_pMutex);

			if(bFirst)
				m_qTask.push_front(pTask);
			else
				m_qTask.push_back(pTask);

			m_iPushTaskCnt ++;
			return true;
		}
		return false;
	}

	Bool  HawkThread::Start(void* pArgs)
	{
		if(!m_bRunning && !m_pThread)
		{			
			m_pArgs    = pArgs;
			m_bRunning = true;
			m_pThread  = HawkMalloc(sizeof(pthread_t));
			if (pthread_create((pthread_t*)m_pThread, 0, hawk_ThreadRoutine, this) != HAWK_OK)
			{
				m_bRunning = false;
				m_iState   = STATE_CLOSED;
				return false;
			}

			while (m_iState != STATE_RUNNING)
			{
				HawkSleep(DEFAULT_SLEEP);
			}

			return true;
		}
		return false;
	}

	Bool  HawkThread::Close()
	{
		if (m_pThread && m_bRunning) 
		{
			m_bRunning = false;
			m_iState   = STATE_CLOSING;

			//等待线程结束
			pthread_join(*((pthread_t*)m_pThread), 0);

			HawkFree(m_pThread);
			m_pThread = 0;
			m_iId     = 0;
			m_iState  = STATE_CLOSED;
		}

		ClearTaskQueue();
		return true;
	}

	void HawkThread::ClearTaskQueue()
	{
		HawkAutoMutex(lock, m_pMutex);
		TaskQueue::iterator it = m_qTask.begin();
		for (;it!=m_qTask.end();it++)
		{
			HawkTask* pTask = *it;
			if(pTask)
				pTask->Close();
			HAWK_RELEASE(pTask);
		}
		m_qTask.clear();
	}

	void HawkThread::RunTaskQueue()
	{
		while (m_bRunning)
		{
			HawkTask* pTask = 0;
			if (m_qTask.size())
			{
				HawkAutoMutex(lock, m_pMutex);
				if (m_qTask.size())
				{
					pTask = m_qTask.front();
					m_qTask.pop_front();
					m_iPopTaskCnt ++;
				}
			}

			if (pTask)
			{
				pTask->Run(pTask->GetParam());
				pTask->Close();
				HAWK_RELEASE(pTask);
			}
			else
			{
				break;
			}
		}
	}

	PVoid HawkThread::RunLoop()
	{
		m_iId	 = HawkOSOperator::GetThreadId();
		m_iState = STATE_RUNNING;

		if (m_pRoutine)
		{
			return (*m_pRoutine)(m_pArgs);
		}

		while (m_bRunning)
		{
			HawkTask* pTask = 0;
			if (m_qTask.size())
			{
				HawkAutoMutex(lock, m_pMutex);
				if (m_qTask.size())
				{
					pTask = m_qTask.front();
					m_qTask.pop_front();
					m_iPopTaskCnt ++;
				}
			}

			if (pTask)
			{
				pTask->Run(pTask->GetParam());
				pTask->Close();
				HAWK_RELEASE(pTask);
			}
			else
			{
				HawkSleep(DEFAULT_SLEEP);
			}
		}

		return 0;
	}
}
