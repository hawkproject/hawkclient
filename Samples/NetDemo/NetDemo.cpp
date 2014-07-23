#include "HawkUtil.h"

UInt32	glb_ThreadNum	= 1;
Bool	glb_Security	= false;
Bool	glb_CltRunning	= true;
AString glb_SvrAddr		= "";

void* CltThreadProc(void* pArgs)
{
	HawkSession* pSession = new HawkSession;

	if (glb_Security)
	{
		HawkGateSecurity* pISecurity = new HawkGateSecurity(HawkGateSecurity::SECURITY_DECODE);
		HawkGateSecurity* pOSecurity = new HawkGateSecurity(HawkGateSecurity::SECURITY_ENCODE);
		pSession->SetISecurity(pISecurity);
		pSession->SetOSecurity(pOSecurity);
		HAWK_RELEASE(pISecurity);
		HAWK_RELEASE(pOSecurity);
	}

	if (pSession->Init(SocketAddr(glb_SvrAddr)))
	{
		UInt32 iLastTime = 0;
		while (glb_CltRunning)
		{
			//发包频率控制
			UInt32 iCurTime = HawkOSOperator::GetTickCount();
			if (iCurTime - iLastTime < 200)
			{
				HawkSleep(200 + iLastTime - iCurTime);
			}
			iLastTime = HawkOSOperator::GetTickCount();

			SysProtocol::Sys_Octets sCmd;
			AString sTxt = HawkStringUtil::RandomString<AString>(HawkRand::RandInt(32, 128));
			sCmd.m_sOctets.Replace(sTxt.c_str(), sTxt.size());

			if (!pSession->Tick(DEFAULT_SLEEP))
			{
				HawkFmtPrint("Session Disconnect");
				break;
			}

			Protocol* pProto = 0;
			while (pSession->DecodeProtocol(pProto) && pProto)
			{
#ifdef _DEBUG
				HawkFmtPrint("Recv Proto, Type: %u, Size: %u, Crc: %u", pProto->GetType(),pProto->GetSize(), pProto->GetCrc());
#endif
				P_ProtocolManager->ReleaseProto(pProto);
			}

			if (!pSession->SendProtocol(&sCmd, true))
			{
				HawkFmtPrint("Session Exception");
				break;
			}
		}
	}
	HAWK_RELEASE(pSession);

	return 0;
}

void PostFile(const AString& sFile)
{
	HawkFileSender sSender;
	sSender.PostFile("127.0.0.1:8080", "ProtocolGen.cpp", sFile);
}

int main(int argc, Char* argv[])
{
	HawkUtil::Init();

	//PostFile("E:/HawkProtocolGen.cpp");

	vector<HawkThread*> vThreads;
	if (argc >= 3)
	{
		glb_SvrAddr   = argv[1];
		glb_ThreadNum = HawkStringUtil::StringToUInt<AString>(argv[2]);

		if (argc >= 4)
		{
			glb_Security = HawkStringUtil::StringToBool<AString>(argv[3]);
		}

		for (Size_t i=0;i<glb_ThreadNum;i++)
		{
			HawkThread* pThread = new HawkThread(CltThreadProc);
			vThreads.push_back(pThread);
			pThread->Start();
		}
	}
	else
	{
		HawkFmtPrint("usage: clientdemo <ip:port> <threads> <security>.");
	}	

	while(HawkOSOperator::WaitKeyboardInput("",false) != 'Q');

	glb_CltRunning = false;
	for (Size_t i=0;i<glb_ThreadNum;i++)
	{
		vThreads[i]->Close();
		HAWK_RELEASE(vThreads[i]);
	}
	vThreads.clear();

	HawkUtil::Release();
	return 0;
}
