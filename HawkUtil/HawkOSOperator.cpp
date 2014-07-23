#include "HawkOSOperator.h"
#include "HawkDiskFile.h"
#include "HawkStringUtil.h"
#include "HawkLoggerManager.h"

namespace Hawk
{
	Bool HawkOSOperator::SetWorkDir(const AString& sDir)
	{
		if (_chdir(sDir.c_str()) == HAWK_OK)
			return true;

		return false;
	}

	AString HawkOSOperator::GetWorkDir()
	{
		Char cTmp[1024] = {0};
		_getcwd(cTmp,1024);
		return cTmp;
	}

	UInt32 HawkOSOperator::GetTickCount()
	{
		UInt32 iSysMs = 0;

#ifdef PLATFORM_WINDOWS
		iSysMs = (UInt32)timeGetTime();
#else
		struct timeval curTm; 
		gettimeofday(&curTm, NULL); 
		iSysMs = (Int32)curTm.tv_sec*1000 + curTm.tv_usec/1000;
#endif

		return iSysMs;
	}

	tm HawkOSOperator::GetSysClock()
	{
		time_t lTime;
		time(&lTime);
		tm* pTm = localtime(&lTime);
		return *pTm;
	}

	time_t HawkOSOperator::GetSysTime()
	{
		return time(NULL);
	}

	AString HawkOSOperator::GetTimeString()
	{
		struct tm xTM = GetSysClock();
		Char sTime[128] = {0};
		sprintf(sTime,"%04d-%02d-%02d %02d:%02d:%02d",
			xTM.tm_year + 1900,
			xTM.tm_mon + 1,
			xTM.tm_mday,
			xTM.tm_hour,
			xTM.tm_min,
			xTM.tm_sec);
		return sTime;
	}

	void HawkOSOperator::OSSleep(Int32 iTime)
	{
#ifdef PLATFORM_WINDOWS
		Sleep(iTime);
#else
		usleep(iTime * 1000);
#endif
	}

	AString HawkOSOperator::SplitFileName(const AString& sFile)
	{
		AString sTmpFile = sFile;
		HawkStringUtil::Replace<AString>(sTmpFile,"\\","/");
		Int32 iPos = (Int32)sTmpFile.find_last_of('/');
		if (iPos > 0)
		{
			return sTmpFile.substr(iPos+1,sTmpFile.size()-iPos-1);
		}
		return sTmpFile;
	}

	AString HawkOSOperator::SplitFilePath(const AString& sFile)
	{
		AString sTmpFile = sFile;
		HawkStringUtil::Replace<AString>(sTmpFile,"\\","/");
		Int32 iPos = (Int32)sTmpFile.find_last_of('/');
		if (iPos > 0)
		{
			return sTmpFile.substr(0,iPos+1);
		}
		return sTmpFile;
	}

	AString HawkOSOperator::SetLocale(Int32 iCategory, const AString& sLocal)
	{
		return setlocale(iCategory,sLocal.c_str());
	}

	Bool HawkOSOperator::ExistFile(const AString& sFile)
	{
		struct stat st;
		if(stat(sFile.c_str(),&st) == 0 && (st.st_mode & S_IFREG))
			return true;

		return false;
	}

	Bool HawkOSOperator::ExistFolder(const AString& sFolder)
	{
		struct stat st;
		if(stat(sFolder.c_str(),&st) == 0 && (st.st_mode & S_IFDIR))
			return true;

		return false;
	}

	Int64 HawkOSOperator::GetFileSize(const AString& sFile)
	{
		struct stat st;
		if(stat(sFile.c_str(),&st) == HAWK_OK)
			return st.st_size;
			
		return 0;
	}	

	//采用 AP Hash算法
	UInt32  HawkOSOperator::CalcCrc(const UChar* pData, UInt32 iSize, UInt32* pCrc)
	{
		UInt32 iCrc = 0;
		if (pCrc)
			iCrc = *pCrc;

		for (UInt32 i = 0; i < iSize; i++) 
		{
			iCrc ^= ((i & 1) == 0) ? ((iCrc << 7) ^ pData[i] ^ (iCrc >> 3)) : (~((iCrc << 11) ^ pData[i] ^ (iCrc >> 5)));
		}

		if (pCrc)
			*pCrc = iCrc;

		return iCrc;

		/*Mysql Hash 算法
		UInt32 iCrc = 1, iSeed = 4;
		if (pCrc)
			iCrc = *pCrc;

		for (UInt32 i = 0; i < iSize; i++) 
		{
			iCrc  ^= (((iCrc & 63) + iSeed)*(pData[i]))+ (iCrc << 8);
			iSeed += 3;
			iCrc ^= ((i & 1) == 0) ? ((iCrc << 7) ^ pData[i] ^ (iCrc >> 3)) : (~((iCrc << 11) ^ pData[i] ^ (iCrc >> 5)));
		}

		if (pCrc)
			*pCrc = iCrc;

		return iCrc;		
		*/
	}

	UInt32 HawkOSOperator::CalcFileCrc(const AString& sFile)
	{
		UInt32 iCrc = 0;
		HawkDiskFile diskFile;
		if (diskFile.Open(sFile))
		{
			UChar szData[DEFAULT_SIZE] = {0};
			Int64 iRead = diskFile.Read(szData, DEFAULT_SIZE);
			while (iRead > 0)
			{
				HawkOSOperator::CalcCrc(szData, (UInt32)iRead, &iCrc);
				iRead = diskFile.Read(szData, DEFAULT_SIZE);
			}
			diskFile.Close();
		}
		return iCrc;
	}
	
	void HawkOSOperator::OsDeleteFile(const AString& sFile)
	{
		remove(sFile.c_str());
	}

	Bool HawkOSOperator::RenameFile(const AString& sOldName,const AString& sNewName)
	{
		Int32 iRet = rename(sOldName.c_str(),sNewName.c_str());

		return iRet == HAWK_OK;
	}

	Bool HawkOSOperator::SetFileWritable(const AString& sFile)
	{
#ifdef PLATFORM_WINDOWS
		_chmod(sFile.c_str(), _S_IREAD | _S_IWRITE);
#else
		chmod(sFile.c_str(), 0666);
#endif
		return true;
	}

	Bool HawkOSOperator::CreateDir(const AString& sDir)
	{
		if (!sDir.size())
			return true;

		AString sSysDir = sDir;
		HawkStringUtil::Replace<AString>(sSysDir,"\\","/");

		Size_t iPos = sSysDir.rfind('/');
		if (iPos != AString::npos)
		{
			AString sParent = sSysDir.substr(0, iPos);
			if (sParent.size() && !ExistFolder(sParent))
				CreateDir(sParent);
		}

#ifdef PLATFORM_WINDOWS
		if (_mkdir(sSysDir.c_str()) != HAWK_OK && errno != EEXIST)
			return false;
#else
		mode_t iMask = umask(0);
		if (mkdir(sSysDir.c_str(), S_IRWXU|S_IRWXG|S_IXOTH|S_IROTH) != HAWK_OK && errno != EEXIST)
		{
			umask(iMask);
			return false;
		}
		umask(iMask);
#endif

		return true;
	}	

	void HawkOSOperator::MakeSureFilePath(const AString& sFolderPath)
	{
		CreateDir(sFolderPath);
	}

	void  HawkOSOperator::MakeSureFileName(const AString& sFilePath)
	{
		AString sFolderPath = SplitFilePath(sFilePath);
		CreateDir(sFolderPath);
	}

	UInt32  HawkOSOperator::GetThreadId()
	{
#ifdef PLATFORM_WINDOWS
		return (UInt32)GetCurrentThreadId();
#else
		return (UInt32)syscall(__NR_gettid);
#endif
	}

	UInt32  HawkOSOperator::GetProcessId()
	{
#ifdef PLATFORM_WINDOWS
		return (UInt32)GetCurrentProcessId();
#else
		return (UInt32)getpid();
#endif
	}

	Int32  HawkOSOperator::WaitKeyboardInput(const AString& sInfo, Bool bPrint)
	{
		if (bPrint)
		{
			if(sInfo.size())
				printf("%s", sInfo.c_str());
			else
				printf("Wait Keyboard Input Event......");
		}		

#ifdef PLATFORM_WINDOWS
		Int32 iRet = _getch();
#else
		Int32 iRet = getchar();
#endif
		return iRet - 32;		
	}
}
