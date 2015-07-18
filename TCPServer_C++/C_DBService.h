#pragma once

#include "Poco/Data/RecordSet.h"
#include "Poco/Data/SessionPool.h"

#include "DBDataModel.h"

interface I_PbMessageConsolePrompter;

struct SourceInfo
{
	tstring szSourceType;
	tstring szSourceId;
	tstring szQueue;
};

class C_DBService
{
	typedef vector<SourceInfo> C_VecSourceList;

public:
	C_DBService(void);
	virtual ~C_DBService(void);

public:
	tstring SelectValue(LPCTSTR pszQueryValue ,  LPCTSTR pszTableName );
	tstring SelectValue(LPCTSTR pszQueryValue ,  LPCTSTR pszTableName ,LPCTSTR pszCondition , LPCTSTR pszValue);
	tstring SelectValueEx(LPCTSTR pszQueryValue ,  LPCTSTR pszTableName ,LPCTSTR pszCondition , LPCTSTR pszValue, LPCTSTR pszCondition2 , LPCTSTR pszValue2);
	BOOL UpdateValue(LPCTSTR pszTableName , LPCTSTR pszTargetColumn,LPCTSTR pszTargetValue, LPCTSTR pszCondition , LPCTSTR pszValue);
	BOOL UpdateValueEx(LPCTSTR pszTableName , LPCTSTR pszTargetColumn,LPCTSTR pszTargetValue, LPCTSTR pszCondition , LPCTSTR pszValue, LPCTSTR pszCondition2, LPCTSTR pszValue2);
	long SelectCount(LPCTSTR pszTableName, LPCTSTR pszCondition, LPCTSTR pszValue);
	
	BOOL CreateOnlineStatisticsRecord(LPCTSTR pszDate);
	BOOL CreateOnlineTimeRecord(LPCTSTR pszDate, LPCTSTR pszMac);

	BOOL SelectChannel(C_DirectoryInfoSet& channelList, LPCTSTR pszTempletId);
	BOOL SelectConntentByParentId(LPCTSTR pszTemplateId,  LPCTSTR pszDirectoryIndex,C_ContentSet& contentset);
	BOOL SelectMacList(StringVector& vecMacList, LPCTSTR pszAreaId);

	void SetMsgConsolePrompter(I_PbMessageConsolePrompter* pMsgConsolePrompter);
	void SetSessionPool(Poco::Data::SessionPool* pSessionPool);

private:
	Poco::Data::SessionPool* m_pSessionPool;
	I_PbMessageConsolePrompter* m_pMsgConsolePrompter;
};

