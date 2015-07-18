#pragma once

#include "json.h"
#include "DBDataModel.h"

class C_NetCommandComposer
{
public:
	C_NetCommandComposer(void);
	virtual ~C_NetCommandComposer(void);

public:
	void SetCmdName(LPCTSTR pszCmdName);
	void SetCmdType(LPCTSTR pszCmdType);
	void SetResult(LPCTSTR pszResult);

	void AddContent(LogOnInfo& logOnInfo);
	void AddContent(C_DirectoryInfoSet& channelSet);
	void AddContent(C_ContentSet& contentset, LPCTSTR pszParentId);

	void AddParameters(LPCTSTR pszKey, LPCTSTR pszValue);
	void AppendPara2Root();

	tstring Compose();

private:
	Json::Value m_root;
	Json::Value m_content;
	Json::Value m_item;
	Json::Value m_Param;
	tstring m_szResult;
};

