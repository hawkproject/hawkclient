#ifndef HAWK_MSGMANAGER_H
#define HAWK_MSGMANAGER_H

#include "HawkMsgPump.h"
#include "HawkManagerBase.h"

namespace Hawk
{
	/************************************************************************/
	/* 消息管理器,在创建和释放消息之前,必须注册左右得消息类型               */
	/************************************************************************/
	class UTIL_API HawkMsgManager : public HawkManagerBase
	{
	protected:
		//构造
		HawkMsgManager();

		//析构
		virtual ~HawkMsgManager();

		//管理器单例申明
		HAKW_SINGLETON_DECL(MsgManager);

		//注册列表
		typedef map<Int32, Int32> MsgRegMap;

	public:
		//注册消息类型
		virtual Bool	 Register(Int32 iMsg);	

		//获取消息
		virtual HawkMsg* GetMsg(Int32 iMsg);

		//释放消息
		virtual Bool     FreeMsg(HawkMsg*& pMsg);

		//获取注册协议ID列表
		virtual UInt32	 GetRegMsgIds(vector<Int32>& vMsgIds);

	public:
		//初始化消息泵
		virtual Bool     InitMsgPump(Bool bThreadSafe = false);

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
		//注册类型列表
		MsgRegMap		m_mReg;

		//注册型消息泵
		HawkMsgPump*	m_pMsgPump;
	};

	#define P_MsgManager  HawkMsgManager::GetInstance()
}
#endif
