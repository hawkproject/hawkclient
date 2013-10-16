#ifndef HAWK_UTIL_H
#define HAWK_UTIL_H

#if !defined(UTIL_EXPORT)
#	include "HawkAtomic.h"
#	include "HawkBase64.h"
#	include "HawkBinCfgFile.h"
#	include "HawkByteOrder.h"
#	include "HawkCallback.h"
#	include "HawkCounter.h"
#	include "HawkDiskFile.h"
#	include "HawkException.h"
#	include "HawkFile.h"
#	include "HawkFileSender.h"
#	include "HawkFolder.h"
#	include "HawkGateSecurity.h"
#	include "HawkHash.h"
#	include "HawkHMACSHA1Security.h"
#	include "HawkIPAddr.h"
#	include "HawkJson.h"
#	include "HawkLogger.h"
#	include "HawkLoggerManager.h"
#	include "HawkMalloc.h"
#	include "HawkManagerBase.h"
#	include "HawkMarshal.h"
#	include "HawkMarshalData.h"
#	include "HawkMath.h"
#	include "HawkMd5.h"
#	include "HawkMemoryFile.h"
#	include "HawkMsg.h"
#	include "HawkMsgListener.h"
#	include "HawkMsgManager.h"
#	include "HawkMsgPump.h"
#	include "HawkMutex.h"
#	include "HawkOctets.h"
#	include "HawkOctetsStream.h"
#	include "HawkOSOperator.h"
#	include "HawkPackage.h"
#	include "HawkParamVector.h"
#	include "HawkProtocol.h"
#	include "HawkProtocolManager.h"
#	include "HawkRand.h"
#	include "HawkRC4Security.h"
#	include "HawkRefCounter.h"
#	include "HawkRWEFds.h"
#	include "HawkScope.h"
#	include "HawkSecurity.h"
#	include "HawkSession.h"
#	include "HawkSHA1Security.h"
#	include "HawkSmartPtr.h"
#	include "HawkSocket.h"
#	include "HawkSocketAddr.h"
#	include "HawkSocketPair.h"
#	include "HawkSqlite.h"
#	include "HawkStdHeader.h"
#	include "HawkStreamCompress.h"
#	include "HawkStringUtil.h"
#	include "HawkSysProtocol.h"
#	include "HawkTask.h"
#	include "HawkThread.h"
#	include "HawkTimerManager.h"
#	include "HawkValueHolder.h"
#	include "HawkXID.h"
#	include "HawkXmlAttribute.h"
#	include "HawkXmlDocument.h"
#	include "HawkXmlElement.h"
#	include "HawkXmlFile.h"
#	include "HawkXorSecurity.h"
#	include "HawkZipReader.h"
#	include "HawkZlib.h"
	using namespace Hawk;
#else
#	include "HawkStdHeader.h"
#endif

namespace Hawk
{
	/************************************************************************/
	/* 应用底层初始化,更新,停止,释放的统一接口封装                          */
	/************************************************************************/
	class UTIL_API HawkUtil
	{
	public:
		//初始化
		static void Init();

		//周期更新
		static void Tick(UInt32 iPeriod);

		//释放资源
		static void Release();
	};
};
#endif
