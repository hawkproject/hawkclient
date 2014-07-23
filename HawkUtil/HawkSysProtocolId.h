#ifndef __HAWKSYSPROTOCOLID_H__
#define __HAWKSYSPROTOCOLID_H__

#include "HawkUtil.h"

namespace Hawk
{
	namespace SysProtocol
	{
		enum
		{
			//加密方式
			SYS_SECURITY = 1,
			//选服务器
			SYS_SEL_SERVER = 2,
			//会话开始
			SYS_SESSION_START = 3,
			//拒绝连接
			SYS_REFUSE_CONN = 4,
			//会话断开
			SYS_SESSION_BREAK = 5,
			//会话关闭
			SYS_CLOSE_SESSION = 6,
			//Ping操作
			SYS_CLT_PING = 7,
			//Pong操作
			SYS_SVR_PONG = 8,
			//字节数组
			SYS_OCTETS = 9,
			//会话心跳
			SYS_HEART_BEAT = 10,
			//心跳超时
			SYS_HEART_BREAK = 11,
			//消息通知
			SYS_MSG_NOTIFY = 12,
			//错误通知
			SYS_ERR_NOTIFY = 13,
			//日志消息
			SYS_LOG_MSG = 14,
			//性能请求
			SYS_PROF_REQ = 15,
			//性能信息
			SYS_PROF_INFO = 16,
		};
	}
}
#endif
