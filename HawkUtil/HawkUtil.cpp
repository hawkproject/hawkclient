#include "HawkUtil.h"
#include "HawkOSOperator.h"
#include "HawkLoggerManager.h"
#include "HawkProtocolManager.h"
#include "HawkTimerManager.h"
#include "HawkMsgManager.h"
#include "pthread.h"

#ifdef PLATFORM_WINDOWS
#	include "HawkWin32.h"
#	ifdef _DEBUG
#		pragma comment(lib, "HawkWin32_d.lib")
#	else
#		pragma comment(lib, "HawkWin32.lib")
#	endif
#endif

namespace Hawk
{
	void HawkUtil::Init()
	{
#ifdef PLATFORM_WINDOWS
		HawkWin32::Install();

		//使用pthread的线程和进程装载		
		pthread_win32_process_attach_np();
		pthread_win32_thread_attach_np();

		//网络初始化
		WSADATA sData;
		WSAStartup(MAKEWORD(2, 2),&sData);		

		//设置CRT调试模式
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_REPORT_FLAG|_CRTDBG_LEAK_CHECK_DF);
#else
		struct sigaction sAction;  
		sAction.sa_handler = SIG_IGN;  
		sigaction(SIGPIPE, &sAction, 0); 
#endif		

		//初始化管理器
		HawkLoggerManager::InitInstance();
		HawkMsgManager::InitInstance();
		HawkTimerManager::InitInstance();
		HawkProtocolManager::InitInstance();

		//注册系统协议
		P_ProtocolManager->RegSysProtocol();

		//运行管理器
		HawkLoggerManager::GetInstance()->Start();
		HawkMsgManager::GetInstance()->Start();
		HawkTimerManager::GetInstance()->Start();	
		HawkProtocolManager::GetInstance()->Start();
	}

	void HawkUtil::Tick(UInt32 iPeriod)
	{
		//更新管理器
		HawkProtocolManager::GetInstance()->Tick(iPeriod);
		HawkTimerManager::GetInstance()->Tick(iPeriod);
		HawkMsgManager::GetInstance()->Tick(iPeriod);
		HawkLoggerManager::GetInstance()->Tick(iPeriod);		
	}

	void HawkUtil::Release()
	{
		//释放管理器
		HawkProtocolManager::ExitInstance();	
		HawkTimerManager::ExitInstance();
		HawkMsgManager::ExitInstance();
		HawkLoggerManager::ExitInstance();
		
#ifdef PLATFORM_WINDOWS
		//网络清理
		WSACleanup();

		//使用pthread的线程和进程卸载
		pthread_win32_thread_detach_np();
		pthread_win32_process_detach_np();

		HawkWin32::Uninstall();
#endif
	}
}
