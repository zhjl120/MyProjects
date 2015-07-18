#include "StdAfx.h"
#include "C_NetCmdFetchChannels.h"


C_NetCmdFetchChannels::C_NetCmdFetchChannels()
{
	m_szCmdName = _T("fetch channels");
}

C_NetCmdFetchChannels::~C_NetCmdFetchChannels(void)
{
}

BOOL C_NetCmdFetchChannels::HandleRequest(Poco::Net::StreamSocket& sktClient, LPCTSTR pszCmdStream, 
	C_DBOperate& DBOperate, LPCTSTR pszMac )
{
	if( ! m_NetCmdDecomposer.Decompose(pszCmdStream))
		return FALSE;

	tstring szRecentlyReq = m_NetCmdDecomposer.GetCmdPara(_T("recently request"));

	if (DBOperate.IsUpdateChannels(szRecentlyReq.c_str()))
		m_szResult = _T("success");
	else
		m_szResult = _T("noupdate");

	HandleResponse(sktClient, DBOperate, pszMac);

	return TRUE;
}

BOOL C_NetCmdFetchChannels::HandleResponse(Poco::Net::StreamSocket& sktClient, C_DBOperate& DBOperate, LPCTSTR pszMac)
{
	m_NetCmdComposer.SetCmdName(m_szCmdName.c_str());
	m_NetCmdComposer.SetCmdType(_T("response"));
	
	C_DirectoryInfoSet channelSet;
	
	if (m_szResult.compare(_T("success")) == 0)
	{
		DBOperate.GetChannelList(channelSet);
		m_NetCmdComposer.SetResult( _T("success"));
	}

	else if (m_szResult.compare(_T("noupdate")) == 0)
		m_NetCmdComposer.SetResult( _T("noupdate"));

	else if (m_szResult.compare(_T("fail")) == 0)
		m_NetCmdComposer.SetResult( _T("fail"));

	m_NetCmdComposer.AddContent(channelSet);
	tstring szResponseCmd  = m_NetCmdComposer.Compose();

	sktClient.sendBytes(szResponseCmd.c_str(), strlen(szResponseCmd.c_str()));

	ClearChannelSet(channelSet);

	return TRUE;
}

void C_NetCmdFetchChannels::ClearChannelSet(C_DirectoryInfoSet& channelSet)
{
	for (int nIndex = 0; nIndex < channelSet.size(); ++nIndex)
	{
		DirectoryInfo* pDirInfo = channelSet.at(nIndex);
		delete pDirInfo;
		pDirInfo = NULL;
	}

	channelSet.clear();
}
