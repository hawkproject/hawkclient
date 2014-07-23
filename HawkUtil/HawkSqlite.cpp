#include "HawkSqlite.h"
#include "HawkScope.h"
#include "HawkOSOperator.h"
#include "HawkLoggerManager.h"
#include "sqlite3.h"

namespace Hawk
{
	//////////////////////////////////////////////////////////////////////////

	//非法字符串转换处理: 
	//:		' -> ''
	//:		/ -> //		
	//:		[ -> /[
	//:		] -> /]
	//:		% -> /%
	//:		& -> /&
	//:		_ -> /_
	//:		( -> /(
	//:		) -> /)
	static const Char sqlite3_trans_chars[] = 
	{ 
		'\'','/','[',']','%','&','_','(',')',0 
	};

	Bool is_trans_char(Char cVal)
	{
		Size_t iTrans = ARRAY_SIZE(sqlite3_trans_chars);
		for (Size_t i=0;i<iTrans;i++)
		{
			if (cVal == sqlite3_trans_chars[i])
				return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	//查询结果集
	HawkSqlite::QueryResult::QueryResult()
	{
		m_iRow = 0;
		m_iCol = 0;
		m_pRes = 0;
	}

	HawkSqlite::QueryResult::~QueryResult()
	{
		if (m_pRes)
		{
			//清理结果集
			sqlite3_free_table((Char**)m_pRes);			

			//数据重置
			m_iRow = 0;
			m_iCol = 0;
			m_pRes = 0;
		}
	}	

	const Utf8* HawkSqlite::QueryResult::GetResult(UInt64 iRow,UInt64 iCol,UInt32* pResLen) const
	{
		if (m_pRes && m_iRow && m_iCol)
		{
			A_Exception(iRow >= 0 && iRow < m_iRow);
			A_Exception(iCol >= 0 && iCol < m_iCol);

			//跳过第一行列名
			Utf8** ppResult = (Utf8**)m_pRes + m_iCol;
			return ppResult[iRow * m_iCol + iCol];
		}
		return 0;
	}

	const Utf8* HawkSqlite::QueryResult::operator[] (UInt64 iIdx) const
	{		
		if (m_pRes && m_iRow && m_iCol)
		{
			UInt64 iRow = iIdx / m_iCol;
			UInt64 iCol = iIdx % m_iCol;
			return GetResult(iRow, iCol);
		}
		return 0;
	}

	const Utf8* HawkSqlite::QueryResult::operator()(UInt64 iRow, UInt64 iCol) const
	{
		return GetResult(iRow,iCol);
	}

	//////////////////////////////////////////////////////////////////////////

	HawkSqlite::HawkSqlite()
	{
		m_pDBHandle = 0;		
		m_iErrCode  = 0;		
		m_bTrans    = false;
		m_sDBName.clear();
		m_sErrMsg.clear();
	}

	HawkSqlite::~HawkSqlite()
	{
		Close();
	}

	Bool HawkSqlite::Open(const AString& sDBName)
	{
		if (!sqlite3_threadsafe())
		{
			HawkFmtError("Sqlite3 Cannot Support MultiThread.");
		}

		if (m_pDBHandle)
			sqlite3_close((sqlite3*)m_pDBHandle);

		HawkOSOperator::MakeSureFileName(sDBName);

		Long lHdlAddr = (Long)&m_pDBHandle;
		if(sqlite3_open_v2(sDBName.c_str(), reinterpret_cast<sqlite3**>(lHdlAddr),SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,0) != SQLITE_OK)
			return false;

		return true;
	}

	Bool HawkSqlite::Close()
	{		
		if (m_pDBHandle)
		{
			CommitTrans();

			Int32 iRes = sqlite3_close((sqlite3*)m_pDBHandle);
			if (iRes == SQLITE_OK)
			{
				m_pDBHandle = 0;
				return true;
			}
		}
		return false;
	}

	Bool HawkSqlite::Select(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			Char*  pErrMsg = 0;
			Char** ppData  = 0;
			Int32 iRow     = 0;
			Int32 iCol     = 0;

			if (sqlite3_get_table((sqlite3*)m_pDBHandle, (const Char*)sSql.c_str(), &ppData, &iRow, &iCol, &pErrMsg) != SQLITE_OK)
			{
				m_sErrMsg  = (Utf8*)pErrMsg;
				m_iErrCode = sqlite3_errcode((sqlite3*)m_pDBHandle);
				sqlite3_free(pErrMsg);
				return false;
			}

			sResult.SetRow(iRow);
			sResult.SetCol(iCol);
			sResult.SetRes((void*)ppData);

			return true;
		}
		return false;
	}

	Bool HawkSqlite::Insert(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			Char* pErrMsg = 0;
			if (SqlExe(sSql, &pErrMsg) != SQLITE_OK)
			{
				m_sErrMsg  = (Utf8*)pErrMsg;
				m_iErrCode = sqlite3_errcode((sqlite3*)m_pDBHandle);
				sqlite3_free(pErrMsg);
				return false;
			}

			return true;
		}
		return false;
	}

	Bool HawkSqlite::Update(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			Char* pErrMsg = 0;
			if (SqlExe(sSql, &pErrMsg) != SQLITE_OK)
			{
				m_sErrMsg  = (Utf8*)pErrMsg;
				m_iErrCode = sqlite3_errcode((sqlite3*)m_pDBHandle);
				sqlite3_free(pErrMsg);
				return false;
			}

			return true;
		}
		return false;
	}

	Bool HawkSqlite::Delete(const UString& sSql, QueryResult& sResult)
	{
		if (m_pDBHandle && sSql.size())
		{
			Char* pErrMsg = 0;
			if (SqlExe(sSql, &pErrMsg) != SQLITE_OK)
			{
				m_sErrMsg  = (Utf8*)pErrMsg;
				m_iErrCode = sqlite3_errcode((sqlite3*)m_pDBHandle);
				sqlite3_free(pErrMsg);
				return false;
			}

			return true;
		}
		return false;
	}

	Int32 HawkSqlite::SqlExe(const UString& sSql, Char** ppErrMsg)
	{
		if (!m_pDBHandle)
			return SQLITE_ERROR;

		return sqlite3_exec((sqlite3*)m_pDBHandle, (const Char*)sSql.c_str(), 0, 0, ppErrMsg);
	}

	Bool HawkSqlite::BeginTrans()
	{
		if (m_pDBHandle && !m_bTrans)
		{
			Char* pErrMsg = 0;		
			if (SqlExe((Utf8*)"BEGIN;", &pErrMsg) != SQLITE_OK)
			{
				m_sErrMsg  = (Utf8*)pErrMsg;
				m_iErrCode = sqlite3_errcode((sqlite3*)m_pDBHandle);
				sqlite3_free(pErrMsg);
				return false;
			}

			m_bTrans = true;
			return true;
		}
		return false;		
	}

	Bool HawkSqlite::RollbackTrans()
	{
		if (m_pDBHandle && m_bTrans)
		{
			Char* pErrMsg = 0;
			if (SqlExe((Utf8*)"ROLLBACK;", &pErrMsg) != SQLITE_OK)
			{
				m_sErrMsg  = (Utf8*)pErrMsg;
				m_iErrCode = sqlite3_errcode((sqlite3*)m_pDBHandle);
				sqlite3_free(pErrMsg);
				return false;
			}

			m_bTrans = false;
			return true;
		}
		return false;
	}

	Bool HawkSqlite::CommitTrans()
	{
		if (m_pDBHandle && m_bTrans)
		{
			Char* pErrMsg = 0;
			if (SqlExe((Utf8*)"COMMIT;", &pErrMsg) != SQLITE_OK)
			{
				m_sErrMsg  = (Utf8*)pErrMsg;
				m_iErrCode = sqlite3_errcode((sqlite3*)m_pDBHandle);
				sqlite3_free(pErrMsg);
				return false;
			}
			m_bTrans = false;
			return true;
		}
		return false;
	}

	UString HawkSqlite::AmendString(const UString& sValue)
	{
		AString sRet = AmendString((Char*)sValue.c_str());
		return (Utf8*)sRet.c_str();
	}

	AString HawkSqlite::AmendString(const AString& sValue)
	{
		Int32 iSize = (Int32)sValue.size()*2 + 1;
		Char* pBuf  = (Char*)HawkMalloc(iSize * sizeof(Char));
		memset(pBuf,0,iSize * sizeof(Char));
		HawkScope::MallocPtr scope(pBuf);		
		Char* pPos = pBuf;

		for (Size_t i=0;i<sValue.size();i++)
		{
			if (is_trans_char(sValue[i]))
			{
				if(sValue[i] == '\'')
				{
					*pPos++ = '\'';
					*pPos++ = '\'';

					//跳过后续点
					if (i < sValue.size()-1 && sValue[i+1] == '\'')
						i += 1;					
				}
				else
				{
					if (i > 0 && sValue[i-1] != '/')
						*pPos++ = '/';

					*pPos++ = sValue[i];
				}
			}
			else
			{
				*pPos++ = sValue[i];
			}
		}

		return pBuf;
	}
}
