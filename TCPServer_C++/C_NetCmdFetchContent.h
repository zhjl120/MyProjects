#pragma once

#include "C_NetCommand.h"

class C_NetCmdFetchContent :	public C_NetCommand
{
public:
	C_NetCmdFetchContent();
	virtual ~C_NetCmdFetchContent(void);

public:
	virtual BOOL HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac);

private:
	BOOL HandleResponse(Poco::Net::StreamSocket& sktClient, C_DBOperate& DBOperate);
	void ContentSort(C_ContentSet& contentset);
	void ReleaseSet(C_ContentSet& contentset);

private:
	tstring m_szDirectoryId;
};

class lessData
{
public:
	bool operator() (Content *lhs, Content *rhs) const   //sort by queue
	{ return lhs->m_szQueue < rhs->m_szQueue; }
};