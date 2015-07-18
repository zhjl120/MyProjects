#include "StdAfx.h"
#include "C_DBService.h"

#include "Poco/Data/Common.h"
#include "Poco/Data/Statement.h"
#include "Poco/Data/Session.h"

#include "MessagePrompter/I_PbMessageConsolePrompter.h"
#include "C_ErrorLog.h"

C_DBService::C_DBService(void)
{
}

C_DBService::~C_DBService(void)
{
}

tstring C_DBService::SelectValue( LPCTSTR pszQueryValue , LPCTSTR pszTableName )
{
	Poco::Data::Session sess(m_pSessionPool->get());
	Poco::Data::Statement Querier(sess);

	tstring szReturnValue = "";
	tstring szSQLStr = "SELECT ";
	szSQLStr += pszQueryValue;
	szSQLStr += " FROM ";
	szSQLStr += pszTableName;

	try
	{
		Querier<<szSQLStr.c_str();
		Querier.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (SelectValue): ");
		szErr += szSQLStr;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DATABASE_CONNECTION_1"));
		cout<<"***SQL= "<<szSQLStr<<endl;
		return szReturnValue;
	}	

	Poco::Data::RecordSet QueryResult(Querier);

	BOOL bMore = QueryResult.moveFirst();
	if(bMore)
	{
		szReturnValue = QueryResult[pszQueryValue].convert<std::string>().c_str();
	}

	return szReturnValue;
}

tstring C_DBService::SelectValue( LPCTSTR pszQueryValue , LPCTSTR pszTableName ,LPCTSTR pszCondition , LPCTSTR pszValue )
{
	Poco::Data::Session sess(m_pSessionPool->get());
	Poco::Data::Statement Querier(sess);

	tstring szReturnValue = "";
	tstring szSQLStr = "SELECT ";
	szSQLStr += pszQueryValue;
	szSQLStr += " FROM ";
	szSQLStr += pszTableName;
	szSQLStr += " WHERE ";
	szSQLStr += pszCondition;
	szSQLStr += " = '";
	szSQLStr += pszValue;
	szSQLStr += "'" ;
	
	try
	{
		Querier<<szSQLStr.c_str();
		Querier.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (SelectValue): ");
		szErr += szSQLStr;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DATABASE_CONNECTION_1"));
		cout<<"***SQL= "<<szSQLStr<<endl;
		return szReturnValue;
	}	

	Poco::Data::RecordSet QueryResult(Querier);

	BOOL bMore = QueryResult.moveFirst();
	if(bMore)
	{
		szReturnValue = QueryResult[pszQueryValue].convert<std::string>().c_str();
	}

	return szReturnValue;
}

BOOL C_DBService::UpdateValue( LPCTSTR pszTableName , LPCTSTR pszTargetColumn,
															  LPCTSTR pszTargetValue, LPCTSTR pszCondition , LPCTSTR pszValue )
{
	Poco::Data::Session sess(m_pSessionPool->get());

	Poco::Data::Statement Querier(sess);

	tstring szReturnValue = _T("");
	tstring szSQLStr =	"UPDATE ";
	szSQLStr += pszTableName;
	szSQLStr +=" SET ";
	szSQLStr += pszTargetColumn;
	szSQLStr += " = '";
	szSQLStr += pszTargetValue;
	szSQLStr +="' WHERE ";
	szSQLStr += pszCondition;
	szSQLStr += " = '";
	szSQLStr += pszValue;
	szSQLStr += "'";
	
	try
	{
		Querier<<szSQLStr.c_str();
		Querier.execute();

		if( Querier.done() )
			return TRUE;
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (UpdateValue): ");
		szErr += szSQLStr;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DATABASE_CONNECTION_2"));
		cout<<"***SQL= "<<szSQLStr<<endl;
	}	

	return FALSE;
}

long C_DBService::SelectCount( LPCTSTR pszTableName, LPCTSTR pszCondition, LPCTSTR pszValue )
{
	Poco::Data::Session sess(m_pSessionPool->get());

	Poco::Data::Statement Querier(sess);

	tstring szReturnValue = "";
	tstring szSQLStr = "SELECT COUNT(*) FROM ";
	szSQLStr += pszTableName;
	szSQLStr += " WHERE ";
	szSQLStr += pszCondition;
	szSQLStr +=" = '";
	szSQLStr += pszValue;
	szSQLStr += "'" ;

	try
	{
		Querier<<szSQLStr.c_str();
		Querier.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (SelectCount): ");
		szErr += szSQLStr;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DATABASE_CONNECTION_4"));
		cout<<"***SQL= "<<szSQLStr<<endl;
		return -1;
	}	

	Poco::Data::RecordSet QueryResult(Querier);

	BOOL bMore = QueryResult.moveFirst();
	if(bMore)
	{
		szReturnValue = QueryResult[0].convert<std::string>().c_str();
	}

	long nReturnValue = atoi(szReturnValue.c_str());

	return nReturnValue;
}

BOOL C_DBService::CreateOnlineStatisticsRecord( LPCTSTR pszDate)
{
	Poco::Data::Session sess(m_pSessionPool->get());
	Poco::Data::Statement Querier(sess);

	tstring szSQLStr = "INSERT INTO loginstatistics (Date) VALUES ('";
	szSQLStr += pszDate;
	szSQLStr += "')";

	try
	{
		Querier<<szSQLStr.c_str();
		Querier.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (CreateOnlineStatisticsRecord): ");
		szErr += szSQLStr;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DATABASE_CONNECTION_4"));
		cout<<"***SQL= "<<szSQLStr<<endl;
		return FALSE;
	}	

	return TRUE;
}

BOOL C_DBService::CreateOnlineTimeRecord( LPCTSTR pszDate, LPCTSTR pszMac )
{
	Poco::Data::Session sess(m_pSessionPool->get());
	Poco::Data::Statement Querier(sess);

	tstring szSQLStr = "INSERT INTO onlinetimestatistics (Date,Mac) VALUES ('";
	szSQLStr += pszDate;
	szSQLStr += "','";
	szSQLStr += pszMac;
	szSQLStr += "')";

	try
	{
		Querier<<szSQLStr.c_str();
		Querier.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (CreateOnlineTimeRecord): ");
		szErr += szSQLStr;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DATABASE_CONNECTION_4"));
		cout<<"***SQL= "<<szSQLStr<<endl;
		return FALSE;
	}	

	return TRUE;
}


BOOL C_DBService::SelectChannel( C_DirectoryInfoSet& channelList, LPCTSTR pszTempletId )
{
	Poco::Data::Session sess(m_pSessionPool->get());

	Poco::Data::Statement SetNames(sess);
	Poco::Data::Statement Querier(sess);

	tstring szChannelTable = _T("epg_channel_");
	szChannelTable+= pszTempletId;

	try
	{
		SetNames<<"set names gbk";
		SetNames.execute();
		Querier<<"SELECT * FROM "<<szChannelTable.c_str()<<" WHERE parentID = 0";
		Querier.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (SelectChannel): SELECT * FROM ");
		szErr += szChannelTable;
		szErr +=_T(" WHERE parentID = 0");
		C_ErrorLog::WriteErrLog(szErr.c_str());

		cout<<"Query database error - SelectChannel()"<<endl;
		return FALSE;
	}	

	Poco::Data::RecordSet QueryResult(Querier);

	BOOL bMore = QueryResult.moveFirst();

	while(bMore)
	{
		DirectoryInfo* pDir = new DirectoryInfo;

		pDir->m_szTitle = (QueryResult)["title"].convert<std::string>().c_str();
		pDir->m_szDescription = (QueryResult)["description"].convert<std::string>().c_str();
		pDir->m_szResource = (QueryResult)["resource"].convert<std::string>().c_str();
		pDir->m_szIndex = (QueryResult)["id"].convert<std::string>().c_str();
		pDir->m_szUpdatetime = (QueryResult)["updatetime"].convert<std::string>().c_str();

		channelList.push_back(pDir);
		bMore = QueryResult.moveNext();
	}

	return TRUE;
}

BOOL C_DBService::SelectConntentByParentId(LPCTSTR pszTemplateId, LPCTSTR pszDirectoryIndex, C_ContentSet& contentset)
{
	Poco::Data::Session sess(m_pSessionPool->get());

////////////////////////// Get Directory////////////////////////////////////
	Poco::Data::Statement SetNames(sess);
	Poco::Data::Statement Querier(sess);

	tstring szChannelTableName = _T("epg_channel_");
	szChannelTableName += pszTemplateId;

	tstring szSql = "SELECT * FROM ";
	szSql += szChannelTableName;
	szSql += " WHERE parentID = ";
	szSql += pszDirectoryIndex;

	try
	{
		SetNames<<"set names gbk";
		SetNames.execute();
		Querier<<szSql.c_str();
		Querier.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (SelectConntentByParentId): ");
		szErr += szSql;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		cout<<"Query database error - SelectConntentByParentId()"<<endl;
		return FALSE;
	}	

	Poco::Data::RecordSet QueryResult(Querier);
	BOOL bMore1 = QueryResult.moveFirst();

	while(bMore1)
	{
		Content* pDir = new Content;

		pDir->m_szType = _T("dir");
		pDir->m_szIndex = (QueryResult)["id"].convert<std::string>().c_str();
		pDir->m_szTitle = (QueryResult)["title"].convert<std::string>().c_str();
		pDir->m_szDescription = (QueryResult)["description"].convert<std::string>().c_str();
		pDir->m_szResource = (QueryResult)["resource"].convert<std::string>().c_str();
		pDir->m_szUpdatetime = (QueryResult)["updatetime"].convert<std::string>().c_str();
		pDir->m_szQueue = (QueryResult)["queue"].convert<std::string>().c_str();

		contentset.push_back(pDir);
		bMore1 = QueryResult.moveNext();
	}

////////////////////////// Get File///////////////////////////////////////////
	Poco::Data::Statement SetNames2(sess);
	Poco::Data::Statement Querier2(sess);

	tstring szContentTableName = _T("epg_channelcontent_");
	szContentTableName += pszTemplateId;

	tstring szSQLstr = "SELECT source_type,source_id,queue  FROM ";
	szSQLstr+= szContentTableName.c_str();
	szSQLstr+= " WHERE parentID = ";
	szSQLstr += pszDirectoryIndex;

	try
	{
		SetNames2<<"set names gbk";
		SetNames2.execute();
		Querier2<<szSQLstr.c_str();
		Querier2.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (SelectConntentByParentId): ");
		szErr += szSQLstr;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		cout<<"Query database error - SelectConntentByParentId()"<<endl;
		return FALSE;
	}	

	Poco::Data::RecordSet QueryResult2(Querier2);
	BOOL bMore2 = QueryResult2.moveFirst();

	C_VecSourceList vecSourceInfo;

	while(bMore2)
	{
		SourceInfo src;

		src.szSourceId = (QueryResult2)["source_id"].convert<std::string>().c_str();
		src.szSourceType = (QueryResult2)["source_type"].convert<std::string>().c_str();
		src.szQueue = (QueryResult2)["queue"].convert<std::string>().c_str();
	
		vecSourceInfo.push_back(src);
		bMore2 = QueryResult2.moveNext();
	}

	for (C_VecSourceList::iterator it = vecSourceInfo.begin(); it != vecSourceInfo.end(); ++it)
	{
		Poco::Data::Statement SetNames(sess);
		Poco::Data::Statement Querier(sess);

		tstring szSQLstr = "SELECT * FROM channelcontent_source WHERE type = '";
		szSQLstr +=(*it).szSourceType;
		szSQLstr += "' AND id = ";
		szSQLstr +=(*it).szSourceId;

		try
		{
			SetNames<<"set names gbk";
			SetNames.execute();
			Querier<<szSQLstr.c_str();
			Querier.execute();
		}
		catch(...)
		{
			tstring szErr = _T("SQL Error (SelectConntentByParentId): ");
			szErr += szSQLstr;
			C_ErrorLog::WriteErrLog(szErr.c_str());

			cout<<"Query database error - SelectChannelInfo()"<<endl;
			return FALSE;
		}	

		Poco::Data::RecordSet QueryResult(Querier);
		BOOL bMore3 = QueryResult.moveFirst();

		if(bMore3)
		{
			Content* pfile = new Content;

			pfile->m_szIndex = (QueryResult)["id"].convert<std::string>().c_str();
			pfile->m_szType = (QueryResult)["type"].convert<std::string>().c_str();
			pfile->m_szTitle = (QueryResult)["title"].convert<std::string>().c_str();
			pfile->m_szDescription = (QueryResult)["description"].convert<std::string>().c_str();
			pfile->m_szResource = (QueryResult)["resource"].convert<std::string>().c_str();
			pfile->m_szUrls = (QueryResult)["urls"].convert<std::string>().c_str();
			pfile->m_szQueue = (*it).szQueue;
			pfile->m_szDirecotryId = pszDirectoryIndex;

			contentset.push_back(pfile);
		}
	}

	return TRUE;
}

void C_DBService::SetMsgConsolePrompter( I_PbMessageConsolePrompter* pMsgConsolePrompter )
{
	m_pMsgConsolePrompter = pMsgConsolePrompter;
}

void C_DBService::SetSessionPool( Poco::Data::SessionPool* pSessionPool )
{
	m_pSessionPool = pSessionPool;
}

tstring C_DBService::SelectValueEx( LPCTSTR pszQueryValue , LPCTSTR pszTableName ,	LPCTSTR pszCondition ,
															     LPCTSTR pszValue, LPCTSTR pszCondition2 , LPCTSTR pszValue2 )
{
	Poco::Data::Session sess(m_pSessionPool->get());

	Poco::Data::Statement Querier(sess);

	tstring szReturnValue = "";
	tstring szSQLStr = "SELECT ";
	szSQLStr += pszQueryValue;
	szSQLStr += " FROM ";
	szSQLStr += pszTableName;
	szSQLStr += " WHERE ";
	szSQLStr += pszCondition;
	szSQLStr += " = '";
	szSQLStr += pszValue;
	szSQLStr += "'" ;
	szSQLStr += " AND ";
	szSQLStr += pszCondition2;
	szSQLStr += " = '";
	szSQLStr += pszValue2;
	szSQLStr += "'";

	try
	{
		Querier<<szSQLStr.c_str();
		Querier.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (SelectValueEx): ");
		szErr += szSQLStr;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DATABASE_CONNECTION_1"));
		cout<<"***SQL= "<<szSQLStr<<endl;
		return szReturnValue;
	}	

	Poco::Data::RecordSet QueryResult(Querier);

	BOOL bMore = QueryResult.moveFirst();
	if(bMore)
	{
		szReturnValue = QueryResult[pszQueryValue].convert<std::string>().c_str();
	}

	return szReturnValue;
}

BOOL C_DBService::SelectMacList( StringVector& vecMacList, LPCTSTR pszAreaId )
{
	Poco::Data::Session sess(m_pSessionPool->get());

	Poco::Data::Statement SetNames(sess);
	Poco::Data::Statement Querier(sess);

	try
	{
		SetNames<<"set names gbk";
		SetNames.execute();
		Querier<< "SELECT mac FROM device_information WHERE area_id = "<<pszAreaId;
		Querier.execute();
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (SelectMacList): SELECT mac FROM device_information WHERE area_id = ");
		szErr += pszAreaId;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		cout<<"Query database error - SelectChannel()"<<endl;
		return FALSE;
	}	

	Poco::Data::RecordSet QueryResult(Querier);

	BOOL bMore = QueryResult.moveFirst();

	while(bMore)
	{
		vecMacList.push_back((QueryResult)["mac"].convert<std::string>());
		bMore = QueryResult.moveNext();
	}

	return TRUE;
}

BOOL C_DBService::UpdateValueEx( LPCTSTR pszTableName , LPCTSTR pszTargetColumn,LPCTSTR pszTargetValue,
	LPCTSTR pszCondition , LPCTSTR pszValue, LPCTSTR pszCondition2, LPCTSTR pszValue2 )
{
	Poco::Data::Session sess(m_pSessionPool->get());

	Poco::Data::Statement Querier(sess);

	tstring szReturnValue = _T("");
	tstring szSQLStr =	"UPDATE ";
	szSQLStr += pszTableName;
	szSQLStr +=" SET ";
	szSQLStr += pszTargetColumn;
	szSQLStr += " = '";
	szSQLStr += pszTargetValue;
	szSQLStr +="' WHERE ";
	szSQLStr += pszCondition;
	szSQLStr += " = '";
	szSQLStr += pszValue;
	szSQLStr += "' AND ";
	szSQLStr += pszCondition2;
	szSQLStr += " = '";
	szSQLStr += pszValue2;
	szSQLStr += "'";

	try
	{
		Querier<<szSQLStr.c_str();
		Querier.execute();

		if( Querier.done() )
			return TRUE;
	}
	catch(...)
	{
		tstring szErr = _T("SQL Error (UpdateValue): ");
		szErr += szSQLStr;
		C_ErrorLog::WriteErrLog(szErr.c_str());

		m_pMsgConsolePrompter->PromptMessageLineBy(_T("DATABASE_CONNECTION_2"));
		cout<<"***SQL= "<<szSQLStr<<endl;
	}	

	return FALSE;
}

