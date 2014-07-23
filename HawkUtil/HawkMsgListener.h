#ifndef HAWK_MSGLISTENER_H
#define HAWK_MSGLISTENER_H

#include "HawkMsg.h"

namespace Hawk
{
	/************************************************************************/
	/* 消息监听器                                                           */
	/************************************************************************/
	class UTIL_API HawkMsgListener : public HawkRefCounter
	{
	public:
		//构造
		HawkMsgListener();

		//析构
		virtual ~HawkMsgListener();

		//消息注册表
		typedef map<Int32, Int32> RegMsgMap;

	public:
		//注册消息监听
		virtual Bool   RegisterMsg(Int32 iMsg);

		//消息处理
		virtual Bool   OnMessage(const HawkMsg& sMsg);

		//移除消息监听(参数为0表示移除所有)
		virtual Bool   RemoveMsg(Int32 iMsg = 0);

	protected:
		//注册的消息映射
		RegMsgMap  m_mRegMsg;
	};	
}
#endif
