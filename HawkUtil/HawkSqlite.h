#ifndef HAWK_SQLITE_H
#define HAWK_SQLITE_H

#include "HawkRefCounter.h"

namespace Hawk
{
	//////////////////////////////////////////////////////////////////////////
	//Sqlite3数据库操作封装,默认采用Utf8编码								//
	//////////////////////////////////////////////////////////////////////////
	class UTIL_API HawkSqlite : public HawkRefCounter
	{
	public:
		//DB对象构造
		HawkSqlite();

		//DB对象析构
		virtual ~HawkSqlite();

	public:
		//数据库名称
		DEFINE_PROPERTY(AString, m_sDBName,	  DBName);
		//数据库句柄
		DEFINE_PROPERTY(PVoid,   m_pDBHandle, DBHandle);
		//错误信息
		DEFINE_PROPERTY(UString, m_sErrMsg,   ErrMsg);
		//错误码
		DEFINE_PROPERTY(Int32,   m_iErrCode,  ErrCode);
		//是否在事物中
		DEFINE_PROPERTY(Bool,    m_bTrans,    IsTrans);

	public:
		//DB查询结果集
		class UTIL_API QueryResult : public HawkRefCounter
		{
		public:
			//构造
			QueryResult();

			//析构
			virtual ~QueryResult();

			//友员类申明
			friend class HawkSqlite;

		public:
			//结果行数
			DEFINE_PROPERTY(UInt64, m_iRow,	Row);
			//结果列数
			DEFINE_PROPERTY(UInt64, m_iCol,	Col);
			//结果集句柄
			DEFINE_PROPERTY(PVoid,  m_pRes, Res);

		public:
			//根据行列获取返回结果
			const Utf8* GetResult(UInt64 iRow, UInt64 iCol, UInt32* pResLen = 0) const;			

			//根据索引获取返回结果
			const Utf8* operator[] (UInt64 iIdx) const;

			//操作符的行列访问
			const Utf8* operator()(UInt64 iRow, UInt64 iCol) const;			
		};

	public:
		//打开数据库
		virtual Bool	Open(const AString& sDBName);

		//关闭数据库
		virtual Bool	Close();		

	public:		
		//查询操作
		virtual Bool	Select(const UString& sSql, QueryResult& sResult);

		//插入操作
		virtual Bool	Insert(const UString& sSql, QueryResult& sResult);
		
		//更新操作
		virtual Bool	Update(const UString& sSql, QueryResult& sResult);
		
		//删除操作
		virtual Bool	Delete(const UString& sSql, QueryResult& sResult);

		//数据库增删改查操作
		virtual Int32	SqlExe(const UString& sSql, Char** ppErrMsg);

	public:
		//开始事物处理
		virtual Bool	BeginTrans();

		//回滚事物
		virtual Bool	RollbackTrans();

		//提交事物
		virtual Bool	CommitTrans();

	public:				
		//安全转义sqlite3字符串
		virtual UString AmendString(const UString& sValue);

		//安全转义sqlite3字符串
		virtual AString AmendString(const AString& sValue);
	};
}
#endif
