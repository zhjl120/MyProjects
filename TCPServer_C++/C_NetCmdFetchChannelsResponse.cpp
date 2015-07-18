#include "StdAfx.h"
#include "C_NetCmdFetchChannelsResponse.h"


C_NetCmdFetchChannelsResponse::C_NetCmdFetchChannelsResponse(LPCTSTR pszCmdName):
C_NetCommand(pszCmdName)
{
	m_szCmdType =_T("response");
}

C_NetCmdFetchChannelsResponse::~C_NetCmdFetchChannelsResponse(void)
{
}

BOOL C_NetCmdFetchChannelsResponse::HandleResponse( C_NetCommand* pszRequestCmd , C_DBOperate& DBOperate )
{
	m_NetCmdComposer.SetCmdName(m_szCmdName.c_str());
	m_NetCmdComposer.SetCmdType(m_szCmdType.c_str());

	tstring szHandleReqResult = pszRequestCmd->GetResult();

	if (szHandleReqResult.compare(_T("success")) == 0)
	{
		DBOperate.GetChannelRecords(m_ChannelSet);
		//m_szResult = _T("success");
		m_NetCmdComposer.SetResult( _T("success"));
	}

	else if (szHandleReqResult.compare(_T("noupdate")) == 0)
	{
		//m_szResult = _T("noupdate");
		m_NetCmdComposer.SetResult( _T("noupdate"));
	}

	else if (szHandleReqResult.compare(_T("fail")) == 0)
	{
		//m_szResult =  _T("fail");
		m_NetCmdComposer.SetResult( _T("fail"));
	}

	m_NetCmdComposer.AddContent(m_ChannelSet);
	m_szCmdStream = m_NetCmdComposer.Compose();

	ClearChannelSet();

	return TRUE;
}

BOOL C_NetCmdFetchChannelsResponse::ClearChannelSet()
{
	for (int nIndex = 0; nIndex < m_ChannelSet.size(); ++nIndex)
	{
		ChannelContent* pContent = m_ChannelSet.at(nIndex);
		delete pContent;
		pContent = NULL;
	}

	m_ChannelSet.clear();

	return TRUE;
}
