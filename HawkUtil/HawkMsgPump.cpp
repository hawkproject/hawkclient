#include "HawkMsgPump.h"
#include "HawkMsgManager.h"

namespace Hawk
{
	HawkMsgPump::HawkMsgPump(Bool bThreadSafe) : m_pListenerMutex(0), m_pMsgMutex(0)
	{
		if (bThreadSafe)
		{
			m_pListenerMutex = new HawkMutex;
			m_pMsgMutex      = new HawkMutex;
		}
	}

	HawkMsgPump::~HawkMsgPump()
	{
		RemoveAllListener();
		RemoveAllMsg();

		HAWK_RELEASE(m_pListenerMutex);
		HAWK_RELEASE(m_pMsgMutex);
	}

	Bool HawkMsgPump::AddMsgListener(Int32 iMsg, const HawkMsgListener* pListener)
	{
		HawkAutoMutex(lock, m_pListenerMutex);

		MsgListenerMap::iterator ml_it = m_mMsgListener.find(iMsg);
		if (ml_it == m_mMsgListener.end())
		{
			m_mMsgListener[iMsg] = RegListenerMap();
			ml_it = m_mMsgListener.find(iMsg);
		}

		if (ml_it != m_mMsgListener.end())
		{
			RegListenerMap& mRegListener = ml_it->second;
			if (mRegListener.find((HawkMsgListener*)pListener) == mRegListener.end())
			{
				mRegListener[(HawkMsgListener*)pListener] = (HawkMsgListener*)pListener;
				return true;
			}
		}
		
		return false;
	}

	Bool HawkMsgPump::PostMsg(HawkMsg* pMsg)
	{
		if (pMsg)
		{
			HawkAutoMutex(lock, m_pMsgMutex);

			m_vPumpMsg.push_back(pMsg);

			return true;
		}

		return true;
	}

	Bool HawkMsgPump::SendMsg(HawkMsg* pMsg)
	{
		if (pMsg)
		{
			HawkAutoMutex(lock, m_pListenerMutex);

			MsgListenerMap::iterator ml_it = m_mMsgListener.find(pMsg->Msg);
			if (ml_it != m_mMsgListener.end())
			{
				RegListenerMap::iterator rl_it = ml_it->second.begin();
				for (;rl_it != ml_it->second.end();rl_it++)
				{
					rl_it->second->OnMessage(*pMsg);
				}
			}

			P_MsgManager->FreeMsg(pMsg);
			return true;
		}
		return false;
	}

	Bool HawkMsgPump::DispatchAllMsg()
	{
		HawkAutoMutex(msg_lock,		m_pMsgMutex);
		HawkAutoMutex(listener_lock,	m_pListenerMutex);

		for (Size_t i=0;i<m_vPumpMsg.size();i++)
		{
			MsgListenerMap::iterator ml_it = m_mMsgListener.find(m_vPumpMsg[i]->Msg);
			if (ml_it != m_mMsgListener.end())
			{
				RegListenerMap::iterator rl_it = ml_it->second.begin();
				for (;rl_it != ml_it->second.end();rl_it++)
				{
					rl_it->second->OnMessage(*m_vPumpMsg[i]);
				}
			}
			P_MsgManager->FreeMsg(m_vPumpMsg[i]);
		}
		m_vPumpMsg.clear();

		return true;
	}

	Bool HawkMsgPump::RemoveMsgListener(Int32 iMsg, const HawkMsgListener* pListener)
	{
		HawkAutoMutex(lock, m_pListenerMutex);

		MsgListenerMap::iterator ml_it = m_mMsgListener.find(iMsg);
		if (ml_it != m_mMsgListener.end())
		{
			RegListenerMap::iterator rl_it = ml_it->second.find((HawkMsgListener*)pListener);
			if (rl_it != ml_it->second.end())
			{
				ml_it->second.erase(rl_it);
				return true;
			}
		}

		return false;
	}

	Bool HawkMsgPump::RemoveAllListener()
	{
		HawkAutoMutex(lock, m_pListenerMutex);

		map<Int32, RegListenerMap>::iterator it = m_mMsgListener.begin();
		for (;it != m_mMsgListener.end();it++)
		{
			it->second.clear();
		}
		m_mMsgListener.clear();
		return true;
	}

	Bool HawkMsgPump::RemoveAllMsg()
	{
		HawkAutoMutex(lock, m_pMsgMutex);

		for (Size_t i=0;i<m_vPumpMsg.size();i++)
		{
			P_MsgManager->FreeMsg(m_vPumpMsg[i]);
		}
		m_vPumpMsg.clear();
		return true;
	}
}
