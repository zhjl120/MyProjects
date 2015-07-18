#include "StdAfx.h"
#include "C_NetCmdFetchChannelContent.h"

C_NetCmdFetchChannelContent::C_NetCmdFetchChannelContent(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType = _T("request");
}

C_NetCmdFetchChannelContent::~C_NetCmdFetchChannelContent(void)
{
}

BOOL C_NetCmdFetchChannelContent::HandleRequest( LPCTSTR pszCmdStream , C_DBOperate& DBOperate )
{
	if( ! m_NetCmdDecomposer.Decompose(pszCmdStream))
		return FALSE ;

	tstring szRecentlyReq , szChannelIndex;
	szRecentlyReq = m_NetCmdDecomposer.GetCmdPara(_T("recently request"));
	szChannelIndex = m_NetCmdDecomposer.GetCmdPara(_T("channel index"));
	
	cout<<"Channel Index:"<<szChannelIndex<<endl;

	if (DBOperate.IsHaveChannelIndex(szChannelIndex.c_str()))
	{
		if (DBOperate.IsUpdateChannelContent(szChannelIndex.c_str() , szRecentlyReq.c_str()))
		{
			m_szChannelIndex = szChannelIndex;
			m_szResult = _T("success");
		}
		else
		{
			m_szResult = _T("noupdate");
		}
	}
	else
		m_szResult = _T("fail");

	return TRUE;
}

LPCTSTR C_NetCmdFetchChannelContent::GetChannelIndex()
{
	return m_szChannelIndex.c_str();
}
