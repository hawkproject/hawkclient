#ifndef HAWK_MSGPUMP_H
#define HAWK_MSGPUMP_H

#include "HawkMsgListener.h"
#include "HawkMutex.h"

namespace Hawk
{
	/************************************************************************/
	/* 消息泵处理器                                                         */
	/************************************************************************/
	class UTIL_API HawkMsgPump : public HawkRefCounter
	{
	public:
		//构造
		HawkMsgPump(Bool bThreadSafe = false);

		//析构
		virtual ~HawkMsgPump();

		//消息监听者列表
		typedef map<HawkMsgListener*, HawkMsgListener*> RegListenerMap;

		//消息监听列表
		typedef map<Int32, RegListenerMap>				MsgListenerMap;

	public:
		//注册消息监听器
		virtual Bool     AddMsgListener(Int32 iMsg, const HawkMsgListener* pListener);

		//投递消息(非阻塞)
		virtual Bool	 PostMsg(HawkMsg* pMsg);

		//发送消息(阻塞)
		virtual Bool     SendMsg(HawkMsg* pMsg);

		//分发所有消息
		virtual Bool     DispatchAllMsg();

		//移除消息监听器
		virtual Bool     RemoveMsgListener(Int32 iMsg, const HawkMsgListener* pListener);

	protected:
		//清理所有监听器
		virtual Bool     RemoveAllListener();

		//清理所有消息
		virtual Bool     RemoveAllMsg();

	protected:
		//消息监听列表
		MsgListenerMap	  m_mMsgListener;		
		//消息队列
		vector<HawkMsg*>  m_vPumpMsg;
		//监听器锁
		HawkMutex*		  m_pListenerMutex;
		//消息队列
		HawkMutex*		  m_pMsgMutex;
	};	
}
#endif
