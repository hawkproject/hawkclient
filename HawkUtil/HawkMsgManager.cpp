#include "HawkMsgManager.h"
#include "HawkLoggerManager.h"
#include "HawkOSOperator.h"

namespace Hawk
{
	HAKW_SINGLETON_IMPL(MsgManager);

	HawkMsgManager::HawkMsgManager() : m_pMsgPump(0)
	{
	}

	HawkMsgManager::~HawkMsgManager()
	{		
		m_mReg.clear();
		HAWK_RELEASE(m_pMsgPump);
	}

	Bool HawkMsgManager::Register(Int32 iMsg)
	{
		MsgRegMap::iterator it = m_mReg.find(iMsg);
		HawkAssert(it == m_mReg.end());
		if (it == m_mReg.end())
		{
			m_mReg[iMsg] = iMsg;
			return true;
		}
		return false;
	}

	UInt32 HawkMsgManager::GetRegMsgIds(vector<Int32>& vMsgIds)
	{
		vMsgIds.clear();

		MsgRegMap::iterator it = m_mReg.begin();
		for (;it!=m_mReg.end();it++)
		{
			vMsgIds.push_back(it->first);
		}

		return (UInt32)vMsgIds.size();
	}

	HawkMsg* HawkMsgManager::GetMsg(Int32 iMsg)
	{
		HawkMsg* pMsg = new HawkMsg(iMsg);

		pMsg->SysFlag = HawkMsg::SYS_CREATE;
		pMsg->Time    = HawkOSOperator::GetTickCount();

		return pMsg;
	}

	Bool HawkMsgManager::FreeMsg(HawkMsg*& pMsg)
	{
		if (pMsg)
		{
			if (pMsg->SysFlag & HawkMsg::SYS_CREATE)
			{
				HAWK_RELEASE(pMsg);
			}			
			
			return true;
		}
		return false;
	}

	Bool HawkMsgManager::InitMsgPump(Bool bThreadSafe)
	{
		if (!m_pMsgPump)
		{
			m_pMsgPump = new HawkMsgPump(bThreadSafe);
			return true;
		}
		return false;
	}

	Bool HawkMsgManager::AddMsgListener(Int32 iMsg, const HawkMsgListener* pListener)
	{
		if (m_pMsgPump)
		{
			return m_pMsgPump->AddMsgListener(iMsg, pListener);
		}
		return false;
	}

	Bool HawkMsgManager::PostMsg(HawkMsg* pMsg)
	{
		if (m_pMsgPump)
		{
			return m_pMsgPump->PostMsg(pMsg);
		}
		return false;
	}

	Bool HawkMsgManager::SendMsg(HawkMsg* pMsg)
	{
		if (m_pMsgPump)
		{
			return m_pMsgPump->SendMsg(pMsg);
		}
		return false;
	}

	Bool HawkMsgManager::DispatchAllMsg()
	{
		if (m_pMsgPump)
		{
			return m_pMsgPump->DispatchAllMsg();
		}
		return false;
	}

	Bool HawkMsgManager::RemoveMsgListener(Int32 iMsg, const HawkMsgListener* pListener)
	{
		if (m_pMsgPump)
		{
			return m_pMsgPump->RemoveMsgListener(iMsg, pListener);
		}
		return false;
	}
}
