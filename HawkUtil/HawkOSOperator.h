#ifndef HAWK_OSOPERATOR_H
#define HAWK_OSOPERATOR_H

#include "HawkRefCounter.h"

namespace Hawk
{
	/************************************************************************/
	/* 和操作系统相关函数封装                                               */
	/************************************************************************/
	class UTIL_API HawkOSOperator : public HawkRefCounter
	{
	public:
		//设置工作路径
		static Bool    SetWorkDir(const AString& sDir);

		//获得工作路径
		static AString GetWorkDir();

		//获得系统Tick毫秒数(CPU)
		static UInt32  GetTickCount();

		//获取系统时钟
		static tm      GetSysClock();
	
		//获取系统时钟秒数
		static time_t  GetSysTime();

		//获得时间字符串:2008-08-08 08:08:08
		static AString GetTimeString();

		//Sleep封装
		static void    OSSleep(Int32 iTime);		

		//从文件路径获取文件名
		static AString SplitFileName(const AString& sFile);

		//从文件路径获取文件路径
		static AString SplitFilePath(const AString& sFile);

		//区域设置
		static AString SetLocale(Int32 iCategory, const AString& sLocal);

		//是否存在某文件
		static Bool    ExistFile(const AString& sFile);

		//是否存在某目录
		static Bool    ExistFolder(const AString& sFolder);

		//获得文件大小
		static Int64   GetFileSize(const AString& sFile);

		//计算CRC
		static UInt32  CalcCrc(const UChar* pData, UInt32 iSize, UInt32* pCrc = 0);

		//计算文件CRC
		static UInt32  CalcFileCrc(const AString& sFile);
		
		//删除文件
		static void    OsDeleteFile(const AString& sFile);

		//重命名文件
		static Bool    RenameFile(const AString& sOldName, const AString& sNewName);

		//设置文件可写
		static Bool    SetFileWritable(const AString& sFile);

		//创建目录树
		static Bool    CreateDir(const AString& sDir);		

		//保证文件目录树存在
		static void    MakeSureFilePath(const AString& sFolderPath);

		//保证文件名前缀目录树存在
		static void    MakeSureFileName(const AString& sFilePath);

		//获取内核线程ID
		static UInt32  GetThreadId();

		//获取进程ID
		static UInt32  GetProcessId();

	public:
		//getch封装
		static Int32   WaitKeyboardInput(const AString& sInfo = "", Bool bPrint = true);
	};

	//定义宏为简易函数
	#define HawkSleep(ms)   HawkOSOperator::OSSleep(ms)
	#define HawkSysTime()   HawkOSOperator::GetSysTime()
	#define HawkTickCount() HawkOSOperator::GetTickCount()
}
#endif
