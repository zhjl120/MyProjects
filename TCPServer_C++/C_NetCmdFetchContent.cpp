#include "StdAfx.h"
#include <algorithm>
#include "C_NetCmdFetchContent.h"
#include "Poco\Net\StreamSocket.h"

C_NetCmdFetchContent::C_NetCmdFetchContent():	m_szDirectoryId(_T(""))
{
	m_szCmdName = _T("fetch content");
}

C_NetCmdFetchContent::~C_NetCmdFetchContent(void)
{
}

BOOL C_NetCmdFetchContent::HandleRequest(Poco::Net::StreamSocket& sktClient, 
	LPCTSTR pszCmdStream, C_DBOperate& DBOperate, LPCTSTR pszMac)
{
	if( ! m_NetCmdDecomposer.Decompose(pszCmdStream))
		return FALSE ;

	tstring szRecentlyReq , szDirectoryId;

	szRecentlyReq = m_NetCmdDecomposer.GetCmdPara(_T("recently request"));
	szDirectoryId = m_NetCmdDecomposer.GetCmdPara(_T("directory id"));

	////////////////////////////////////////////////////////////////////////////
	cout<<"MAC : "<<pszMac<<"   DirID : "<<szDirectoryId<<endl;

	if (DBOperate.IsHaveDirectoryIndex(szDirectoryId.c_str()))
	{
		if (DBOperate.IsUpdateContent(szDirectoryId.c_str() , szRecentlyReq.c_str()))
		{
			m_szDirectoryId = szDirectoryId;
			m_szResult = _T("success");
		}
		else
			m_szResult = _T("noupdate");
	}
	else
		m_szResult = _T("fail");

	HandleResponse(sktClient, DBOperate);
	return TRUE;
}

BOOL C_NetCmdFetchContent::HandleResponse(Poco::Net::StreamSocket& sktClient, C_DBOperate& DBOperate )
{
	m_NetCmdComposer.SetCmdName(m_szCmdName.c_str());
	m_NetCmdComposer.SetCmdType(_T("response"));

	C_ContentSet contentset;

	if (m_szResult.compare(_T("success")) == 0)
	{
		DBOperate.GetContentByDirectoryId(contentset, m_szDirectoryId.c_str());
		m_NetCmdComposer.SetResult( _T("success"));
	}

	else if (m_szResult.compare(_T("noupdate")) == 0)
		m_NetCmdComposer.SetResult( _T("noupdate"));

	else if (m_szResult.compare(_T("fail")) == 0)
		m_NetCmdComposer.SetResult( _T("fail"));

	ContentSort(contentset);//sort by queue

	m_NetCmdComposer.AddContent(contentset , m_szDirectoryId.c_str());
	tstring szResponseCmd = m_NetCmdComposer.Compose();
	
	sktClient.sendBytes(szResponseCmd.c_str(), strlen(szResponseCmd.c_str()));

	ReleaseSet(contentset);

	return TRUE;
}

void C_NetCmdFetchContent::ReleaseSet( C_ContentSet& contentset )
{
	for (int nIndex = 0; nIndex < contentset.size(); ++nIndex)
	{
		Content* pContent = contentset.at(nIndex);
		delete pContent;
		pContent = NULL;
	}

	contentset.clear();
}

void C_NetCmdFetchContent::ContentSort( C_ContentSet& contentset )
{
	if(contentset.size() < 1)
		return;

	sort(contentset.begin(), contentset.end(), lessData());
}
