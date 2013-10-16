#include "HawkMsgListener.h"
#include "HawkMsgManager.h"

namespace Hawk
{
	HawkMsgListener::HawkMsgListener()
	{
	}

	HawkMsgListener::~HawkMsgListener()
	{
		RemoveMsg();
	}

	Bool  HawkMsgListener::RegisterMsg(Int32 iMsg)
	{
		if (m_mRegMsg.find(iMsg) == m_mRegMsg.end())
		{
			m_mRegMsg[iMsg] = iMsg;
			P_MsgManager->AddMsgListener(iMsg, this);
			return true;
		}		
		return false;
	}

	Bool  HawkMsgListener::OnMessage(const HawkMsg& sMsg)
	{
		if (m_mRegMsg.find(sMsg.Msg) != m_mRegMsg.end())
		{
			return true;
		}
		return false;
	}

	Bool  HawkMsgListener::RemoveMsg(Int32 iMsg)
	{
		if (iMsg)
		{
			RegMsgMap::iterator it = m_mRegMsg.find(iMsg);
			if (it != m_mRegMsg.end())
			{
				P_MsgManager->RemoveMsgListener(iMsg, this);
				m_mRegMsg.erase(it);
				return true;
			}
		}
		else
		{
			RegMsgMap::iterator it = m_mRegMsg.begin();
			for (;it != m_mRegMsg.end();it++)
			{
				P_MsgManager->RemoveMsgListener(it->first, this);
			}
			m_mRegMsg.clear();
			return true;
		}
		return false;
	}
}
